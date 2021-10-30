/*============================================================================
  nnsched.c
  ============================================================================*/

#include "nnsched.h"

/* Reserve stack for thread */
u64 nnScStack[NN_SC_STACKSIZE/8];
u64 nnScAudioStack[NN_SC_STACKSIZE/8];
u64 nnScGraphicsStack[NN_SC_STACKSIZE/8];
u32 framecont = 0;

#ifdef NN_SC_PERF
static u32       nnsc_perf_index = 0;		/* location of buffer that is used internally */
static u32       nnsc_perf_flag = 0;
static NNScPerf  nnsc_perf[NN_SC_PERF_NUM];	/* measurement data */
static NNScPerf* nnsc_perf_inptr;		/* pointer that is used internally */
NNScPerf* nnsc_perf_ptr;			/* pointer when referring from outside */
#endif  /* NN_SC_PERF */

/************************************************************
  nnScCreateScheduler() - create scheduler
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
    osCreateMesgQueue(&sc->graphicsRequestMQ, sc->graphicsRequestBuf, NN_SC_MAX_MESGS);
    osCreateMesgQueue(&sc->audioRequestMQ, sc->audioRequestBuf, NN_SC_MAX_MESGS);
    osCreateMesgQueue(&sc->waitMQ, sc->waitMsgBuf, NN_SC_MAX_MESGS);

    /*set video mode */
    osCreateViManager(OS_PRIORITY_VIMGR);    
    osViSetMode(&osViModeTable[videoMode]);
    osViBlack(TRUE);

    /*register event handler*/
    osViSetEvent(&sc->retraceMQ, (OSMesg)VIDEO_MSG, numFields);    
    osSetEventMesg(OS_EVENT_SP, &sc->rspMQ, (OSMesg)RSP_DONE_MSG);
    osSetEventMesg(OS_EVENT_DP, &sc->rdpMQ, (OSMesg)RDP_DONE_MSG);
    osSetEventMesg(OS_EVENT_PRENMI, &sc->retraceMQ, (OSMesg)PRE_NMI_MSG);   


    /* activate scheduler thread */
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
  nnScGetAudioMQ() -acquire audio message queue
  ************************************************************/
OSMesgQueue *nnScGetAudioMQ(NNSched *sc)
{
    return( &sc->audioRequestMQ );
}

/************************************************************
  nnScGetGfxMQ() - acquire audio message queue
  ************************************************************/
OSMesgQueue *nnScGetGfxMQ(NNSched *sc)
{
    return( &sc->graphicsRequestMQ );
}

/************************************************************
  nnScEventHandler() - system event processing
  ************************************************************/
void nnScEventHandler(NNSched *sc)
{
    OSMesg msg = (OSMesg)0;

    while(1) {
	/* acquire event */
	osRecvMesg(&sc->retraceMQ, &msg, OS_MESG_BLOCK);
	framecont++;


	switch ( (int)msg ) {
	  case VIDEO_MSG:	/* process retrace signal */
	    nnScEventBroadcast( sc, &sc->retraceMsg );
#ifdef NN_SC_PERF

	    if( nnsc_perf_flag  == 0){
		/* make it unable to  initialize till all the graphic tasks end  */
		nnsc_perf_flag++;
	
		/* make pointer of measured buffer able to be referred from outside */
		nnsc_perf_ptr = &nnsc_perf[nnsc_perf_index];
	
		nnsc_perf_index++; /* switch buffer */
		nnsc_perf_index %= NN_SC_PERF_NUM;
	
		nnsc_perf_inptr = &nnsc_perf[nnsc_perf_index];
	
		nnsc_perf_inptr->autask_cnt =  0;   
		nnsc_perf_inptr->gtask_cnt = 0;
	
		/* acquire time for retrace */
		nnsc_perf_inptr->retrace_time = OS_CYCLES_TO_USEC(osGetTime());
	
	    }
#endif /* NN_SC_PERF */

	    break;
	  case PRE_NMI_MSG:	/*NMI signal processing */
	    nnScEventBroadcast( sc, &sc->prenmiMsg );
	    break;
	}
    }
}

/************************************************************
  nnScAddClient() - register client
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
  nnScRemoveClient() - remove client
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
  nnScEventBroadcast() - transfer message to client
  ************************************************************/
