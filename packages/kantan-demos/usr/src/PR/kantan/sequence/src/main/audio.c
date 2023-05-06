/*============================================================================

		NINTENDO64 TECHNICAL SUPPORT CENTER 
		
		    NINTENDO64 SAMPLE PROGRAM 1_3(n_audio)

		Copyright (C) 1997, NINTENDO Co,Ltd.

============================================================================*/
/*============================================================================
  audio.c

  Ver0.1  1996/11/15
          Created for sample for NTSC scheduler 

  Ver0.1a 1996/11/19
         Fixed the intermittent sound
         Fixed the error which occurred when compiled for TYPE0 MIDI 
  Ver0.2  1996/12/25
          Added auSeqPlayerState function 
          Added auSeqPlayerStop function
          Added  NMI message in audioProc function 
          Fixed  to fade out  with PRE NMI message
          Changed the comment to SJIS
  Ver0.3  1997/6/23
          Changed to naudio version

  Ver0.4  1997/7/30
          Fixed to wait the initial task activation for 1 frame
          Fixed the problem that the sound is intermitted in DMA process
  <!!!USE SJIS CODE!!!>

 This library is created for the scheduler (MN scheduler) that was developed by NINTENDO Technical Support Center.  If other scheduler is used,  some changes need to be made.

 Only the minimum functions are created.  Other necessary functions are to be created by each user.
As parameter adjustment is not sufficient,  please adjust parameters by changing audio.h file.  
  
This source can be used freely, however, it is appreciated if the feedback for bug information etc is given.

(1) Specifications
Two sequence players are created.  That is,  two music can be played back simultaneously.   However, please note that if the maximum number of sound created reaches the set value, it stops.  

(2) Usage
   1. Call auAudioInit() and initialize the parameter and activate the synthesizer.  
   2. Call auSeqplayerInit() and set the necessary data for the sequence player.  
       Call  auSndPlayerInit() and set the necessary data for the sound player.  
   3.  After activating the scheduler using nnCreateScheduler(), activate the audio thread using  
       auCreateAudioThread().
   4. When the sequence is played back,  specify the sequence player that is played back and the file number to play back using auSeqPlayerSetFile(), and read the sequence file.  And play back using 
auSeqPlayerPlay().  When sound is made, the sound of the sound number that was specified by auSndPlay  (sound number) will be played back. 
============================================================================*/
#include <ultra64.h>
#include <PR/ramrom.h>
#include <assert.h>
#include "nnsched.h"

#define NAUDIO		/* use naudio ucode */
#undef NAUDIO		/*  n_audio is not used for this sample  */

#ifdef NAUDIO
#include <PR/n_libaudio.h>
#include <PR/n_libaudio_s_to_n.h>
#endif /* NAUDIO */
#include "audio.h"

ALHeap audio_heap;                          /* audio heap structur */
u8     audio_heap_buf[AUDIO_HEAP_SIZE];     /* audio heap buffer  */
Acmd   *audio_cmdlist_ptr[2];                   /* cmd list    */
OSTask *audio_tlist_ptr[2];                     /* audio task list */
s16    *audio_buffer_ptr[3];                    /* audio output buffer */

ALBank *soundBank_ptr = NULL;
ALBankFile *midi_buffer_ptr = NULL;             /* seq midi bank data */
ALBankFile *snd_buffer_ptr = NULL;            /* sound bank data */
ALSeqFile  *seqHeaderfile_ptr = NULL;           /* seqence header file */
OSMesgQueue audioDmaMessageQ;
OSMesgQueue audioRomMessageQ;
OSMesg      audioDmaMessageBuf[AUDIO_DMA_QUEUE_SIZE];	/*of DMA */
OSMesg      audioRomMessageBuf;
OSIoMesg    audioDmaIOMesgBuf[AUDIO_DMA_QUEUE_SIZE];
s32         nextDMA = 0;

static OSThread audioThread;
static u64      audioThreadStack[AUDIO_STACKSIZE/sizeof(u64)];

ALGlobals    audio_global;
ALSynConfig  audio_config;

/*-----sound player -----*/
ALSndPlayer   sndplayer;
ALSndpConfig  sndplayer_config;
ALInstrument* snd_inst;
ALBank        *snd_bank_ptr;
ALSndId       *snd_id;
u32           snd_alloc_num;

/* DMA buffer that is used for DMA callback routine of the sequence player */
typedef struct {
    ALLink      node;        /* link list pointer */
    int         startAddr;   /* address in ROM of data */
    u32         lastFrame;   /* value of frame counter that is used last  */
    char        *ptr;        /*pointer value to buffer  */
} DMABuffer;

