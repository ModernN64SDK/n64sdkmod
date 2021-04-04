/*======================================================================*/
/*		NuSYS							*/
/*		nuaumgr.c						*/
/*									*/
/*		Copyright (C) 1997, NINTENDO Co,Ltd.			*/
/*	--/--/--	Created  by K.Ohki(SLANP)			*/
/*	98/12/10	Modified by K.Ohki(SLANP)			*/
/*======================================================================*/
/* $Id: nuaumgr.c,v 1.15 1999/06/10 05:01:50 ohki Exp $		*/
/*======================================================================*/
#include <nusys.h>
#include <nualsgi.h>


static s16*	auBuffer_ptr[3];	/* Audio buffer */
static void	nuAuMgr(void* arg);
static s32	minFrameSampleSize;
static s32	maxFrameSampleSize;

/*----------------------------------------------------------------------*/
/*	nuAuMgrInit - Initializes the Audio Manager			*/
/*	IN:	pointer to start of heap buffer				*/
/*	RET:	size of heap area used					*/
/*----------------------------------------------------------------------*/
s32 nuAuMgrInit(void* heap_ptr, u32 size, ALSynConfig* synConfig)
{

    u32	sampleSize;
    u32	frameRate;
    
    nuAuTaskStop = NU_AU_TASK_RUN;	/* Enable task execution. */
    
    frameRate = nuScGetFrameRate();	/* Obtain the frame rate. */
    
    /* Calculate the number of samples required per frame.  Round upward.*/
    nuAuFrameSampleSize = (nuAuRetraceCount * synConfig->outputRate + frameRate)/frameRate;
    nuAuFrameSampleSize += nuAuExtraSampleSize;
    sampleSize = nuAuFrameSampleSize + nuAuFrameSampleSize / 4;    
#ifdef	N_AUDIO
    sampleSize = ((sampleSize + NU_AU_AUDIO_SAMPLES - 1) / NU_AU_AUDIO_SAMPLES) * NU_AU_AUDIO_SAMPLES;	
    minFrameSampleSize = sampleSize;
    maxFrameSampleSize = sampleSize + NU_AU_AUDIO_SAMPLES;
#else
    sampleSize = (sampleSize + NU_AU_AUDIO_SAMPLES - 1) & ~0x0f;
    minFrameSampleSize = nuAuFrameSampleSize & ~0x0f;
    maxFrameSampleSize = sampleSize + NU_AU_AUDIO_SAMPLES;
#endif	/* N_AUDIO */

    /* Initialize the audio heap. */
    nuAuHeapInit(&nuAuHeap, heap_ptr, size);

    /* Allocate the command list buffer. */
    nuAuCmdListBuf = nuAuHeapAlloc(nuAuAcmdLen * sizeof(Acmd));

    /* Allocate the audio buffer. */
    auBuffer_ptr[0] = nuAuHeapAlloc(maxFrameSampleSize * sizeof(s32));
    auBuffer_ptr[1] = nuAuHeapAlloc(maxFrameSampleSize * sizeof(s32));
    auBuffer_ptr[2] = nuAuHeapAlloc(maxFrameSampleSize * sizeof(s32));
    
    /* Allocate the Audio Manager stack area. */
    nuAuMgrStack = nuAuHeapAlloc(NU_AU_STACK_SIZE);

    /* Initialize the synthesizer driver.	*/
    synConfig->dmaproc    = nuAuDmaNew;
    synConfig->heap	  = &nuAuHeap;
    synConfig->outputRate = osAiSetFrequency(synConfig->outputRate);
    alInit(&nuAuGlobal, synConfig);
    
    nuAuTask.msg			= 0;/* reply with this message */
    nuAuTask.list.t.type		= M_AUDTASK;
    nuAuTask.list.t.ucode_boot	     	= (u64*)rspbootTextStart;
    nuAuTask.list.t.ucode_boot_size   	=
	((int) rspbootTextEnd - (int) rspbootTextStart);
#ifdef N_AUDIO
    nuAuTask.list.t.ucode	     	= (u64*) n_aspMainTextStart;
    nuAuTask.list.t.ucode_data        	= (u64 *) n_aspMainDataStart;
#else
    nuAuTask.list.t.ucode	     	= (u64*) aspMainTextStart;
    nuAuTask.list.t.ucode_data       	= (u64 *) aspMainDataStart;
#endif	/* N_AUDIO */
    nuAuTask.list.t.ucode_data_size   	= SP_UCODE_DATA_SIZE;
    nuAuTask.list.t.dram_stack	     	= (u64 *) NULL;
    nuAuTask.list.t.dram_stack_size   	= 0;
    nuAuTask.list.t.output_buff	     	= (u64 *) NULL;
    nuAuTask.list.t.output_buff_size  	= 0;
    nuAuTask.list.t.yield_data_ptr    	= NULL;
    nuAuTask.list.t.yield_data_size   	= 0;

    nuAuSeqPlayer[0].mode 	= 0;
    nuAuSeqPlayer[1].mode 	= 0;
    nuAuSeqPlayer[0].data_ptr 	= NULL;
    nuAuSeqPlayer[1].data_ptr 	= NULL;    

    /* Start the Audio Manager. */
    osCreateThread(&nuAuMgrThread, NU_AU_MGR_THREAD_ID, nuAuMgr, (void*)NULL,
		   (nuAuMgrStack + NU_AU_STACK_SIZE/sizeof(u64)),
		   NU_AU_MGR_THREAD_PRI);
    osStartThread(&nuAuMgrThread);

    /* Return the size of the heap area used.	*/
    return nuAuHeapGetUsed();
}


