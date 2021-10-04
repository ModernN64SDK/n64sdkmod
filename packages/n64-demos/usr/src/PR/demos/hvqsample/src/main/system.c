/* 
 *  N64-HVQ2 library  Sample program
 * 
 *  FILE : system.c
 * 
 *  Copyright (C) 1998,1999 NINTENDO Co.,Ltd.
 * 
 */

/* 1999-02-12 */

#include <ultra64.h>
#include "system.h"

u64 bootStack[STACKSIZE/8];

static OSThread  idleThread;
static u64       idleThreadStack[STACKSIZE/8];
static OSThread  mainThread;
static u64       mainThreadStack[STACKSIZE/8];

OSMesgQueue   PiMessageQ;
static OSMesg PiMessages[DMA_QUEUE_SIZE];

OSMesgQueue   dmaMessageQ;
static OSMesg dmaMessageBuf;

#if USE_RSP
OSMesgQueue   rspMessageQ;
static OSMesg rspMessageBuf;
#endif


/*
 * Main
 */
static void
mainproc(void *arg)
{
  /* Create message queue */
  osCreateMesgQueue(&dmaMessageQ, &dmaMessageBuf, 1);

#if USE_RSP
  /*
   * Connect SP event and message queue   */
  osCreateMesgQueue(&rspMessageQ, &rspMessageBuf, 1);
  osSetEventMesg(OS_EVENT_SP, &rspMessageQ, NULL);
#endif

  /* To main function */
  Main(arg);

  /* Become idle */
  osSetThreadPri(0,0);
  for ( ; ; ) ;
  /* NOT REACHED */
}

/*
 * Idle
 */
static void
idle(void *arg)
{
  /* Activate PI manager */
  osCreatePiManager((OSPri)OS_PRIORITY_PIMGR, &PiMessageQ, PiMessages, DMA_QUEUE_SIZE);

  /*
   * Activate VI manager / initialize video mode
   */
  osCreateViManager(OS_PRIORITY_VIMGR);
  osViSetMode(&osViModeTable[VIMODE]);
  osViSetXScale(1.0f);
  osViSetYScale(1.0f);
  osViSetSpecialFeatures(OS_VI_DIVOT_OFF | OS_VI_GAMMA_OFF);

  /* Activate main thread */
  osCreateThread(&mainThread, 3, mainproc, NULL, mainThreadStack + STACKSIZE/8, 10);
  osStartThread(&mainThread);

  /* Become idle  */
  osSetThreadPri(0,0);
  for ( ; ; ) ;
  /* NOT REACHED */
}

/*
 * Boot
 */
void
boot()
{
  osInitialize();
  osCreateThread(&idleThread, 1, idle, NULL, idleThreadStack + STACKSIZE/8, 10);
  osStartThread(&idleThread);
  /* NOT REACHED */
}

/*
 *  Transfer from ROM to RAM
 */
void
romcpy(void *dest, void *src, int len)
{
  OSIoMesg dmaIoMesgBuf;
  OSMesg dummyMesg;
    
  osInvalDCache(dest, (s32)len);
  osPiStartDma(&dmaIoMesgBuf, OS_MESG_PRI_NORMAL, OS_READ, (u32)src, dest, (u32)len, &dmaMessageQ);
  (void)osRecvMesg(&dmaMessageQ, &dummyMesg, OS_MESG_BLOCK);
}

/* end */
