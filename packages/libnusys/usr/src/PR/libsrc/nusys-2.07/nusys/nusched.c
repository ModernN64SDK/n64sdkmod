/*======================================================================*/
/*		NuSYS							*/
/*		nusched.c						*/
/*									*/
/*		Copyright (C) 1997, NINTENDO Co,Ltd.			*/
/*======================================================================*/  
/*	1997/01/23							*/
/*		Added the flag OS_TASK_LOADABLE to support		*/
/* 		gSPLoadUcode, which was added to F3DEX Version 1.20. 	*/
/*	1997/02/03							*/
/*    		Fixed problem that caused a hang if the values from the	*/
/*		 performance check were greater than			*/
/*		NN_SC_GTASK_NUM or NN_SC_AUTASK_NUM.			*/
/*	1997/07/15							*/
/*		Changed to use nusched as the NuSYS kernel.		*/
/*		Added msgType as a nuScAddClient parameter,		*/
/*		and improved to allow selection of sent message.	*/
/*		Changed the NuSched structure to an internal variable.	*/
/*	1997/09/28							*/
/*		Set the critical section of the thread.			*/
/*      1997/11/28							*/
/*	 	Added a flag that does not wait for the RDP to finish, 	*/
/*		to support Z-SORT microcode.				*/
/*	1997/12/13							*/
/*		Added an internal counter for DP.			*/
/*	1998/12/16							*/
/*		Fixed so that if a PRENMI message is received before    */
/*		client registration, a PRENMI message is dispatched     */
/*		when the client is registered.				*/
/*	1998/12/21							*/
/*		Improved so that a flag is set 2 frames before a PRENMI */
/*		reset, and subsequent new tasks are not started.	*/
/*		Also improved so that subsequently for each frame the	*/
/*		Y-scale is set to 1.0 and osAfterPreNMI() is called. 	*/
/*	1999/05/30							*/
/*		Made change so the user can set marks in the    	*/
/*		performance measurement part.					*/
/*======================================================================*/
/* $Id: nusched.c,v 1.14 1999/06/10 04:44:03 ohki Exp $		*/
/*======================================================================*/

#include <nusys.h>

/*----------------------------------------------------------------------*/
/*	DEFINE								*/
/*----------------------------------------------------------------------*/
#define TASK_YIELD		1	/* GFX TASK YILED FLAG */
#define TASK_YIELDED		2	/* GFX TASK YILEDED FLAG */

#define VIDEO_MSG		666	/* Retrace message */
#define RSP_DONE_MSG		667	/* RSP task finished */
#define RDP_DONE_MSG		668	/* RDP rendering finished*/
#define PRE_NMI_MSG    	 	669	/* NMI message */
#define BEFORE_RESET_MSG 	700	/* NMI message */

/*----------------------------------------------------------------------*/
/*	internal function						*/
/*----------------------------------------------------------------------*/
static void nuScEventHandler(void);
static void nuScEventBroadcast(NUScMsg *msg);
static void nuScExecuteAudio(void);
static void nuScExecuteGraphics(void);
static void nuScWaitTaskReady(NUScTask *task);

/*----------------------------------------------------------------------*/
/*	variable							*/
/*----------------------------------------------------------------------*/
static u64	nuScStack[NU_SC_STACK_SIZE/sizeof(u64)];	/* event thread stack	*/
static u64	nuScAudioStack[NU_SC_STACK_SIZE/sizeof(u64)];/* audio thread stack */
static u64	nuScGraphicsStack[NU_SC_STACK_SIZE/sizeof(u64)];/*gfx thread stack */

NUSched		nusched;		/* Scheduler structure  	*/
NUScPreNMIFunc	nuScPreNMIFunc = NULL;	/* PRE NMI callback function ptr */

u8		nuVersion[] = "NuSystem"NU_VERSION; /* Please do not delete this */
u32		nuScRetraceCounter = (u32)nuVersion;/* Retrace counter */
						/* Dummy is initialized */
u8		nuScPreNMIFlag;