/* DMA status that is used for DMA callback routine of the sequence player */
/* structure to store usage status of DMABuffer                                 */
typedef struct {
    u8          initialized;  /* a flag to tell whether or not initialized    */
    DMABuffer   *firstUsed;   /* head of DMABuffer link list that is used */
    DMABuffer   *firstFree;   /*  head of free DMABuffer link list      */

} DMAState;

Audio_seqplayer seqplayer[2];
DMAState    dmaState;
DMABuffer   dmaBuffs[AUDIO_DMA_BUFFER_NUM];
u32         audio_framecnt = 0;
ALDMAproc dmaNew(DMAState **);


#ifdef  _AUDIO_DEBUG_DMA_
static u32 dma_frame_max = 0;   /* maximum value of DMA call back */
static u32 dma_call_cnt =0;     /* the number of calling of DMA call back */
static u32 dma_use_cnt =0;      /* actual number times that DMA was used */
#endif /* _AUDIO_DEBUG_DMA_ */

#ifdef NAUDIO
u8 min_only_one = 1;
#endif /* NAUDIO */

/*-----------------------------------------------------------------------------
  DMA transfer (used for audio library)
  u32   src_addr : source address for reading
  void* dest_addr: destination memory address
  u32   size     : transfer size
-----------------------------------------------------------------------------*/
void auRomRead(u32 src_addr, void* dest_addr, u32 size)
{
  OSIoMesg    DmaIoMessageBuf;
  
  /* make CPU cache invalid */
  osInvalDCache((void*)dest_addr, (s32)size);

  /* start DMA read */
  osPiStartDma(&DmaIoMessageBuf, OS_MESG_PRI_NORMAL, OS_READ, src_addr,
	       dest_addr, size, &audioRomMessageQ);

  /* wait DMA read end */
  (void)osRecvMesg(&audioRomMessageQ, NULL, OS_MESG_BLOCK);

}


/*-----------------------------------------------------------------------------
  initialize audio library

initialize parameter that is used to allocate buffer or to use for audio library 
call this first.
-----------------------------------------------------------------------------*/
void auAudioInit(void)
{

  /*initialize audio heap */
  alHeapInit(&audio_heap, audio_heap_buf, AUDIO_HEAP_SIZE);
  
  /* create command list buffer */
  audio_cmdlist_ptr[0] = alHeapAlloc(&audio_heap, 1,
				     AUDIO_CLIST_SIZE_MAX*sizeof(Acmd));
  audio_cmdlist_ptr[1] = alHeapAlloc(&audio_heap, 1,
				     AUDIO_CLIST_SIZE_MAX*sizeof(Acmd));
  
  /* create task list buffer */
  audio_tlist_ptr[0] = alHeapAlloc(&audio_heap, 1, sizeof(OSTask));
  audio_tlist_ptr[1] = alHeapAlloc(&audio_heap, 1, sizeof(OSTask));

  /* create audio buffer */
  audio_buffer_ptr[0] = alHeapAlloc(&audio_heap, 1,
				    sizeof(s32)*AUDIO_BUFFER_MAX);
  audio_buffer_ptr[1] = alHeapAlloc(&audio_heap, 1,
				    sizeof(s32)*AUDIO_BUFFER_MAX);
  audio_buffer_ptr[2] = alHeapAlloc(&audio_heap, 1,
				    sizeof(s32)*AUDIO_BUFFER_MAX);


  /* create message queue for DMA */
  osCreateMesgQueue(&audioDmaMessageQ, audioDmaMessageBuf, AUDIO_DMA_QUEUE_SIZE);
  osCreateMesgQueue(&audioRomMessageQ, &audioRomMessageBuf, 1);

  /* clear DMABuffer initialized flag*/
  dmaState.initialized = 0;

  /* initialize audio library */
  audio_config.outputRate = osAiSetFrequency(AUDIO_OUTPUT_RATE);
  audio_config.maxVVoices = AUDIO_VVOICE_MAX;  /* the number of maximum virtual voice  */
  audio_config.maxPVoices = AUDIO_PVOICE_MAX;  /* the number of maximum physical voice */
  audio_config.maxUpdates = AUDIO_UPDATE_MAX;  /*the number of maximum buffer for parameter updating */
  audio_config.dmaproc    = &dmaNew;           /* DMA call back function */
  audio_config.fxType     = AL_FX_NONE;        /* effect type */
  audio_config.heap       = &audio_heap;       /* audio heap */
  audio_config.params     = 0;                 /* custom effect */
  alInit(&audio_global, &audio_config);
}



