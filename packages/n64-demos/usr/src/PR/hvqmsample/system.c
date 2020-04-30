/* 
 *  N64-HVQM2 library  Sample program
 * 
 *  FILE : system.c (boot program/system utility)
 * 
 *  Copyright (C) 1998,1999 NINTENDO Co.,Ltd.
 * 
 */

/* 1998-12-15 */

#include <ultra64.h>
#include "system.h"

/***********************************************************************
 * Boot code stack
 ***********************************************************************/
u64 bootStack[STACKSIZE/8];

/***********************************************************************
 * Idle thread
 ***********************************************************************/
static OSThread  idleThread;
static u64       idleThreadStack[STACKSIZE/8];

/***********************************************************************
 * Main thread
 ***********************************************************************/
static OSThread  mainThread;
static u64       mainThreadStack[STACKSIZE/8];

/***********************************************************************
 * PI command message queue
 ***********************************************************************/
static OSMesgQueue  PiMessageQ;
static OSMesg       PiMessages[PI_COMMAND_QUEUE_SIZE];

/***********************************************************************
 *
 * mainproc - Main thread procedure
 *
 ***********************************************************************/
static void
mainproc(void *arg)
{
  /* To main function */
  Main(arg);

  /* To idle state */
  osSetThreadPri(0,0);
  for ( ; ; ) ;
  /* NOT REACHED */
}

/***********************************************************************
 *
 * idle - Idle thread procedure
 *
 ***********************************************************************/
static void
idle(void *arg)
{
  /* Start PI manager */
  osCreatePiManager( (OSPri)OS_PRIORITY_PIMGR, 
		    &PiMessageQ, PiMessages, PI_COMMAND_QUEUE_SIZE );

  /*
   * Start VI manager, initialize video mode
   */
  osCreateViManager( (OSPri)OS_PRIORITY_VIMGR );
  osViSetMode( &osViModeTable[VIMODE] );
  osViSetXScale( 1.0f );
  osViSetYScale( 1.0f );
  osViSetSpecialFeatures( VIFEAT );
  osViSetSpecialFeatures(OS_VI_GAMMA_OFF);

  /* Start main thread */
  osCreateThread( &mainThread, MAIN_THREAD_ID, mainproc, NULL, 
		 mainThreadStack + STACKSIZE/8, MAIN_PRIORITY );
  osStartThread( &mainThread );

  /* Become idle */
  osSetThreadPri( 0, 0 );
  for ( ; ; ) ;
  /* NOT REACHED */
}

/***********************************************************************
 *
 * boot - Boot code
 *
 ***********************************************************************/
void
boot()
{
  osInitialize();
  osCreateThread( &idleThread, IDLE_THREAD_ID, idle, NULL, 
		 idleThreadStack + STACKSIZE/8, IDLE_PRIORITY );
  osStartThread( &idleThread );
  /* NOT REACHED */
}

/***********************************************************************
 *
 * void romcpy(void *dest, void *src, u32 len, s32 pri, OSIoMesg *mb, 
 *             OSMesgQueue *mq)
 *
 * Arguments
 *     dest      DRAM address
 *     src       PI device (ROM) address
 *     len       Transfer length (bytes)
 *     pri       Priority of the transfer request
 *     mb        I/O message block request
 *     mq        Message queue receiving notification of end of DMA
 *
 * Explanation
 *     DMA transfers "len" bytes from ROM address "SRC" to DRAM 
 *  address "dest" and returns after waiting for end of DMA. The
 *  data cache of the transfer destination in DRAM is invalidated 
 *  ahead of time.
 *
 *     The parameters have the same meaning as for osPiStartDma() 
 *
 ***********************************************************************/
void
romcpy(void *dest, void *src, u32 len, s32 pri, OSIoMesg *mb, OSMesgQueue *mq)
{
  osInvalDCache( dest, (s32)len );
  while ( osPiStartDma( mb, pri, OS_READ, (u32)src, dest, len, mq ) == -1 ) {}
  osRecvMesg( mq, (OSMesg *)NULL, OS_MESG_BLOCK );
}

/* end */