#ifdef NU_DEBUG
/*------------------------------*/
/*	Performance		*/
/*------------------------------*/
NUDebTaskPerf		nuDebTaskPerf[NU_DEB_PERF_BUF_NUM];
NUDebTaskPerf*		nuDebTaskPerfPtr;
u32			nuDebTaskPerfInterval = 1;
volatile u32		nuDebTaskPerfCnt = 0;
volatile u32		nuDebTaskPerfEnd = NU_DEB_PERF_STOP;

static NUDebTaskPerf*	debTaskPerfPtr;
static u32		debFrameSwapCnt;
#endif /* NU_DEBUG */

/*----------------------------------------------------------------------*/
/* nuScCreateScheduler() -- Creates the Scheduler				*/
/*									*/
/* IN:	videoMode	Video mode setting				*/
/* 	numFields	Retrace cycles (1=60 frame/sec, 2=30 frame/sec...)	*/
/* RET:	Nothing								*/
/*----------------------------------------------------------------------*/
void nuScCreateScheduler(u8 videoMode, u8 numFields)
{

    /*  Initialize variables  */
    nusched.curGraphicsTask = NULL;
    nusched.curAudioTask    = NULL;
    nusched.graphicsTaskSuspended = NULL;
    nusched.clientList      = NULL;
    nusched.retraceMsg      = NU_SC_RETRACE_MSG;
    nusched.prenmiMsg       = NU_SC_PRENMI_MSG;
    nusched.retraceCount    = numFields;
    nusched.frameBufferNum  = 2;
    
    /* Distinguish between NTSC and PAL and set frame rate accordingly.	*/
    if(osTvType == OS_TV_PAL){
	nusched.frameRate = 50;
    } else {
	nusched.frameRate = 60;
    }
    nuScPreNMIFlag = NU_SC_PRENMI_YET;
  
#ifdef	NU_DEBUG
    debTaskPerfPtr = &nuDebTaskPerf[0];
    debTaskPerfPtr->retraceTime = 0;
    debTaskPerfPtr->auTaskCnt = 0;
    debTaskPerfPtr->gfxTaskCnt = 0;
    nuDebTaskPerfPtr = debTaskPerfPtr;
    osDpSetStatus(DPC_CLR_TMEM_CTR | DPC_CLR_PIPE_CTR | DPC_CLR_CMD_CTR | DPC_CLR_CLOCK_CTR);
#endif /* NU_DEBUG */
    
    /* Create the message queue. */
    osCreateMesgQueue(&nusched.retraceMQ, nusched.retraceMsgBuf,
		      NU_SC_MAX_MESGS);
    osCreateMesgQueue(&nusched.rspMQ, nusched.rspMsgBuf, NU_SC_MAX_MESGS);
    osCreateMesgQueue(&nusched.rdpMQ, nusched.rdpMsgBuf, NU_SC_MAX_MESGS);
    osCreateMesgQueue(&nusched.graphicsRequestMQ, nusched.graphicsRequestBuf,
		      NU_SC_MAX_MESGS);
    
    osCreateMesgQueue(&nusched.audioRequestMQ, nusched.audioRequestBuf,
		      NU_SC_MAX_MESGS);
    osCreateMesgQueue(&nusched.waitMQ, nusched.waitMsgBuf, NU_SC_MAX_MESGS);
    
    /* Set the video mode. */
    osCreateViManager(OS_PRIORITY_VIMGR);
    osViSetMode(&osViModeTable[videoMode]);
    osViBlack(TRUE);

    /* Register the event handler. */
    osViSetEvent(&nusched.retraceMQ, (OSMesg)VIDEO_MSG, numFields);    
    osSetEventMesg(OS_EVENT_SP    , &nusched.rspMQ,     (OSMesg)RSP_DONE_MSG);
    osSetEventMesg(OS_EVENT_DP    , &nusched.rdpMQ,     (OSMesg)RDP_DONE_MSG);
    osSetEventMesg(OS_EVENT_PRENMI, &nusched.retraceMQ, (OSMesg)PRE_NMI_MSG);


    /* Start the Scheduler thread. */
    osCreateThread(&nusched.schedulerThread, 19,
		   (void(*)(void*))nuScEventHandler,
		   (void *)&nusched, nuScStack+NU_SC_STACK_SIZE/sizeof(u64),
		   NU_SC_HANDLER_PRI);
    osStartThread(&nusched.schedulerThread);
    
    osCreateThread(&nusched.audioThread, 18,
		   (void(*)(void *))nuScExecuteAudio,
		   (void *)&nusched, nuScAudioStack+NU_SC_STACK_SIZE/sizeof(u64),
		   NU_SC_AUDIO_PRI);
    osStartThread(&nusched.audioThread);
    
    osCreateThread(&nusched.graphicsThread, 17,
		   (void(*)(void*))nuScExecuteGraphics,
		   (void *)&nusched, nuScGraphicsStack+NU_SC_STACK_SIZE/sizeof(u64),
		 NU_SC_GRAPHICS_PRI);
    osStartThread(&nusched.graphicsThread);
    
}

