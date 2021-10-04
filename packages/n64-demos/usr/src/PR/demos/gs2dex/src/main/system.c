/*---------------------------------------------------------------------
	Copyright (C) 1997, Nintendo.
	
	File		system.c
	Coded    by	Yoshitaka Yasumoto.	Feb  3, 1997.
	Modified by	
	Comments	
	
	$Id: system.c,v 1.3 1997/08/07 03:24:22 yasu Exp $
  ---------------------------------------------------------------------*/
#include <ultra64.h>
#include "system.h"

/*
 *  Call the main function after providing the required initialization setting of the system.
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
OSMesgQueue	rdpMessageQ;
OSMesgQueue	retraceMessageQ;

static OSMesg	piMessages[NUM_PI_MSGS];
static OSMesg	siMessageBuf;
static OSMesg	dmaMessageBuf;
static OSMesg	rspMessageBuf;
static OSMesg	rdpMessageBuf;
static OSMesg	retraceMessageBuf;

OSIoMesg	dmaIOMessageBuf;

OSContStatus	contStatus[MAXCONTROLLERS];
OSContPad	contPad[MAXCONTROLLERS];
u8		contExist;


void	mainproc(void *arg)
{
  /* Create the message queue. */
  osCreateMesgQueue(&dmaMessageQ,     &dmaMessageBuf,     1);
  osCreateMesgQueue(&rspMessageQ,     &rspMessageBuf,     1);
  osCreateMesgQueue(&rdpMessageQ,     &rdpMessageBuf,     1);
  osCreateMesgQueue(&siMessageQ,      &siMessageBuf,      1);
  osCreateMesgQueue(&retraceMessageQ, &retraceMessageBuf, 1);
  
  /* Connect the event with the message queue. */
  osSetEventMesg(OS_EVENT_SP, &rspMessageQ, NULL);
  osSetEventMesg(OS_EVENT_DP, &rdpMessageQ, NULL);
  osSetEventMesg(OS_EVENT_SI, &siMessageQ,  NULL);
  osViSetEvent(&retraceMessageQ, NULL, 1);
  
  /* Initialize the controller. */
  osContInit(&siMessageQ, &contExist, contStatus);
  
/* Call the Main function. */
Main(arg);
}

static	void	idle(void *arg)
{
/* Activate the Vi manager. */
osCreateViManager(OS_PRIORITY_VIMGR);
osViSetMode(&osViModeTable[OS_VI_NTSC_LAN1]);

  /* Switch OFF the GAMMA correction. */
  osViSetSpecialFeatures( OS_VI_GAMMA_OFF | OS_VI_GAMMA_DITHER_OFF );
  osViSetSpecialFeatures( OS_VI_DITHER_FILTER_ON );
  
  /* Activate the Pi manager. */
  osCreatePiManager((OSPri)OS_PRIORITY_PIMGR,
		    &piMessageQ, piMessages, NUM_PI_MSGS);
  
  /* Activate the Main Thread. */
  osCreateThread(&mainThread, 3, mainproc, arg,
		 mainThreadStack+STACKSIZE/sizeof(u64), 10);
  osStartThread(&mainThread);
  osSetThreadPri(0,0);

  /* Start the Idle loop. */
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