/*----------------------------------------------------------------------*/
/*	nuAuMgr - Audio Manager						*/
/*	IN:	nothing							*/
/*	RET:	nothing							*/
/*----------------------------------------------------------------------*/
static void nuAuMgr(void* arg)
{
    OSMesgQueue nuAuRtnMesgQ;
    OSMesg	nuAuRtnMesgBuf;
    OSMesg	nuAuMsgBuf[NU_AU_MESG_MAX];
    s32		sampleSize[3];
    NUScMsg*	mesg_type;
    s32		samplesLeft, Samples=0;    
    Acmd*	cmdListAfter_ptr;    
    s32		cmdList_len;
    u32		status;
    s32		frameSampleSize;
    u32		bufCnt;
    u32		bufPtr;
    s32		readCnt;

    
    osCreateMesgQueue(&nuAuMesgQ, nuAuMsgBuf, NU_AU_MESG_MAX);

    /* Wait queue for messages from the audio task thread.  */
    osCreateMesgQueue(&nuAuRtnMesgQ, &nuAuRtnMesgBuf, 1);

    /* Register a retrace client with the Scheduler. */
    nuScAddClient(&nuAuClient, &nuAuMesgQ, NU_SC_RETRACE_MSG | NU_SC_PRENMI_MSG);

    frameSampleSize = nuAuFrameSampleSize * 2  + nuAuFrameSampleSize / 2;

    cmdList_len  = 0;
    bufCnt = 0;
    readCnt = 0;
    bufPtr = 0;
    while(1){
	(void)osRecvMesg(&nuAuMesgQ, (OSMesg*)&mesg_type, OS_MESG_BLOCK);
#ifdef	NU_DEBUG
	if((nuAuDebFlag & NU_AU_DEBUG_RETRACEOFF)
	   &&(*mesg_type == NU_SC_RETRACE_MSG)){
	    continue;
	}
#endif	/* NU_DEBUG */
	switch(*mesg_type){
#ifdef	NU_DEBUG
	case NU_AU_AI_DEBUG_MSG:
#endif	/* NU_DEBUG */
	case NU_SC_RETRACE_MSG:
	    /* Obtain the status of the AI DMA register.  */
	    status = osAiGetStatus();

	    if(status & AI_STATUS_FIFO_FULL){
		continue;
	    }

    	    /* Get the number of samples remaining in the AI buffer. */
	    samplesLeft = osAiGetLength() >> 2;

#ifdef	NU_DEBUG
	    if(nuAuDebFlag & NU_AU_DEBUG_NORMAL){
		if(!samplesLeft && (nuAuFrameCounter > 4)){
		    osSyncPrintf("nuAuMgr: no samples left!!\n");
		}
	    }
#endif	/* NU_DEBUG */
	    /* This section is not executed the first time through the loop */
	    /* because the data have not yet been created the first time through.	*/
	    if(bufCnt){
#ifdef	NU_DEBUG
		if(!(nuAuDebFlag & NU_AU_DEBUG_DISABLEAI)){
#endif	/* NU_DEBUG */
		/*  Swap the buffer. As a result, AI data are sent and a tone is sounded. */
		osAiSetNextBuffer(auBuffer_ptr[readCnt], sampleSize[readCnt]<<2);
#ifdef	NU_DEBUG
		}
#endif	/* NU_DEBUG */
		Samples = sampleSize[readCnt];
		readCnt = (readCnt + 1) % 3;
		bufCnt--;
	    }

	    /* Start the task. Do not start the task if there is no command list. */
	    if(cmdList_len && (nuAuTaskStop)) {	
#ifdef	NU_DEBUG
		if(!(nuAuDebFlag & NU_AU_DEBUG_DISABLETASK)){
#endif	/* NU_DEBUG */
		nuAuTask.list.t.data_ptr  = (u64 *)nuAuCmdListBuf;
		nuAuTask.list.t.data_size =
		    (cmdListAfter_ptr - nuAuCmdListBuf) * sizeof(Acmd);
		nuAuTask.msgQ		 = &nuAuRtnMesgQ;
		osSendMesg(&nusched.audioRequestMQ, (OSMesg*)&nuAuTask, OS_MESG_BLOCK);
		
		osRecvMesg(&nuAuRtnMesgQ, NULL, OS_MESG_BLOCK);
		
		
#ifdef	NU_DEBUG
		}
#endif	/* NU_DEBUG */
		/* Clear DMA buffers.			*/
		/* Clear buffers using less that 2 frames. */
		
		nuAuCleanDMABuffers();
		cmdList_len = 0;		
		bufPtr = (bufPtr + 1) % 3;
		bufCnt++;
	    }

	    /* Controlling the sample number.				     */
	    /* Based on the number of samples currently being transferred    */
	    /* by DMA and the number of samples to be transferred next,      */
	    /* compute the number of samples created in these frames.        */
	    /* Taking the difference between this total and the number of    */
	    /* samples per 1.25 frames, 				     */
	    /* control the sample number so that the total number of samples */
	    /* in the FIFO buffer is equal to the number in 1.25 frames.     */
	    
	    Samples = frameSampleSize - (Samples + samplesLeft);
	    
	    if(Samples > maxFrameSampleSize){
		Samples = maxFrameSampleSize;
	    } else if(Samples < minFrameSampleSize){
		Samples = minFrameSampleSize;
	    } else {
#ifdef N_AUDIO
		Samples = ((Samples + NU_AU_AUDIO_SAMPLES -1)/NU_AU_AUDIO_SAMPLES) * NU_AU_AUDIO_SAMPLES;
#else
		Samples = (Samples + NU_AU_AUDIO_SAMPLES - 1) & ~0x0f;
#endif	/* N_AUDIO */
	    }

	    /* Create the audio command list. 		*/
	    cmdListAfter_ptr = alAudioFrame(nuAuCmdListBuf, &cmdList_len,
					    (s16*)osVirtualToPhysical(auBuffer_ptr[bufPtr]),
					    Samples);
	    sampleSize[bufPtr] = Samples;
	    Samples = 0;
	    
	    
	    /* Sequence control 	*/
	    if(nuAuMgrFunc){
		(*nuAuMgrFunc)();
	    }
#ifdef NU_DEBUG
	    /* Update the maximum size of the audio command list. */
	    if(cmdList_len >= nuAuDebAcmdLenMax){
		nuAuDebAcmdLenMax = cmdList_len;
	    }
	    if(cmdList_len >= nuAuAcmdLen){
		if(nuAuDebFlag & NU_AU_DEBUG_NORMAL){
		    osSyncPrintf("nuAuMgr: cmdlist_len %d is too big.\n",
				 cmdList_len);
		}
		nuAuDebStatus |= NU_AU_DEBUG_ACMDBUFOVER;
	    }
#endif /* NU_DEBUG */
	    
	    /* If a PRE NMI message occurs, continue to call the subsequent function. */
	    if((u32)nuAuPreNMIFunc && nuAuPreNMI){
		(*nuAuPreNMIFunc)(NU_SC_RETRACE_MSG, nuAuPreNMI);
		nuAuPreNMI++;
	    }
	    break;
	case NU_SC_PRENMI_MSG:
	    /* Call the function if a PRE NMI message occurs. */
	    if(nuAuPreNMIFunc){
		(*nuAuPreNMIFunc)(NU_SC_PRENMI_MSG, nuAuPreNMI);
	    }
	    nuAuPreNMI++;
	    break;
	default:
#ifdef NU_DEBUG
	    osSyncPrintf("nuAuMgr: Unknown message %d received \n", *mesg_type);
#endif /* NU_DEBUG */
	    break;
        }
    }
}