/*----------------------------------------------------------------------*/
/* nuScGetAudioMQ() -- Obtains the audio message queue		*/
/*									*/
/* IN:	Nothing								*/
/* RET:	OSMesgQueue*	 Pointer to the message queue			*/
/*----------------------------------------------------------------------*/
OSMesgQueue *nuScGetAudioMQ(void)
{
    return( &nusched.audioRequestMQ );
}

/*----------------------------------------------------------------------*/
/* nuScGetGfxMQ() -- Obtains the graphics message queue			*/
/*									*/
/* IN:				Nothing						*/
/* RET:	OSMesgQueue*		Pointer to the message queue			*/
/*----------------------------------------------------------------------*/
OSMesgQueue *nuScGetGfxMQ(void)
{
    return( &nusched.graphicsRequestMQ );
}

/*----------------------------------------------------------------------*/
/*  nuScEventHandler() -- Handles system events			*/
/*									*/
/* IN:	Nothing								*/
/* RET:	Nothing								*/
/*----------------------------------------------------------------------*/
static void nuScEventHandler(void)
{
    OSMesg	msg;
    s32		beforeResetFrame;

    nuScRetraceCounter = 0;
    
    while(1) {
	/* Wait for RETRACE and PRENMI events. */
	osRecvMesg(&nusched.retraceMQ, &msg, OS_MESG_BLOCK);
	
	/* Broadcast the event message. */
	switch ( (int)msg ) {
	case VIDEO_MSG:		/* Process the retrace signal. */
	    nuScRetraceCounter++;
#ifdef NU_DEBUG
	    if(nuDebTaskPerfEnd == NU_DEB_PERF_START){
		debTaskPerfPtr->retraceTime = OS_CYCLES_TO_USEC(osGetTime());
		nuDebTaskPerfEnd = NU_DEB_PERF_RUNNING;
		debTaskPerfPtr->auTaskCnt = 0;		
	    }
#endif /* NU_DEBUG */

	    nuScEventBroadcast(&nusched.retraceMsg );

	    /* Processing after a PRENMI event. */
	    if(nuScPreNMIFlag){
		if(beforeResetFrame){
		    beforeResetFrame--;
		} else {
		    nuScPreNMIFlag |= NU_SC_BEFORE_RESET;
		    osAfterPreNMI();
		    osViSetYScale(1.0);
		    osViBlack(TRUE);
		}
	    }
	    break;
	case PRE_NMI_MSG:		/* Process the PRENMI signal. */
	    nuScPreNMIFlag = NU_SC_PRENMI_GET;
	    nuScEventBroadcast(&nusched.prenmiMsg );
	    
	    /* PRE NMI event callback function */
	    if(nuScPreNMIFunc != NULL){
		(*nuScPreNMIFunc)();
	    }
	    
	    /**************************************************************/
	    /*** Calculates the 3rd frame before the reset 			***/
	    /*** and, for the frames after the 2nd frame before the reset,	***/
	    /*** sets the Y-scale is to 1.0 and calls osAfterPreNMI. 		***/
	    /*** The 3rd frame before the reset is used because it is in	***/
	    /*** the next retrace that osViSetYScale(1.0) becomes		***/
	    /*** valid.							***/
	    /***							***/
	    /**************************************************************/
	    beforeResetFrame = (nusched.frameRate / 2) / nusched.retraceCount - 3;
	    break;
	default:
	    break;
	}
    }
}