/*---------------------------------------------------------------

   DMA CallBack for sequence player 

   DMA call back routine that is called from sequence player.  If there is necessary value for DMABuffer, return the value of buffer, and if not, read data into buffer by DMA.  By this mechanism,  the number of
DMA calls is reduced.  However,  unused portion is deleted by 2 frames.  This control may be more efficient if it is changed to LIFO as CPU cache.
    DMA  
    CPU LIFO 
 
---------------------------------------------------------------*/
s32 dmaCallBack(s32 addr, s32 len, void *state)
{
  void        *freeBuffer;
  int         delta;
  DMABuffer   *dmaPtr,*lastDmaPtr;
  s32         addrEnd,buffEnd;


  lastDmaPtr = 0;
  dmaPtr = dmaState.firstUsed;
  addrEnd = addr+len;

#ifdef _AUDIO_DEBUG_DMA_
  dma_call_cnt++;
#endif /* _AUDIO_DEBUG_DMA_ */

  while(dmaPtr) {  /* check if desired data is in buffer */

      buffEnd = dmaPtr->startAddr + AUDIO_DMA_BUFFER_SIZE;
      
      if(dmaPtr->startAddr > addr) {/* As startAdd  is aligned in ascending order in DMABuffer,  if small value comes, buffer does not need the data */
				         
	break;                   
      } else if(addrEnd <= buffEnd){  /* data is in buffer */
	dmaPtr->lastFrame = audio_framecnt; /* set value of current frame counter */
     
	/* calculate the address which has necessary data */
	freeBuffer = dmaPtr->ptr + addr - dmaPtr->startAddr;

	/*return physical address of buffer*/
	return (int) osVirtualToPhysical(freeBuffer);
      }
      lastDmaPtr = dmaPtr;
      dmaPtr = (DMABuffer*)dmaPtr->node.next;
    }

  /*
     As DMABuffer does not have desired data, set data to be transferred by DMA 
  */

  /* take one empty buffer out of DMABuffer  */
  dmaPtr = dmaState.firstFree;

/*  assert(dmaPtr);*/  /* check if buffer is there anyway */

  dmaState.firstFree = (DMABuffer*)dmaPtr->node.next;
  alUnlink((ALLink*)dmaPtr);

  /* 
     insert the acquired DMABuffer into the link list that is being used so that startAddr is in ascending order.
 
  */
  if(lastDmaPtr) {

    /* insert into link list as usual */
      alLink((ALLink*)dmaPtr,(ALLink*)lastDmaPtr);

  } else if(dmaState.firstUsed) {

    /* insert into the head of link list */
    lastDmaPtr = dmaState.firstUsed;
    dmaState.firstUsed = dmaPtr;
    dmaPtr->node.next = (ALLink*)lastDmaPtr;
    dmaPtr->node.prev = 0;
    lastDmaPtr->node.prev = (ALLink*)dmaPtr;

  } else {

    /* if DMABuffer in use does not exist */
    dmaState.firstUsed = dmaPtr;
    dmaPtr->node.next = 0;
    dmaPtr->node.prev = 0;
  
  }
  
  freeBuffer = dmaPtr->ptr;
  delta = addr & 0x1;
  addr -= delta;            /* address location is placed in 2 byte boundary for DMA */
  dmaPtr->startAddr = addr;
  dmaPtr->lastFrame = audio_framecnt;  /*set current frame counter */

  /* DMA transfer*/
  osPiStartDma(&audioDmaIOMesgBuf[nextDMA++], OS_MESG_PRI_NORMAL, OS_READ,
	       (u32)addr, freeBuffer, AUDIO_DMA_BUFFER_SIZE, &audioDmaMessageQ);

#ifdef _AUDIO_DEBUG_DMA_
  dma_use_cnt++;
#endif /* AUDIO_DEBUG_DMA_ */

  return (int) osVirtualToPhysical(freeBuffer) + delta;
}

/*-------------------------------------------------------------
 
  sequence player DMA callback set

a call back routine which is called once for initialization when registering in sequence player.  Next time, a call back routine which was returned by return will be called.
---------------------------------------------------------------*/
ALDMAproc dmaNew(DMAState **state)
{
  int         i;

  /* check if it is initialized.  If not, initialize it.  */
  if(!dmaState.initialized) {
    
    dmaState.firstFree = &dmaBuffs[0];    /*  register the head of free DMABuffer list  */
    dmaState.firstUsed = NULL;            /* no DMABuffer in use  */

    for (i = 0; i < AUDIO_DMA_BUFFER_NUM - 1; i++){
      alLink((ALLink*)&dmaBuffs[i+1],(ALLink*)&dmaBuffs[i]);
      dmaBuffs[i].ptr = alHeapAlloc(&audio_heap, 1, AUDIO_DMA_BUFFER_SIZE);

    }
    /* set initialized flag */
    dmaState.initialized = 1;
  }
  *state = &dmaState;  /* state is never used in this case */


  return dmaCallBack;
}

