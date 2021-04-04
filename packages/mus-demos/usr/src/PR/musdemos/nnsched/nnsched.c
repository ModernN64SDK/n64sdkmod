/*============================================================================

		NINTENDO64 TECHNICAL SUPPORT CENTER 
		
		    NINTENDO64 SAMPLE PROGRAM 1

		Copyright (C) 1997, NINTENDO Co., Ltd.

1997/01/23
    Added OS_TASK_LOADABLE flag in order to accommodate the addition of gSPLoadUcode to  F3DEX Ver1.20.
 
1997/02/03
     Fixed bug that arose when performance check found value larger than  _SC_GTASK_NUM and NN_SC_AUTASK_NUM.

1997/12/10
    Corrected display of warning message.

============================================================================*/

#include "nnsched.h"

/* secure stack for thread */
u64 nnScStack[NN_SC_STACKSIZE/8];
u64 nnScAudioStack[NN_SC_STACKSIZE/8];
u64 nnScGraphicsStack[NN_SC_STACKSIZE/8];
u32 framecont = 0;

#ifdef NN_SC_PERF
static u32       nnsc_perf_index = 0; /* location of internally used buffer */
static u32       nnsc_perf_flag = 0;
static NNScPerf  nnsc_perf[NN_SC_PERF_NUM];        /* measurement data  */
static NNScPerf* nnsc_perf_inptr;     /* internally used pointer */
NNScPerf* nnsc_perf_ptr;              /* pointer from external reference */
#endif  /* NN_SC_PERF */

/************************************************************
  nnScCreateScheduler()   -    Create scheduler
************************************************************/
void nnScCreateScheduler(NNSched *sc, u8 videoMode, u8 numFields)
{

  /* initialize variables */
  sc->curGraphicsTask = 0;
  sc->curAudioTask    = 0;
  sc->graphicsTaskSuspended = 0;
  sc->clientList      = 0;
  sc->firstTime       = 1; 
  sc->retraceMsg      = NN_SC_RETRACE_MSG;
  sc->prenmiMsg       = NN_SC_PRE_NMI_MSG;

  /* create message queue */
  osCreateMesgQueue(&sc->retraceMQ, sc->retraceMsgBuf, NN_SC_MAX_MESGS);
  osCreateMesgQueue(&sc->rspMQ, sc->rspMsgBuf, NN_SC_MAX_MESGS);
  osCreateMesgQueue(&sc->rdpMQ, sc->rdpMsgBuf, NN_SC_MAX_MESGS);
  osCreateMesgQueue(&sc->graphicsRequestMQ, sc->graphicsRequestBuf, 
NN_SC_MAX_MESGS);
  osCreateMesgQueue(&sc->audioRequestMQ, sc->audioRequestBuf, 
NN_SC_MAX_MESGS);
  osCreateMesgQueue(&sc->waitMQ, sc->waitMsgBuf, NN_SC_MAX_MESGS);

  /* video mode settings  */
  osCreateViManager(OS_PRIORITY_VIMGR);    
  osViSetMode(&osViModeTable[videoMode]);
  osViBlack(TRUE);

  /* register event handler */
  osViSetEvent(&sc->retraceMQ, (OSMesg)VIDEO_MSG, numFields);    
  osSetEventMesg(OS_EVENT_SP, &sc->rspMQ, (OSMesg)RSP_DONE_MSG);
  osSetEventMesg(OS_EVENT_DP, &sc->rdpMQ, (OSMesg)RDP_DONE_MSG);
  osSetEventMesg(OS_EVENT_PRENMI, &sc->retraceMQ, (OSMesg)PRE_NMI_MSG);   


  /* start scheduler thread */
  osCreateThread(&sc->schedulerThread, 19, (void(*)(void*))nnScEventHandler,
		 (void *)sc, nnScStack+NN_SC_STACKSIZE/sizeof(u64),
		 NN_SC_PRI);
  osStartThread(&sc->schedulerThread);

  osCreateThread(&sc->audioThread, 18, (void(*)(void *))nnScExecuteAudio,
		 (void *)sc, nnScAudioStack+NN_SC_STACKSIZE/sizeof(u64),
		 NN_SC_AUDIO_PRI);
  osStartThread(&sc->audioThread);

  osCreateThread(&sc->graphicsThread, 17,(void(*)(void*))nnScExecuteGraphics,
		 (void *)sc, nnScGraphicsStack+NN_SC_STACKSIZE/sizeof(u64),
		 NN_SC_GRAPHICS_PRI);
  osStartThread(&sc->graphicsThread);

}

/************************************************************
  nnScGetAudioMQ()   ---   Get audio message queue 
************************************************************/
OSMesgQueue *nnScGetAudioMQ(NNSched *sc)
{
  return( &sc->audioRequestMQ );
}