/*----------------------------------------------------------------------*/
/* nuScAddClient() -- Registers a client				*/
/*									*/
/* IN:	*client		Pointer the the structure for the client to be registered */
/* 	*msgQ		Queue for receiving messages				*/
/*	msgType	Type of message	RETRACE = NU_SC_RETRACE_MSG	*/
/*				 	PRENMI  = NU_SC_PRENMI_MSG	*/
/* RET:	Nothing								*/
/*----------------------------------------------------------------------*/
void nuScAddClient(NUScClient* client, OSMesgQueue* msgQ, NUScMsg msgType)
{
    OSIntMask	mask;
    

    mask = osSetIntMask(OS_IM_NONE);

    client->msgQ = msgQ;
    client->next = nusched.clientList;
    client->msgType = msgType;
    nusched.clientList = client;
    
    /* Dispatch a message if a PRENMI message has already been received. */
    if((msgType & NU_SC_PRENMI_MSG) && nuScPreNMIFlag){
	osSendMesg(msgQ, (OSMesg*)&nusched.prenmiMsg, OS_MESG_NOBLOCK);
    }
    
    osSetIntMask(mask);

}

/*----------------------------------------------------------------------*/
/* nuScResetClientMesgType() -- Changes the client message		*/
/*									*/
/* IN:	*client		Pointer to the structure for the client to be registered */
/*	msgType	Type of message	RETRACE = NU_SC_RETRACE_MSG	*/
/*				 	PRENMI  = NU_SC_PRENMI_MSG	*/
/* RET:	Nothing								*/
/*----------------------------------------------------------------------*/
void nuScResetClientMesgType(NUScClient* client, NUScMsg msgType)
{
    OSIntMask	mask;
    mask = osSetIntMask(OS_IM_NONE);
    client->msgType = msgType;
    
    osSetIntMask(mask);
}

/*----------------------------------------------------------------------*/
/* nuScRemoveClient() -- Removes a client				*/
/*									*/
/* IN:		c		Structure for the client to be removed	*/
/* RET:		Nothing							*/
/*----------------------------------------------------------------------*/
void nuScRemoveClient(NUScClient *c)
{
    NUScClient*	client;
    NUScClient*	prev;
    OSIntMask	mask;

    mask = osSetIntMask(OS_IM_NONE);
    client = nusched.clientList;
    prev = 0;
    while(client != 0){
	if(client == c){
	    if(prev){
		prev->next = c->next;
	    } else {
		nusched.clientList = c->next;
	    }
	    break;
	}
	prev   = client;
	client = client->next;
    }
    osSetIntMask(mask);
}

/*----------------------------------------------------------------------*/
/* nuScEventBroadcast() -- Transfer message to a client		*/
/*									*/
/* IN:	*msg		Message to be transferred.					*/
/*----------------------------------------------------------------------*/
static void nuScEventBroadcast(NUScMsg *msg)
{
    NUScClient *client;
  
    /* Notify clients that require retrace messages. */
    for(client = nusched.clientList; client != 0; client = client->next){
	
	/* Check msgType for registered clients and notify. */
	if(client->msgType & *msg){
	    osSendMesg(client->msgQ, (OSMesg *)msg, OS_MESG_NOBLOCK);
	}
    }
}