/*-----------------------------------------------------------------

  DMA buffer cleanup

  search DMABuffer and add buffer which frames are not used for more than 2 frames to a link list of free buffer. 
 
-----------------------------------------------------------------*/
void auCleanDMABuffers(void)
{
  DMABuffer	*dmaPtr,*nextPtr;
  u32 i;
  OSIoMesg	*iomsg = (OSIoMesg *)0;

	for (i=0; i<nextDMA; i++){
		if(osRecvMesg(&audioDmaMessageQ, (OSMesg *)&iomsg, OS_MESG_NOBLOCK) == -1)
			osSyncPrintf("Dma not done");
	}

  dmaPtr = dmaState.firstUsed;
  while(dmaPtr) {
    nextPtr = (DMABuffer*)dmaPtr->node.next;

    /*
       add buffer which has passed AUDIO_BUFFER_USE_FRAME number to a link list of free buffer.  To  
       increase AUDIO_BUFFER_USE_FRAME number, it is necessary to increase buffer.  If decreased, the number of DMA use will increase. 
    */
    if(dmaPtr->lastFrame + AUDIO_BUFFER_USE_FRAME  < audio_framecnt){
      if(dmaState.firstUsed == dmaPtr){
	dmaState.firstUsed = (DMABuffer*)dmaPtr->node.next;
      }
      alUnlink((ALLink*)dmaPtr);
      if(dmaState.firstFree){
	alLink((ALLink*)dmaPtr,(ALLink*)dmaState.firstFree);
      }else{
	dmaState.firstFree = dmaPtr;
	dmaPtr->node.next = 0;
	dmaPtr->node.prev = 0;
      }
    }
    dmaPtr = nextPtr;
  }
#ifdef _AUDIO_DEBUG_DMA_
 
  if(nextDMA >dma_frame_max){
    dma_frame_max = nextDMA;
  }

#endif /* _AUDIO_DEBUG_DMA_ */

  nextDMA=0;
  audio_framecnt++;  
}


