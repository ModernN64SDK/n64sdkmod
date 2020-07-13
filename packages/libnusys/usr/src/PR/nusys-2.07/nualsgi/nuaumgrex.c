/*======================================================================*/
/*		NuSYS							*/
/*		nuaumgr.c						*/
/*									*/
/*		Copyright (C) 1997, NINTENDO Co,Ltd.			*/
/*									*/
/*	--/--/--	Created  by K.Ohki(SLANP)			*/
/*	98/11/26	Modified by K.Ohki(SLANP)			*/
/*======================================================================*/
/* $Id: nuaumgrex.c,v 1.9 1999/06/10 05:05:47 ohki Exp $		*/
/*======================================================================*/
#include <nusys.h>
#include <nualsgi.h>

static s16*	auBuffer_ptr[2];	/* Audio buffer */
static void	nuAuMgr(void* arg);

/*----------------------------------------------------------------------*/
/*	nuAuMgrInitEx - Initializes the Audio Manager			*/
/*									*/
/*	Initializes the Audio Manager that uses AI_FIFO_EVENT.		*/
/*	IN:	Pointer to start of heap buffer				*/
/*	RET:	size of heap area used					*/
/*----------------------------------------------------------------------*/
s32 nuAuMgrInitEx(void* heap_ptr, u32 size, ALSynConfig* synConfig)
{
    s32 frameRate;
    nuAuTaskStop = NU_AU_TASK_RUN;	/* Enable task execution */
    
    frameRate = nuScGetFrameRate();	/* Get frame rate */

    /* Calculate the number of samples per frame. Round upward.*/
    nuAuFrameSampleSize = (nusched.retraceCount * synConfig->outputRate + frameRate - 1)/frameRate;
    nuAuFrameSampleSize += nuAuExtraSampleSize;
    nuAuFrameSampleSize = ((nuAuFrameSampleSize + (NU_AU_AUDIO_SAMPLES -1)) / NU_AU_AUDIO_SAMPLES) * NU_AU_AUDIO_SAMPLES;
    
    /* Initialize the audio heap. 	*/
    nuAuHeapInit(&nuAuHeap, heap_ptr, size);

    /* Reserve the command list buffer.	*/
    nuAuCmdListBuf = nuAuHeapAlloc(nuAuAcmdLen * sizeof(Acmd));

    /* Reserve the audio buffer. */
    auBuffer_ptr[0] = nuAuHeapAlloc(nuAuFrameSampleSize * sizeof(s32));
    auBuffer_ptr[1] = nuAuHeapAlloc(nuAuFrameSampleSize * sizeof(s32));
    
    /* Reserve the Audio Manager stack area.	*/
    nuAuMgrStack = nuAuHeapAlloc(NU_AU_STACK_SIZE);
    
    /* Initialize the synthesizer driver.	*/
    synConfig->dmaproc    = nuAuDmaNew;
    synConfig->heap	  = &nuAuHeap;
    synConfig->outputRate = osAiSetFrequency(synConfig->outputRate);
    alInit(&nuAuGlobal, synConfig);
    
    /* Initialize the task structure. */
    nuAuTask.msg			= 0;/* reply with this message */
    nuAuTask.list.t.type	     	= M_AUDTASK;
    nuAuTask.list.t.ucode_boot	     	= (u64*)rspbootTextStart;
    nuAuTask.list.t.ucode_boot_size   	=
	((int) rspbootTextEnd - (int) rspbootTextStart);
#ifdef N_AUDIO
    nuAuTask.list.t.ucode	     	= (u64*) n_aspMainTextStart;
    nuAuTask.list.t.ucode_data        	= (u64 *) n_aspMainDataStart;
#else
    nuAuTask.list.t.ucode	     	= (u64*) aspMainTextStart;
    nuAuTask.list.t.ucode_data        	= (u64 *) aspMainDataStart;
#endif	/* N_AUDIO */
    nuAuTask.list.t.ucode_data_size   	= SP_UCODE_DATA_SIZE;
    nuAuTask.list.t.dram_stack	    	= (u64 *) NULL;
    nuAuTask.list.t.dram_stack_size   	= 0;
    nuAuTask.list.t.output_buff	     	= (u64 *) NULL;
    nuAuTask.list.t.output_buff_size  	= 0;
    nuAuTask.list.t.yield_data_ptr    	= NULL;
    nuAuTask.list.t.yield_data_size   	= 0;

    nuAuSeqPlayer[0].mode	= 0;
    nuAuSeqPlayer[1].mode	= 0;
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
    OSMesgQueue	nuAuRtnMesgQ;
    OSMesg	nuAuRtnMesgBuf;
    OSMesg	nuAuMsgBuf[NU_AU_MESG_MAX];

    NUScMsg*	mesg_type;
    NUScMsg	aiEventMsg;
    Acmd*	cmdListAfter_ptr;
    s32		cmdList_len;
    u32		bufCnt;
    u32		bufPtr;
    s32		readCnt;
    
    osCreateMesgQueue(&nuAuMesgQ, nuAuMsgBuf, NU_AU_MESG_MAX);

    /* Queue for messages from the audio task thread.  */
    osCreateMesgQueue(&nuAuRtnMesgQ, &nuAuRtnMesgBuf, 1);

    /* Set the AI event message.	*/
    aiEventMsg = NU_AU_AI_FIFO_MSG;
    osSetEventMesg(OS_EVENT_AI, &nuAuMesgQ, (OSMesg*)&aiEventMsg);

    /* Register a RETRACE client with the Scheduler. */
    nuScAddClient(&nuAuClient, &nuAuMesgQ, NU_SC_RETRACE_MSG | NU_SC_PRENMI_MSG);

    cmdList_len  = 0;
    bufCnt = 0;
    readCnt = 0;
    bufPtr = 0;
    while(1){
	(void)osRecvMesg(&nuAuMesgQ, (OSMesg*)&mesg_type, OS_MESG_BLOCK);

#ifdef	NU_DEBUG
	if((nuAuDebFlag & NU_AU_DEBUG_FIFOOFF)
	   && (*mesg_type == NU_AU_AI_FIFO_MSG)){
	    continue;
	}
	if((nuAuDebFlag & NU_AU_DEBUG_RETRACEOFF)
	   &&(*mesg_type == NU_SC_RETRACE_MSG)){
	    continue;
	}
#endif	/* NU_DEBUG	*/
	switch(*mesg_type){
	    
	    /* An AI event is issued when the AI FIFO (register for DMA      */
	    /* settings) changes from 1 ->0.  In this case, if there are     */
	    /* synthesized waveform data that have not been transferred, DMA */
	    /* settings are performed for the FIFO register.  This mechanism */
	    /* eliminates noise. Initially, however, the FIFO is empty, so   */
	    /* AI events are not issued.  Consequently, until an AI event is */
	    /* issued, RSP commands, macrocode startup, and FIFO settings    */
  	    /* are performed using retrace messages.	Once an AI event is  */
  	    /* issued, retrace messages are not needed, so message           */
 	    /* registration is eliminated.  If for some reason the buffer    */
 	    /* cannot be set when an AI event occurs, settings are changed   */
	    /* to enable retrace messages again. 			     */
	case NU_SC_RETRACE_MSG:
	    if((bufCnt > 1) && !nuAuPreNMI){
		nuScResetClientMesgType(&nuAuClient, nuAuClient.msgType ^ NU_SC_RETRACE_MSG);
	    }
	    
	case NU_AU_AI_FIFO_MSG:
#ifdef	NU_DEBUG
	case NU_AU_AI_DEBUG_MSG:
#endif	/* NU_DEBUG */

	    /* If the buffer contains data not yet transferred, check the DMA FIFO register. */	    /* If it is OK, set the data for transfer. */
	    if(bufCnt > 0){
		u32 status;
		
		status = osAiGetStatus();
		
		/*  Switch buffers.  This causes AI data to be sent and a tone to sound. */
		if(!(status & AI_STATUS_FIFO_FULL)){
#ifdef	NU_DEBUG
		    if(!(nuAuDebFlag & NU_AU_DEBUG_DISABLEAI)){
#endif	/* NU_DEBUG */
		    osAiSetNextBuffer(auBuffer_ptr[readCnt], nuAuFrameSampleSize<<2);
#ifdef	NU_DEBUG
		    }
#endif	/* NU_DEBUG */
		    readCnt ^= 1;
		    bufCnt--;
		}
	    } else if(!(nuAuClient.msgType & NU_SC_RETRACE_MSG)){
		/* The FIFO register is empty, so AI events are not issued.	*/
		/* Consequently, the retrace message is re-registered so that it is sent.	*/
		nuScResetClientMesgType(&nuAuClient, nuAuClient.msgType | NU_SC_RETRACE_MSG);
	    }
	    
	    /* Task startup. Do not start the task if there is no command list. */
	    if((cmdList_len > 0) && (bufCnt < 2) && (nuAuTaskStop)){
#ifdef NU_DEBUG
		if(!(nuAuDebFlag & NU_AU_DEBUG_DISABLETASK)){
#endif	/* NU_DEBUG */		    
		nuAuTask.list.t.data_ptr  = (u64 *)nuAuCmdListBuf;
		nuAuTask.list.t.data_size =
		    (cmdListAfter_ptr - nuAuCmdListBuf) * sizeof(Acmd);
		nuAuTask.msgQ		= &nuAuRtnMesgQ;
		osSendMesg(&nusched.audioRequestMQ,
			   (OSMesg*)&nuAuTask, OS_MESG_BLOCK);
		
		osRecvMesg(&nuAuRtnMesgQ, NULL, OS_MESG_BLOCK);
#ifdef	NU_DEBUG
		}
#endif	/* NU_DEBUG */

                bufPtr ^=1;
                bufCnt++;
                cmdList_len = 0;
	    }
	    
	    if(cmdList_len == 0){
		/* Create the audio command list. */
		cmdListAfter_ptr = alAudioFrame(nuAuCmdListBuf, &cmdList_len,
						(s16*)osVirtualToPhysical(auBuffer_ptr[bufPtr]),
						nuAuFrameSampleSize);
		
		/* Clear DMA buffers. Clear buffers using fewer than 2 frames.*/		nuAuCleanDMABuffers();
		
		/* Sequence control 	*/
		if(nuAuMgrFunc){
		    (*nuAuMgrFunc)();
		}
		
#ifdef NU_DEBUG
		/* Update the maximum size of the audio command list. */
		if(cmdList_len > nuAuDebAcmdLenMax){
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
	    }
	    
	    /* If a PRE NMI message occurs, this function is subsequently called continuously. */
	    if((nuAuPreNMI) && (*mesg_type & NU_SC_RETRACE_MSG)){
		if((u32)nuAuPreNMIFunc){
		    (*nuAuPreNMIFunc)(NU_SC_RETRACE_MSG, nuAuPreNMI);
		}
		nuAuPreNMI++;
	    }
	break;
    case NU_SC_PRENMI_MSG:
	    /* Call the function if a PRE NMI message occurs.*/
	    if(nuAuPreNMIFunc){
		/* Re-register the retrace message so that it is sent. */
		nuScResetClientMesgType(&nuAuClient, nuAuClient.msgType | NU_SC_RETRACE_MSG);
		
		(*nuAuPreNMIFunc)(NU_SC_PRENMI_MSG, nuAuPreNMI);
	    }
	    nuAuPreNMI++;
	    break;
	default:
#ifdef NU_DEBUG
	    if(nuAuDebFlag & NU_AU_DEBUG_NORMAL){
		osSyncPrintf("nuAuMgr: Unknown message %d received \n", *mesg_type);
	    }
#endif /* NU_DEBUG */
	    break;
        }
    }
}