/*----------------------------------------------------------------------*/
/*  nuScExecuteAudio() -- Executes an audio task				*/
/*									*/
/*	IN:	Nothing							*/
/*	RET:	Nothing							*/
/*----------------------------------------------------------------------*/
static void nuScExecuteAudio(void)
{

    NUScTask*	gfxTask;
    NUScTask*	audioTask;
    OSMesg 	msg;
    u32		yieldFlag;
#ifdef NU_DEBUG
    OSIntMask	mask;
#endif /* NU_DEBUG */
    
    while(1) {
	/* Wait for request for audio task execution. */
	osRecvMesg(&nusched.audioRequestMQ, (OSMesg *)&audioTask, OS_MESG_BLOCK);
	/* Do not create task after 0.5 sec - 2 frames. */
	/* Do notify of end of task, however. */
	if(nuScPreNMIFlag & NU_SC_BEFORE_RESET){
	    /* Notify the thread that started the audio task that the task has finished. */
	    osSendMesg(audioTask->msgQ, audioTask->msg, OS_MESG_BLOCK);
	    continue;
	}
	
	osWritebackDCacheAll();	/* Flash the cache. */
	
	/* Check current RSP status. */
	yieldFlag = 0;
	gfxTask = nusched.curGraphicsTask;
	
	/* If a graphics task is being executed, have it yield. */
	if( gfxTask ) {
	    
	    /* Wait for completion (yield) of the graphics task. */
	    osSpTaskYield();		/* Task yield */
	    osRecvMesg(&nusched.rspMQ, &msg, OS_MESG_BLOCK);
	    
	    /* Check whether the task actually has yielded.*/
	    if (osSpTaskYielded(&gfxTask->list)){
		
		/* Yielded */
		yieldFlag = TASK_YIELD;
	    } else {
		
		/* Task finishes with yield. */
		yieldFlag = TASK_YIELDED;
	    }
	}
#ifdef NU_DEBUG
	mask = osSetIntMask(OS_IM_NONE);
	if(debTaskPerfPtr->auTaskCnt < NU_DEB_PERF_AUTASK_CNT){
	    debTaskPerfPtr->auTaskTime[debTaskPerfPtr->auTaskCnt].rspStart =
		OS_CYCLES_TO_USEC(osGetTime());
	}
	osSetIntMask(mask);
#endif /* NU_DEBUG */
	
	/* Execute audio task. */
	nusched.curAudioTask = audioTask;
	osSpTaskStart(&audioTask->list);
	
	/* Wait for end of the RSP task. */
	osRecvMesg(&nusched.rspMQ, &msg, OS_MESG_BLOCK);
	nusched.curAudioTask = (NUScTask *)NULL;

#ifdef NU_DEBUG
	mask = osSetIntMask(OS_IM_NONE);
	if(debTaskPerfPtr->auTaskCnt < NU_DEB_PERF_AUTASK_CNT){
	   debTaskPerfPtr->auTaskTime[debTaskPerfPtr->auTaskCnt].rspEnd =
	       OS_CYCLES_TO_USEC(osGetTime());
	   debTaskPerfPtr->auTaskCnt++;
	}
	osSetIntMask(mask);
#endif /* NU_DEBUG */

	/* Check whether a graphics task is waiting to be executed.	*/
	/* If so, send message.			*/
	if( nusched.graphicsTaskSuspended )
	    osSendMesg( &nusched.waitMQ, &msg, OS_MESG_BLOCK );
	
	/* Resume the yielding graphics task. */
	if( yieldFlag == TASK_YIELD ) {
	    osSpTaskStart(&gfxTask->list);    
	} else if ( yieldFlag == TASK_YIELDED ) {
	    /* The graphics task finishes at the same time as the yield,	*/
	    /*	so the RSPTask completion message is the same as that for the yield.  */
	    /* Send the message to the graphics task thread. */
	    osSendMesg(&nusched.rspMQ, &msg, OS_MESG_BLOCK);
	}

	/* Notify the thread that started the audio task that the task has finished. */
	osSendMesg(audioTask->msgQ, audioTask->msg, OS_MESG_BLOCK);
    }
}