/*------------------------------------------------------

  audio thread 

calculate data for micro code  using a retrace message from the scheduler , and sent the message to a scheduler to activate audio task.
 As it is created by auCreateAudioThread(), there is no need to activate from outside. 
  

------------------------------------------------------*/
void audioProc(void* arg)
{
  OSMesgQueue  msgQ;
  OSMesg       msgbuf[AUDIO_MESGS_MAX];
  OSMesgQueue  rtn_msgQ;
  OSMesg       rtn_msgbuf[AUDIO_MESGS_MAX];
  NNScClient client;
  Acmd         *cmdlist_ptr;

  static s32   current_buffer = 0;
  static s32   current_audio_buffer = 2;	
  static s16   audioSamples[3] = {0,0,0};
  static s32   cmdlist_len;
  static u8    first = 1;             /*flag that processed an exceptional treatment just once*/
  s32          current_audio;
  s16*         audioOp;
  s32          frameSize;
  s32          minFrameSize;
  s32          samplesLeft = 0;  
  f32          fsize;
  OSMesgQueue  *sched_audioMQ;
  NNScTask   task[2];
  NNScTask   *au_task;
  short*       msg_type = NULL;
  u32          pre_nmi_flag =0;
  s16          get_seq_vol[2];
  s16          set_seq_vol[2];
  u32          cnt;

#ifdef _AUDIO_DEBUG_DMA_
  f32 hits;
#endif /* _AUDIO_DEBUG_DMA_ */
/*calculate the number of samples RSP processes for 1 frame.*/
  fsize = (f32)AUDIO_NUM_FIELDS * audio_config.outputRate/ (f32)60;
  frameSize = (s32) fsize;
  if(frameSize < fsize) frameSize++;

#ifdef NAUDIO
  frameSize = ((frameSize / AUDIO_SAMPLES) + 1) * AUDIO_SAMPLES;
  minFrameSize = frameSize - AUDIO_SAMPLES;
#else
  if(frameSize & 0x0f) frameSize = (frameSize & ~0xf) + 0x10;
  minFrameSize = frameSize - 16;
#endif /* NAUDIO */
  
  /* create message queue for VI retrace */
  osCreateMesgQueue( &msgQ, msgbuf, AUDIO_MESGS_MAX);
  osCreateMesgQueue( &rtn_msgQ, rtn_msgbuf, AUDIO_MESGS_MAX);

  nnScAddClient((NNSched*)arg, &client, &msgQ);
  sched_audioMQ = nnScGetAudioMQ((NNSched*)arg);
  
  while(1){
    (void)osRecvMesg(&msgQ, (OSMesg*)&msg_type, OS_MESG_BLOCK);

      switch(*msg_type){
      case NN_SC_RETRACE_MSG:      /* retrace message processing */ 
      
      /* If PRE NMI is received,  fade it out*/
      switch(pre_nmi_flag){
      case 1: /* fade out */
        for(cnt = 0;cnt < 2; cnt++){

	      /*As it is within 0.5 second approximately,  if 28frame, 0.   */
	        set_seq_vol[cnt] -= get_seq_vol[cnt]/28;  
	        if(set_seq_vol[cnt] < 0){
	          set_seq_vol[cnt] = 0;
	        }
#ifdef _AUDIO_COMPACTMIDI
	        alCSPSetVol(&seqplayer[cnt].seqplayer, set_seq_vol[cnt]);
#else
	        alSeqpSetVol(&seqplayer[cnt].seqplayer, set_seq_vol[cnt]);
#endif /* _AUDIO_COMPACTMIDI_ */
	    }
	/*
	  check if the fade out processing has ended.   If ended,  a task is not created to stop a task.
	*/
	      if((set_seq_vol[0] == 0) && (set_seq_vol[1] == 0)){
	        pre_nmi_flag++;
	      }
	      break;

      case 2:
	     /* end without creating a task */
	      continue;
	      break;
      }
#ifdef _AUDIO_DEBUG_DMA_
      if(!(audio_framecnt % 60)){
	      if(dma_call_cnt == 0 ){
	        hits =0;
	      } else {
	        hits = (((f32)dma_call_cnt-(f32)dma_use_cnt)*100.0)/(f32)dma_call_cnt;
	        }
	    osSyncPrintf("dma max = %d /f, dma_call = %d, dma_use = %d, Hits = %f\n",
                    dma_frame_max, dma_call_cnt, dma_use_cnt,hits);
	    dma_call_cnt =0;
	    dma_use_cnt =0;
      }
#endif /* _AUDIO_DEBGU_DMA_ */

      au_task = &task[current_buffer];
      cmdlist_ptr = audio_cmdlist_ptr[current_buffer];

      samplesLeft = osAiGetLength()>> 2;   
      osAiSetNextBuffer(audio_buffer_ptr[(current_audio_buffer - 2) % 3],
			                  audioSamples[(current_audio_buffer - 2) % 3]<<2);

      current_audio = current_audio_buffer % 3;

#ifdef _AUDIO_DEBUG_DMA_
      osSyncPrintf("%d\n", nextDMA);/*frequency of DMA for every frame*/
#endif /* _AUDIO_DEBGU_DMA_ */

      auCleanDMABuffers();

      audioOp = (s16*) osVirtualToPhysical(audio_buffer_ptr[current_audio]);
#ifdef NAUDIO
      if((samplesLeft > (AUDIO_SAMPLES + AUDIO_EXTRA_SAMPLES)) && min_only_one){
	    audioSamples[current_audio] = minFrameSize;
	    min_only_one = 0;
      } else {
	    audioSamples[current_audio] = frameSize;
	    min_only_one = 1;
      }
#else
      audioSamples[current_audio] = 16 + 
            (frameSize - samplesLeft + AUDIO_EXTRA_SAMPLES) & ~0x0f;
      if(audioSamples[current_audio] < minFrameSize)
	    audioSamples[current_audio] = minFrameSize;
#endif /* NAUDIO */      
    
      cmdlist_ptr = alAudioFrame(cmdlist_ptr, &cmdlist_len, audioOp, 
                                 audioSamples[current_audio]);


      au_task->next      = 0;                    /* paranoia */
      au_task->msgQ      = &rtn_msgQ;            /* reply to when finished */
      au_task->msg       = 0;                    /* reply with this message */
      
      au_task->list.t.data_ptr    = (u64 *)audio_cmdlist_ptr[current_buffer];
      au_task->list.t.data_size   = (cmdlist_ptr -audio_cmdlist_ptr[current_buffer])* sizeof(Acmd);
      au_task->list.t.type  = M_AUDTASK;
      au_task->list.t.ucode_boot = (u64 *)rspbootTextStart;
      au_task->list.t.ucode_boot_size =
	((int) rspbootTextEnd - (int) rspbootTextStart);
#ifdef NAUDIO
      au_task->list.t.ucode = (u64 *) n_aspMainTextStart;
      au_task->list.t.ucode_data = (u64 *) n_aspMainDataStart;
#else
      au_task->list.t.ucode = (u64 *) aspMainTextStart;
      au_task->list.t.ucode_data = (u64 *) aspMainDataStart;
#endif /* NAUDIO */
      au_task->list.t.ucode_data_size = SP_UCODE_DATA_SIZE;
      au_task->list.t.dram_stack = (u64 *) NULL;
      au_task->list.t.dram_stack_size = 0;
      au_task->list.t.output_buff = (u64 *) NULL;
      au_task->list.t.output_buff_size = 0;
      au_task->list.t.yield_data_ptr = NULL;
      au_task->list.t.yield_data_size = 0;
      
      
      current_buffer ^= 1;
      current_audio_buffer++;

      if(first)
        first = 0;/*´Do not execute the first  task so that DMA ends completely. */
      else{
        osSendMesg(sched_audioMQ, (OSMesg*)&task[current_buffer], OS_MESG_BLOCK);
        osRecvMesg(&rtn_msgQ, NULL, OS_MESG_BLOCK);
      }

      break;

      case NN_SC_PRE_NMI_MSG: /* PRE NMI message */
      /* 
      !!Important!! Stop the task before NMI occurrs. Write PRE NMI message  processing here.  Sequence 
       is being faded out for this sample, but sound is not.
      */
      pre_nmi_flag++;
#ifdef _AUDIO_COMPACTMIDI_
      get_seq_vol[0] = alCSPGetVol(&seqplayer[0].seqplayer);
      get_seq_vol[1] = alCSPGetVol(&seqplayer[1].seqplayer);
#else
      get_seq_vol[0] = alSeqpGetVol(&seqplayer[0].seqplayer);
      get_seq_vol[1] = alSeqpGetVol(&seqplayer[1].seqplayer);
#endif /* _AUDIO_COMPACTMIDI_ */
      set_seq_vol[0] = get_seq_vol[0];
      set_seq_vol[1] = get_seq_vol[1];
      break;
    }
  }
  
}


