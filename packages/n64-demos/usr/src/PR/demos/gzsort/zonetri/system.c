/*---------------------------------------------------------------------*
	Copyright (C) 1997, Nintendo.
	
	File		system.c
	Coded    by	Yoshitaka Yasumoto.	Sep 12, 1997.
	Modified by	
	
	$Id: system.c,v 1.1 1997/10/13 08:59:25 yasu Exp $
 *---------------------------------------------------------------------*/
#include <ultra64.h>
#include "system.h"

/*
 *  Call Main Function after the initial setting for the system
 */
u64	bootStack[STACKSIZE/sizeof(u64)];

extern	void	Main(void *);

static	OSThread	idleThread;
static	u64		idleThreadStack[STACKSIZE/sizeof(u64)];
static	OSThread	mainThread;
static	u64		mainThreadStack[STACKSIZE/sizeof(u64)];

OSMesgQueue	piMessageQ;
OSMesgQueue	siMessageQ;
OSMesgQueue	dmaMessageQ;
OSMesgQueue	rspMessageQ;
OSMesgQueue	rspBrkMessageQ;
OSMesgQueue	rdpMessageQ;
OSMesgQueue	retraceMessageQ;

static OSMesg	piMessages[NUM_PI_MSGS];
static OSMesg	siMessageBuf;
static OSMesg	dmaMessageBuf;
static OSMesg	rspMessageBuf;
static OSMesg	rspBrkMessageBuf;
static OSMesg	rdpMessageBuf;
static OSMesg	retraceMessageBuf;

OSIoMesg	dmaIOMessageBuf;

OSContStatus	contStatus[MAXCONTROLLERS];
OSContPad	contPad[MAXCONTROLLERS];
u8		contExist;


void	mainproc(void *arg)
{
  /* Create Message Queue */
  osCreateMesgQueue(&dmaMessageQ,     &dmaMessageBuf,       1);
  osCreateMesgQueue(&rspMessageQ,     &rspMessageBuf,       1);
  osCreateMesgQueue(&rspBrkMessageQ,     &rspBrkMessageBuf, 1);
  osCreateMesgQueue(&rdpMessageQ,     &rdpMessageBuf,       1);
  osCreateMesgQueue(&siMessageQ,      &siMessageBuf,        1);
  osCreateMesgQueue(&retraceMessageQ, &retraceMessageBuf,   1);
  
  /* Connect Event to Message Queue */
  osSetEventMesg(OS_EVENT_SP, &rspMessageQ, NULL);
  osSetEventMesg(OS_EVENT_SP_BREAK, &rspBrkMessageQ, NULL);
  osSetEventMesg(OS_EVENT_DP, &rdpMessageQ, NULL);
  osSetEventMesg(OS_EVENT_SI, &siMessageQ,  NULL);
  osViSetEvent(&retraceMessageQ, NULL, 1);
  
  /* Initialize Controller */
  osContInit(&siMessageQ, &contExist, contStatus);
  
  /* Call Main Function */
  Main(arg);
}

static	void	idle(void *arg)
{
  /* Activate Vi Manager */
  osCreateViManager(OS_PRIORITY_VIMGR);
  osViSetMode(&osViModeTable[OS_VI_NTSC_LPN1]);

  /* Turn OFF GAMMA Correction */
  osViSetSpecialFeatures( OS_VI_GAMMA_OFF | OS_VI_GAMMA_DITHER_OFF |
			  OS_VI_DIVOT_ON  | OS_VI_DITHER_FILTER_ON );
  
  /* Activate Pi Manager */
  osCreatePiManager((OSPri)OS_PRIORITY_PIMGR,
		    &piMessageQ, piMessages, NUM_PI_MSGS);
  
  /* Activate Main Thread */
  osCreateThread(&mainThread, 3, mainproc, arg,
		 mainThreadStack+STACKSIZE/sizeof(u64), 10);
  osStartThread(&mainThread);
  osSetThreadPri(0,0);

  /* Start Idle Loop */
  while (1);
}

void	boot(void)
{
    osInitialize();
    osCreateThread(&idleThread, 1, idle, (void *)0,
		   idleThreadStack+STACKSIZE/sizeof(u64), 10);
    osStartThread(&idleThread);
}

/*======== End of system.c ========*/