/*----------------------------------------------------------------------*/
/*  nuScExecuteGrapchics() -- Executes a graphics task			*/
/*									*/
/*	IN:	Nothing							*/
/*	RET:	Nothing							*/
/*----------------------------------------------------------------------*/
static void nuScExecuteGraphics(void)
{
    OSMesg	msg;
    NUScTask*	gfxTask;
    OSIntMask	mask;
    
    while(1) {

	/* Wait for request for graphics task execution.*/
	osRecvMesg(&nusched.graphicsRequestMQ, (OSMesg *)&gfxTask, OS_MESG_BLOCK);
	/* Do not create task after 0.5 sec - 2 frames. */
	/* Do notify of end of task, however. */
	if(nuScPreNMIFlag & NU_SC_BEFORE_RESET){
	/* Notify that thread that started the graphics task that the task has finished. */
	    osSendMesg(gfxTask->msgQ, (OSMesg*)gfxTask, OS_MESG_BLOCK);
	    continue;
	}

	/* Wait till frame buffer is available.	*/
	nuScWaitTaskReady(gfxTask);

	/* Check whether an audio task is being executed. */
	/*  If so, wait for message that execution has finished. */
	mask = osSetIntMask(OS_IM_NONE);
	if( nusched.curAudioTask ) {
	    nusched.graphicsTaskSuspended = gfxTask;
	    osSetIntMask(mask);
	    osRecvMesg( &nusched.waitMQ, &msg, OS_MESG_BLOCK );
	    mask = osSetIntMask(OS_IM_NONE);
	    nusched.graphicsTaskSuspended = (NUScTask *)NULL;
	}
	osSetIntMask(mask);

	
#ifdef NU_DEBUG
	mask = osSetIntMask(OS_IM_NONE);
	if(debTaskPerfPtr->gfxTaskCnt < NU_DEB_PERF_GFXTASK_CNT){
	    debTaskPerfPtr->gfxTaskTime[debTaskPerfPtr->gfxTaskCnt].rspStart =
		OS_CYCLES_TO_USEC(osGetTime());
	}
	osSetIntMask(mask);
#endif /* NU_DEBUG */
	    
	/* Execute the graphics task.  */
	mask = osSetIntMask(OS_IM_NONE);
	nusched.curGraphicsTask = gfxTask;
	osSetIntMask(mask);
	
	osSpTaskStart(&gfxTask->list);        /* Execute task.*/

	/* Wait for end of RSP task.*/
	osRecvMesg(&nusched.rspMQ, &msg, OS_MESG_BLOCK);

	mask = osSetIntMask(OS_IM_NONE);
	nusched.curGraphicsTask = (NUScTask *)NULL;
	osSetIntMask(mask);

#ifdef NU_DEBUG
	mask = osSetIntMask(OS_IM_NONE);
	if(debTaskPerfPtr->gfxTaskCnt < NU_DEB_PERF_GFXTASK_CNT){
	    debTaskPerfPtr->gfxTaskTime[debTaskPerfPtr->gfxTaskCnt].rspEnd =
		OS_CYCLES_TO_USEC(osGetTime());
	}
	osSetIntMask(mask);
#endif /* NU_DEBUG */

	/* Check NU_SC_NORDP flag to determine whether to wait for RDP finish. */
	if(!(gfxTask->flags & NU_SC_NORDP)){
	    /* Wait for end of RDPTask. */
	    osRecvMesg(&nusched.rdpMQ, &msg, OS_MESG_BLOCK);
	}

#ifdef NU_DEBUG
	mask = osSetIntMask(OS_IM_NONE);
	if(debTaskPerfPtr->gfxTaskCnt < NU_DEB_PERF_GFXTASK_CNT){
	    if(gfxTask->flags & NU_SC_NORDP){
		
	/* If the RDP is not used, set the start time so that the bar is not displayed.*/
		debTaskPerfPtr->gfxTaskTime[debTaskPerfPtr->gfxTaskCnt].rdpEnd
		    = debTaskPerfPtr->gfxTaskTime[debTaskPerfPtr->gfxTaskCnt].rspStart;
		debTaskPerfPtr->gfxTaskTime[debTaskPerfPtr->gfxTaskCnt].dpCnt[0] = 0;
	    } else {
		debTaskPerfPtr->gfxTaskTime[debTaskPerfPtr->gfxTaskCnt].rdpEnd
		    = OS_CYCLES_TO_USEC(osGetTime());
		osDpGetCounters(debTaskPerfPtr->gfxTaskTime[debTaskPerfPtr->gfxTaskCnt].dpCnt);
		osDpSetStatus(DPC_CLR_TMEM_CTR | DPC_CLR_PIPE_CTR | DPC_CLR_CMD_CTR | DPC_CLR_CLOCK_CTR);
	    }
	    debTaskPerfPtr->gfxTaskCnt++;
	}

	if(gfxTask->flags & NU_SC_SWAPBUFFER){
	    s32 cnt;	    	    
	    nuDebTaskPerfEnd = NU_DEB_PERF_START;
	    
	    debFrameSwapCnt++;
	    if(debFrameSwapCnt >= nuDebTaskPerfInterval){
		nuDebTaskPerfPtr = debTaskPerfPtr;
		nuDebTaskPerfCnt++;
		nuDebTaskPerfCnt %= NU_DEB_PERF_BUF_NUM;
		debTaskPerfPtr = &nuDebTaskPerf[nuDebTaskPerfCnt];
		debFrameSwapCnt = 0;
	    }

	    debTaskPerfPtr->retraceTime = 0;	
	    debTaskPerfPtr->gfxTaskCnt = 0;

	    for(cnt = 0 ; cnt < NU_DEB_MARKER_NUM; cnt++){
		debTaskPerfPtr->markerTime[cnt] = 0;
	    }

	}
	
	osSetIntMask(mask);
#endif /* NU_DEBUG */

	/* Notify that thread that started the graphics task that the task has finished. */
 
	osSendMesg(gfxTask->msgQ, (OSMesg*)gfxTask, OS_MESG_BLOCK);
    }
}