/*--------------------------

  read sequence file header

---------------------------*/
void auReadSeqFileHeader(u32 src_addr)
{
  s32       seqfile_size;

  seqHeaderfile_ptr = alHeapAlloc(&audio_heap, 1, 4);
  /* read sequence file header length */ 
  auRomRead(src_addr, seqHeaderfile_ptr, 4);

  seqfile_size = 4 + seqHeaderfile_ptr->seqCount * sizeof(ALSeqData);
  seqHeaderfile_ptr = alHeapAlloc(&audio_heap, 1, seqfile_size);

  /* read sequence file header */
  auRomRead(src_addr, seqHeaderfile_ptr, seqfile_size);
}


/*----------------------------------------------------
  audio thread create and start
create and start audio thread
----------------------------------------------------*/
void auCreateAudioThread(NNSched *sched)
{

  osCreateThread(&audioThread, AUDIO_THREAD_ID, audioProc, (void *)sched,
		 (audioThreadStack+AUDIO_STACKSIZE/sizeof(u64)),
		  AUDIO_THREAD_PRI);
  osStartThread(&audioThread);

}

/*-----------------------------------------------------------------------------

  sequence player initialize

 initialize sequence player
input
  midi_start: head ROM address of bank file (.ctl)
  midi_end: end ROM address of bank file (.ctl)
  seqheader_start:head ROM address of sequence file (.sbk)
  seqheader_end:end ROM address of sequence file (.sbk)
  midi_table_star:head ROM address of wave table file (.tbl)
-----------------------------------------------------------------------------*/
void auSeqPlayerInit(u8* midi_start, u8* midi_end,
		     u8* seqheader_start, u8* seqheader_end,
		     u8* midi_table_start) 
{
  ALBank *midiBank_ptr;
  u32    size;
  /* create sequence data buffer */
  seqplayer[0].seqdata_ptr = alHeapAlloc(&audio_heap, 1, AUDIO_SEQDATA_SIZE_MAX);
  seqplayer[1].seqdata_ptr = alHeapAlloc(&audio_heap, 1, AUDIO_SEQDATA_SIZE_MAX);

  /* initialize sequence player 0 */
  seqplayer[0].seqconfig.maxVoices   = AUDIO_SEQ_VVOICE_MAX;/* maximum virtual voice number  */
  seqplayer[0].seqconfig.maxEvents   = AUDIO_SEQ_EVTCOUNT_MAX;  /* maximum internal event number */
  seqplayer[0].seqconfig.maxChannels = AUDIO_SEQ_CHANNEL_MAX;/* maximum MIDI channel number */
  seqplayer[0].seqconfig.heap        = &audio_heap;      /* audio heap */
  seqplayer[0].seqconfig.initOsc     = 0;
  seqplayer[0].seqconfig.updateOsc   = 0;
  seqplayer[0].seqconfig.stopOsc     = 0;
  seqplayer[0].seqconfig.debugFlags  = 0;

#ifdef _AUDIO_COMPACTMIDI_
  alCSPNew(&seqplayer[0].seqplayer, &seqplayer[0].seqconfig);
#else
  alSeqpNew(&seqplayer[0].seqplayer, &seqplayer[0].seqconfig);
#endif /* _AUDIO_COMPACTMIDI_ */

  /* initialize sequence player 1 */
  seqplayer[1].seqconfig.maxVoices   = AUDIO_SEQ_VVOICE_MAX;/* maximum virtual voice number */
  seqplayer[1].seqconfig.maxEvents   = AUDIO_SEQ_EVTCOUNT_MAX;  /* maximum internal event number */
  seqplayer[1].seqconfig.maxChannels = AUDIO_SEQ_CHANNEL_MAX;/* maximum MIDI channel number */
  seqplayer[1].seqconfig.heap        = &audio_heap;      /* audio heap */
  seqplayer[1].seqconfig.initOsc     = 0;
  seqplayer[1].seqconfig.updateOsc   = 0;
  seqplayer[1].seqconfig.stopOsc     = 0;
  seqplayer[1].seqconfig.debugFlags  = 0;

#ifdef _AUDIO_COMPACTMIDI_
  alCSPNew(&seqplayer[1].seqplayer, &seqplayer[1].seqconfig);
#else
  alSeqpNew(&seqplayer[1].seqplayer, &seqplayer[1].seqconfig);
#endif /* _AUDIO_COMPACTMIDI_ */

  /* read seqfileheader data */
  auReadSeqFileHeader((u32)seqheader_start);
  alSeqFileNew(seqHeaderfile_ptr, seqheader_start);

  /* read midi bank data */
  size = (u32)midi_end-(u32)midi_start;
  midi_buffer_ptr = alHeapAlloc(&audio_heap, 1, size);
  auRomRead((u32)midi_start, midi_buffer_ptr, size);

  /* specify instrument bank used by sequencer */
  alBnkfNew(midi_buffer_ptr, midi_table_start);
  midiBank_ptr = midi_buffer_ptr->bankArray[0];

#ifdef _AUDIO_COMPACTMIDI_
  alCSPSetBank(&seqplayer[0].seqplayer, midiBank_ptr);
  alCSPSetBank(&seqplayer[1].seqplayer, midiBank_ptr);
#else
  alSeqpSetBank(&seqplayer[0].seqplayer, midiBank_ptr);
  alSeqpSetBank(&seqplayer[1].seqplayer, midiBank_ptr);
#endif /* _AUDIO_COMPACTMIDI_ */

}


