/*---------------------------------------------------------------------
  File  : nu64sys.c

  Coded    by     Tetsuyuki Ootsuka.      July, 1998.
	
  Copyright by Nintendo, Co., Ltd. 1998.
  ---------------------------------------------------------------------*/
#include	<ultra64.h>
#include	<ramrom.h>
#include	"nu64sys.h"
#include	"thread.h"
#include        "graph.h"

/*
 *		BOOT section
 */
public	u64		bootStack[STACKSIZE/8];

/*
 *		IDLE thread
 */
private	void		idle(void *);
private	OSThread	idleThread;
private	u64		idleThreadStack[STACKSIZE/8];

/*
 *		MAIN thread
 */
extern	void		mainproc(void *);
private	OSThread	mainThread;
private	u64		mainThreadStack[STACKSIZE/8];

/*
 *		Pi message queue
 */
#define	NUM_PI_MSGS     8
private	OSMesg		PiMessages[NUM_PI_MSGS];
private	OSMesgQueue	PiMessageQ;

/*
 *		Si message queue
 */
public  OSMesgQueue	siMessageQ;
private OSMesg		siMessageBuf;


/*
 *		Other message queues
 */
public	OSMesgQueue	dmaMessageQ,	rdpMessageQ;
public	OSMesgQueue	rspMessageQ,	retraceMessageQ;
public	OSMesgQueue	rstMessageQ;
private	OSMesg		dmaMessageBuf,	rdpMessageBuf;
private	OSMesg		rspMessageBuf,	retraceMessageBuf;
private	OSMesg		rstMessageBuf;


/*---------------------------------------------------------------------*
 *	IDLE THREAD (Pri.= 0)
 *---------------------------------------------------------------------*/
private	void	idle(void *arg)
{
  
  /*
   *		Initialize Vi manager
   */

  osCreateViManager(OS_PRIORITY_VIMGR);

#if	SCREEN_LOW
  osViSetMode(&osViModeTable[OS_VI_NTSC_LPN1]);
#else
  osViSetMode(&osViModeTable[OS_VI_NTSC_HPF1]);
#endif

  osViSwapBuffer(cfb[0]);
  
  /*
   *		Initialize Pi manager
   */

  osCreatePiManager((OSPri)OS_PRIORITY_PIMGR,
		    &PiMessageQ, PiMessages, NUM_PI_MSGS);
  
  /*
   *		Setup message queue
   */

  osCreateMesgQueue(&dmaMessageQ, &dmaMessageBuf, 1);		/* DMA */

  osCreateMesgQueue(&rspMessageQ, &rspMessageBuf, 1);
  osSetEventMesg(OS_EVENT_SP , &rspMessageQ, NULL);		/* RSP */

  osCreateMesgQueue(&rdpMessageQ, &rdpMessageBuf, 1);
  osSetEventMesg(OS_EVENT_DP, &rdpMessageQ, NULL);		/* RDP */
  
  osCreateMesgQueue(&retraceMessageQ, &retraceMessageBuf, 1);
  osViSetEvent(&retraceMessageQ, NULL, 1);			/* retrace */
  
  osCreateMesgQueue(&siMessageQ, &siMessageBuf, 1);
  osSetEventMesg(OS_EVENT_SI, &siMessageQ, (OSMesg)1);	/* SI */
    
  osCreateMesgQueue(&rstMessageQ, &rstMessageBuf, 1);
  osSetEventMesg(OS_EVENT_PRENMI, &rstMessageQ, NULL);

  /*
   *		Create & start MAINPROC thread
   */  

  osCreateThread(&mainThread, TID_MAINPROC, mainproc, (void *)0,
		 (void *)(mainThreadStack+STACKSIZE/8), 10);
  
#ifndef	DEBUG
  osStartThread(&mainThread);
#endif
  
  /*
   *		Now idling
   */
  osSetThreadPri(0, 0);
/*  osPause(); */
    while(1){
    }
}

/*---------------------------------------------------------------------*
 *	BOOT PART
 *---------------------------------------------------------------------*/
public void boot(void)
{  
  /*
   *		Initialize OS
   */
  osInitialize();
  
  /*
   *		Create idle thread & start it
   */
  osCreateThread(&idleThread, TID_IDLE, idle, (void*)0,
		 idleThreadStack+STACKSIZE/8, 10);
  osStartThread(&idleThread);
}