/************************************************************
  nnScGetGfxMQ() ---  Get audio message queue
************************************************************/
OSMesgQueue *nnScGetGfxMQ(NNSched *sc)
{
  return( &sc->graphicsRequestMQ );
}

/************************************************************
  nnScEventHandler()  -  Process system event
************************************************************/
void nnScEventHandler(NNSched *sc)
{
  OSMesg msg = (OSMesg)0;

  while(1) {
    /* 3/4 of event */
    osRecvMesg(&sc->retraceMQ, &msg, OS_MESG_BLOCK);
    framecont++;


    switch ( (int)msg ) {
    case VIDEO_MSG:		/* process retrace signal */
      nnScEventBroadcast( sc, &sc->retraceMsg );
#ifdef NN_SC_PERF

      if( nnsc_perf_flag  == 0){
	/* do not initialize until all graphic tasks are completed */
	nnsc_perf_flag++;
	
	/* enable externally derived reference to measured buffer pointer */
	nnsc_perf_ptr = &nnsc_perf[nnsc_perf_index];
	
	nnsc_perf_index++;  /* swap buffer */
	nnsc_perf_index %= NN_SC_PERF_NUM;
	
	nnsc_perf_inptr = &nnsc_perf[nnsc_perf_index];
	
	nnsc_perf_inptr->autask_cnt =  0;   
	nnsc_perf_inptr->gtask_cnt = 0;
	
	/* get retrace time */
	nnsc_perf_inptr->retrace_time = OS_CYCLES_TO_USEC(osGetTime());
	
      }
#endif /* NN_SC_PERF */

      break;
    case PRE_NMI_MSG:		/* process NMI signal */
      nnScEventBroadcast( sc, &sc->prenmiMsg );
      break;
    }
  }
}

/************************************************************
  nnScAddClient()  --- Register client  
************************************************************/
void nnScAddClient(NNSched *sc, NNScClient *c, OSMesgQueue *msgQ)
{
  OSIntMask mask;

  mask = osSetIntMask(OS_IM_NONE);
  c->msgQ = msgQ;
  c->next = sc->clientList;
  sc->clientList = c;
  osSetIntMask(mask);
}

/************************************************************
  nnScRemoveClient()  ---  Remove client
************************************************************/
void nnScRemoveClient(NNSched *sc, NNScClient *c)
{
  NNScClient *client = sc->clientList; 
  NNScClient *prev   = 0;
  OSIntMask  mask;

  mask = osSetIntMask(OS_IM_NONE);
    
  while (client != 0) {
    if (client == c) {
      if(prev)
	prev->next = c->next;
      else
	sc->clientList = c->next;
      break;
    }
    prev   = client;
    client = client->next;
  }
  osSetIntMask(mask);
}

/************************************************************
 nnScEventBroadcast() --  Transmit message to client
************************************************************/
void nnScEventBroadcast(NNSched *sc, NNScMsg *msg)
{
  NNScClient *client;
  
  /* inform necessary clients of  retrace message */
  for (client = sc->clientList; client != 0; client = client->next) {
    osSendMesg(client->msgQ, (OSMesg *)msg, OS_MESG_NOBLOCK);
  }
}

/************************************************************
  nnScExecuteAudio() --  Execute audio task
************************************************************/
void nnScExecuteAudio(NNSched *sc)
{
  OSMesg msg = (OSMesg)0;
  NNScTask *task = (NNScTask *)0;
  NNScTask *gfxTask = (NNScTask *)0;
  u32 yieldFlag = 0;

#ifdef NN_SC_PERF
  u32 task_cnt;

#endif /* NN_SC_PERF */

  while(1) {

    /* wait for audio execution request */
    osRecvMesg(&sc->audioRequestMQ, (OSMesg *)&task, OS_MESG_BLOCK);
    osWritebackDCacheAll();	/* flash cache */


    /* Inspect current RSP stack */
    yieldFlag = 0;
    gfxTask = sc->curGraphicsTask;
    if( gfxTask ) {

      /* wait for graphic task to end (yield) */
      osSpTaskYield();		/* task yield */
      osRecvMesg(&sc->rspMQ, &msg, OS_MESG_BLOCK);

      /* verify that task has actually yielded */
      if (osSpTaskYielded(&gfxTask->list)){
	yieldFlag =1;
      } else {
	yieldFlag =2;
      }
    }

#ifdef NN_SC_PERF
    if(nnsc_perf_inptr->autask_cnt < NN_SC_AUTASK_NUM){
      task_cnt = nnsc_perf_inptr->autask_cnt;
      nnsc_perf_inptr->autask_stime[task_cnt] =
	OS_CYCLES_TO_USEC(osGetTime()) - nnsc_perf_inptr->retrace_time;
    }
#endif /* NN_SC_PERF */


    sc->curAudioTask = task;
    osSpTaskStart(&task->list);        /* execute task */

    /* wait for end of RSP task */
    osRecvMesg(&sc->rspMQ, &msg, OS_MESG_BLOCK);
    sc->curAudioTask = (NNScTask *)0;

#ifdef NN_SC_PERF
    if(nnsc_perf_inptr->autask_cnt < NN_SC_AUTASK_NUM){
      nnsc_perf_inptr->autask_etime[task_cnt] =
	OS_CYCLES_TO_USEC(osGetTime()) - nnsc_perf_inptr->retrace_time;
      nnsc_perf_inptr->autask_cnt++;
    }
#endif /* NN_SC_PERF */

    if( sc->graphicsTaskSuspended )
      osSendMesg( &sc->waitMQ, &msg, OS_MESG_BLOCK );

    /* restart graphic task that yielded */
    if( yieldFlag == 1 ) {
      osSpTaskStart(&gfxTask->list);    
    } else if ( yieldFlag == 2 ) {
      osSendMesg(&sc->rspMQ, &msg, OS_MESG_BLOCK);
    }

    /* inform thread started by audio task that task has ended */
    osSendMesg(task->msgQ, task->msg, OS_MESG_BLOCK);
  }
}