/*-----------------------------------------------------------------------------
  sequence player seqno setup

read sequence data

 u32 seqplayer: sequence player number
 s32 seqno    : sequence number 
-----------------------------------------------------------------------------*/
void auSeqPlayerSetFile(u32 seqplayer_no, s32 seqno)
{
  s32 seqdata_len;
  u8*  seqdata_offset;

#ifdef _AUDIO_DEBUG_
  if(seqHeaderfile_ptr->seqCount < seqno){
    osSyncPrintf("seqence no is over!!(seqcount = %d)\n",
		 seqHeaderfile_ptr->seqCount);
    return;
  }
#endif /* _AUDIO_DEBUG_ */

  seqdata_offset = seqHeaderfile_ptr->seqArray[seqno].offset;
  seqdata_len = seqHeaderfile_ptr->seqArray[seqno].len;
  if(seqdata_len & 0x01) seqdata_len++;

  /* read sequence data */
  auRomRead((u32)seqdata_offset, seqplayer[seqplayer_no].seqdata_ptr, seqdata_len);

  seqplayer[seqplayer_no].seqno = seqno;

}

/*----------------------------------------------------------------------------
  play sequence

Input:
  seqplayer_no: sequence player number to start replay

if the sequence player is not AL_STOPPED, FALSE will be returned.
 
----------------------------------------------------------------------------*/
int auSeqPlayerPlay(u32 seqplayer_no)
{
#ifndef _AUDIO_COMPACTMIDI_
  s32 seqdata_len;
  u32 seqno;
#endif /* _AUDIO_COMPACTMIDI_ */

#ifdef _AUDIO_DEBUG_
  if(seqplayer_no > 1){
    osSyncPrintf("seqplayer_no over!!\n");
    return FALSE;
  }
#endif /* _AUDIO_DEBUG_ */

  /* check seqplayer state. */
  if(seqplayer[seqplayer_no].seqplayer.state != AL_STOPPED){
#ifdef _AUDIO_DEBUG_
    osSyncPrintf("sequence player %d is playing\n",seqplayer_no);
#endif /* _AUDIO_DEBU_ */

    return FALSE;
  }

#ifdef _AUDIO_COMPACTMIDI_
  alCSeqNew(&seqplayer[seqplayer_no].sequence, seqplayer[seqplayer_no].seqdata_ptr);
  alCSPSetSeq(&seqplayer[seqplayer_no].seqplayer, &seqplayer[seqplayer_no].sequence);
  alCSPPlay(&seqplayer[seqplayer_no].seqplayer);
#else
  seqno = seqplayer[seqplayer_no].seqno;
  seqdata_len = seqHeaderfile_ptr->seqArray[seqno].len;
  alSeqNew(&seqplayer[seqplayer_no].sequence, seqplayer[seqplayer_no].seqdata_ptr, seqdata_len);
  alSeqpSetSeq(&seqplayer[seqplayer_no].seqplayer, &seqplayer[seqplayer_no].sequence);
  alSeqpPlay(&seqplayer[seqplayer_no].seqplayer);
#endif /* _AUDIO_COMPACTMIDI_ */
  return TRUE;
}