/*----------------------------------------------------------------------*/
/*  nuScWaitTaskReady() -- Implements a wait until a frame buffer is available*/
/*									*/
/* IN:	*task			Pointer the the graphics task structure	*/
/*----------------------------------------------------------------------*/
void nuScWaitTaskReady(NUScTask *task)
{
    NUScClient	client;
    void*	fb	= task->framebuffer;

    /* Wait unnecessary if the number of frame buffers is 1. */
    if(nusched.frameBufferNum == 1) return;
    

    /* Wait till next retrace if frame buffer is not free. */
    while( osViGetCurrentFramebuffer() == fb
	   || osViGetNextFramebuffer() == fb ){
	nuScAddClient(&client, &nusched.waitMQ , NU_SC_RETRACE_MSG);	
	osRecvMesg( &nusched.waitMQ, NULL, OS_MESG_BLOCK );
	nuScRemoveClient(&client );
    }

}

/*----------------------------------------------------------------------*/
/*	nuScSetFrameBufferNum() - Sets the number of frame buffers	*/
/*									*/
/*	Sets the number of frame buffers.  Supports a single frame buffer.	*/
/*									*/
/*	IN:	frameBufferNum		Number of frame buffers			*/
/*					1=Do not check for free frame buffers. */
/*					Value other than 1=Check.		*/
/*	RTN:	Nothing							*/
/*----------------------------------------------------------------------*/
void nuScSetFrameBufferNum(u8 frameBufferNum)
{
    nusched.frameBufferNum = frameBufferNum;
}

/*----------------------------------------------------------------------*/
/*	nuScGetFrameRate() - Obtains the frame rate			*/
/*									*/
/*	Obtains the frame rate.  The frame rate is determined by osTvType:	*/
/*	50 for PAL and 60 for non-PAL formats.				 */
/*									*/
/*	IN:	Nothing							*/
/*	RTN:	Frame rate -- PAL=50, non-PAL=60			*/
/*----------------------------------------------------------------------*/
s32 nuScGetFrameRate(void)
{
    return nusched.frameRate;
}