/************************************************************
 * nnScExecuteGraphics() --  Execute graphic task
************************************************************/
void nnScExecuteGraphics(NNSched *sc)
{
  OSMesg msg = (OSMesg)0;
  NNScTask *task;
#ifdef NN_SC_PERF
  u32 task_cnt;

#endif /* NN_SC_PERF */

  while(1) {

    /* wait for graphic task execution request */
    osRecvMesg(&sc->graphicsRequestMQ, (OSMesg *)&task, OS_MESG_BLOCK);

    /* wait for frame buffer to become available for use */
    nnScWaitTaskReady(sc, task);

    if( sc->curAudioTask ) {

      sc->graphicsTaskSuspended = task;
      osRecvMesg( &sc->waitMQ, &msg, OS_MESG_BLOCK );
      sc->graphicsTaskSuspended = (NNScTask *)0;
    }

#ifdef NN_SC_PERF
    if(nnsc_perf_inptr->gtask_cnt < NN_SC_GTASK_NUM){
      task_cnt = nnsc_perf_inptr->gtask_cnt;
      nnsc_perf_inptr->gtask_stime[task_cnt] =
	OS_CYCLES_TO_USEC(osGetTime()) - nnsc_perf_inptr->retrace_time;
    }
#endif /* NN_SC_PERF */    

    sc->curGraphicsTask = task;
    osSpTaskStart(&task->list);        /* execute task */

    /* wait for end of RSP task */
    osRecvMesg(&sc->rspMQ, &msg, OS_MESG_BLOCK);
    sc->curGraphicsTask = (NNScTask *)0;
#ifdef NN_SC_PERF
    if(nnsc_perf_inptr->gtask_cnt < NN_SC_GTASK_NUM){
      nnsc_perf_inptr->rsp_etime[task_cnt] =
	OS_CYCLES_TO_USEC(osGetTime()) - nnsc_perf_inptr->retrace_time;
    }
#endif /* NN_SC_PERF */

    /* wait for end of  RDP task */
    osRecvMesg(&sc->rdpMQ, &msg, OS_MESG_BLOCK);

#ifdef NN_SC_PERF
    if(nnsc_perf_inptr->gtask_cnt < NN_SC_GTASK_NUM){
      nnsc_perf_inptr->rdp_etime[task_cnt] =
	OS_CYCLES_TO_USEC(osGetTime()) - nnsc_perf_inptr->retrace_time;
      nnsc_perf_inptr->gtask_cnt++;
    }
#endif /* NN_SC_PERF */

    /* invalidate video blackout first time only */
    if (sc->firstTime) {
      osViBlack(FALSE);
      sc->firstTime = 0;
    }

    /* specify next frame buffer to display */
    if ( task->flags & NN_SC_SWAPBUFFER ){
      osViSwapBuffer(task->framebuffer);
#ifdef NN_SC_PERF
      nnsc_perf_flag = 0;
#endif /* NN_SC_PERF */
    }
    /* inform thread started by graphic task that task has ended */
    osSendMesg(task->msgQ, task->msg, OS_MESG_BLOCK);
  }
}

/************************************************************
  nnScWaitTaskReady() - Wait for frame buffer to become available for use 
************************************************************/
void nnScWaitTaskReady(NNSched *sc, NNScTask *task)
{
  OSMesg msg = (OSMesg)0;
  NNScClient client;
  void *fb = task->framebuffer;

  /* wait for next retrace if frame buffer is not empty */
  while( osViGetCurrentFramebuffer() == fb || osViGetNextFramebuffer() == 
fb ) {
    nnScAddClient( sc, &client, &sc->waitMQ );  
    osRecvMesg( &sc->waitMQ, &msg, OS_MESG_BLOCK );
    nnScRemoveClient( sc, &client );
  }
}