/*----------------------------------------------------------------------------
   sequence stop

Input:
  seqplayer_no:sequence player number to stop 
----------------------------------------------------------------------------*/
void auSeqPlayerStop(u32 seqplayer_no)
{
#ifdef _AUDIO_COMPACTMIDI_
  alCSPStop(&seqplayer[seqplayer_no].seqplayer);
#else 
  alSeqpStop(&seqplayer[seqplayer_no].seqplayer);
#endif /* _AUDIO_COMPACTMIDI_ */
}



/*----------------------------------------------------------------------------
  sequence state

Input:
  seqplayer_no: sequence player number to acquire the status

  returns the status of sequence player
----------------------------------------------------------------------------*/
s32 auSeqPlayerState(u32 seqplayer_no)
{
#ifdef _AUDIO_COMPACTMIDI_
  /* it is used as it is in libaudio.h though not in man  */
  return alCSPGetState(&seqplayer[seqplayer_no].seqplayer);
#else 
/*  return alSeqpGetState(&seqplayer[seqplayer_no].seqplayer);*/
  return seqplayer[seqplayer_no].seqplayer.state;

#endif /* _AUDIO_COMPACTMIDI_ */
}

/*-----------------------------------------------------------------------------
  initialize sound player

Input
  sndbankstart_ptr: head ROM address of bank file (.ctl) for sound
  sndbankend_ptr:   end ROM address of  bank file (.ctl) for sound
  sndtablestart_ptr:head ROM address of wave table file (.tbl)
-----------------------------------------------------------------------------*/
void auSndPlayerInit(u8* sndbankstart_ptr, u8* sndbankend_ptr,
		     u8* sndtablestart_ptr)
{
  u32 size;
  u32 cnt;

  /* read sound bank(.ctl file) */
  size = (u32)sndbankend_ptr-(u32)sndbankstart_ptr;
  snd_buffer_ptr = alHeapAlloc(&audio_heap, 1, size);
  auRomRead((u32)sndbankstart_ptr, snd_buffer_ptr, size);

  /* soundplayer initialize */
  sndplayer_config.maxSounds   = AUDIO_SND_VOICE_MAX;
  sndplayer_config.maxEvents   = AUDIO_SND_EVTCOUNT_MAX;
  sndplayer_config.heap        = &audio_heap;
  alSndpNew(&sndplayer, &sndplayer_config);

  /* bankfile initialize */
  alBnkfNew(snd_buffer_ptr, sndtablestart_ptr);

  /*
    register sound data in sound player in the order it is registered in bank file.
  */
  snd_bank_ptr = snd_buffer_ptr->bankArray[0];
  snd_inst = snd_bank_ptr->instArray[0];
  snd_id = alHeapAlloc(&audio_heap, 1, snd_inst->soundCount*sizeof(ALSndId));

  for(cnt = 0, snd_alloc_num =0; cnt < snd_inst->soundCount; cnt++){
    snd_id[cnt] = alSndpAllocate(&sndplayer, snd_inst->soundArray[cnt]);
    if(snd_id[cnt] != -1) {
      snd_alloc_num++;
    }
  }
}

/*-----------------------------------------------------------------------------
  sound replay
Input
  sndno: sound number to replay
-----------------------------------------------------------------------------*/
void auSndPlay(u32 sndno)
{
  s32 status;

  alSndpSetSound(&sndplayer, snd_id[sndno]);
  status = alSndpGetState(&sndplayer);
  if(status == AL_STOPPED){
    alSndpPlay(&sndplayer);
  }

}

/*-----------------------------------------------------------------------------
stop sound
Input
  sndno:sound number to stop 
-----------------------------------------------------------------------------*/
void auSndStop(u32 sndno)
{
  alSndpSetSound(&sndplayer,snd_id[sndno]);
  alSndpStop(&sndplayer);
}

/*-----------------------------------------------------------------------------
  acquire the status of sound
Input
  sndno:sound number of which status is desired to be acquired 
Return:
 the status of sound that is specified by sndno
-----------------------------------------------------------------------------*/
s32 auSndGetState(u32 sndno)
{
  alSndpSetSound(&sndplayer, snd_id[sndno]);
  return alSndpGetState(&sndplayer);
}