void nnScEventBroadcast(NNSched *sc, NNScMsg *msg)
{
    NNScClient *client;
  
    /*inform client who needs retrace message */
    for (client = sc->clientList; client != 0; client = client->next) {
	osSendMesg(client->msgQ, (OSMesg *)msg, OS_MESG_NOBLOCK);
    }
}

/************************************************************
  nnScExecuteAudio() - execute audio task
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
	osWritebackDCacheAll();	/* flush cache */


	/*check current RSP status*/
	yieldFlag = 0;
	gfxTask = sc->curGraphicsTask;
	if( gfxTask ) {

	    /* wait for graphic task end (yield) */
	    osSpTaskYield();	/* yield of task */
	    osRecvMesg(&sc->rspMQ, &msg, OS_MESG_BLOCK);

	    /*check if task was actually yielded  */
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
	osSpTaskStart(&task->list); /*execute task*/

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

	/* restart graphic task that is yielded */
	if( yieldFlag == 1 ) {
	    osSpTaskStart(&gfxTask->list);    
	} else if ( yieldFlag == 2 ) {
	    osSendMesg(&sc->rspMQ, &msg, OS_MESG_BLOCK);
	}

	/* inform the thread that activated audio task of end of the task */
	osSendMesg(task->msgQ, task->msg, OS_MESG_BLOCK);
    }
}

/************************************************************
 * nnScExecuteGraphics() - execute graphic task
 ************************************************************/
void nnScExecuteGraphics(NNSched *sc)
{
    OSMesg msg = (OSMesg)0;
    NNScTask *task;
#ifdef NN_SC_PERF
    u32 task_cnt;

#endif /* NN_SC_PERF */

    while(1) {

	/* wait for graphic task execution request*/
	osRecvMesg(&sc->graphicsRequestMQ, (OSMesg *)&task, OS_MESG_BLOCK);

	/* wait till frame buffer becomes available  */
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
	osSpTaskStart(&task->list); /* execute task*/

	/* wait for end of RSP task */
	osRecvMesg(&sc->rspMQ, &msg, OS_MESG_BLOCK);
	sc->curGraphicsTask = (NNScTask *)0;
#ifdef NN_SC_PERF
	if(nnsc_perf_inptr->gtask_cnt < NN_SC_GTASK_NUM){
	    nnsc_perf_inptr->rsp_etime[task_cnt] =
	      OS_CYCLES_TO_USEC(osGetTime()) - nnsc_perf_inptr->retrace_time;
	}
#endif /* NN_SC_PERF */

	/* wait for end of RDP task */
	osRecvMesg(&sc->rdpMQ, &msg, OS_MESG_BLOCK);

#ifdef NN_SC_PERF
	if(nnsc_perf_inptr->gtask_cnt < NN_SC_GTASK_NUM){
	    nnsc_perf_inptr->rdp_etime[task_cnt] =
	      OS_CYCLES_TO_USEC(osGetTime()) - nnsc_perf_inptr->retrace_time;
	    nnsc_perf_inptr->gtask_cnt++;
	}
#endif /* NN_SC_PERF */

	/* nullify black out of video for the first time only */
	if (sc->firstTime) {
	    osViBlack(FALSE);
	    sc->firstTime = 0;
	}

	/* specify frame buffer that will be displayed next */
	if ( task->flags & NN_SC_SWAPBUFFER ){
	    osViSwapBuffer(task->framebuffer);
#ifdef NN_SC_PERF
	    nnsc_perf_flag = 0;
#endif /* NN_SC_PERF */
	}
	/* inform the thread that activated graphics task of end of the task */
	osSendMesg(task->msgQ, task->msg, OS_MESG_BLOCK);
    }
}

/************************************************************
  nnScWaitTaskReady() - wait till frame buffer becomes available
  ************************************************************/
void nnScWaitTaskReady(NNSched *sc, NNScTask *task)
{
    OSMesg msg = (OSMesg)0;
    NNScClient client;
    void *fb = task->framebuffer;

    /* if no free buffer, wait for the next retrace */
    while( osViGetCurrentFramebuffer() == fb || osViGetNextFramebuffer() == 
	  fb ) {
	nnScAddClient( sc, &client, &sc->waitMQ );  
	osRecvMesg( &sc->waitMQ, &msg, OS_MESG_BLOCK );
	nnScRemoveClient( sc, &client );
    }
}
