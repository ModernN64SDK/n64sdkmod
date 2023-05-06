/*============================================================================
  boot.c
  ============================================================================*/
#include <ultra64.h>
#include <PR/ramrom.h>
#include <assert.h>

#include "def.h"
#include "segment.h"
#include "message.h"

u64 bootStack[STACKSIZE/sizeof(u64)];
static void idle(void *);      /* Idle Thead */

/* Definition of thread structure */
static OSThread idleThread;
static OSThread mainThread;

/* Definition of thread stack */
static u64 idleThreadStack[STACKSIZE/sizeof(u64)];
static u64 mainThreadStack[STACKSIZE/sizeof(u64)];

/* Actual main routine */
extern void mainproc(void *);

/*-----------------------
  System boot
  -----------------------*/
void
boot(void)
{
    /* Initialize software and hardware */
    osInitialize();

    /* Create idle thread */
    osCreateThread(&idleThread,IDLE_THREAD_ID, idle, (void *)0,
		   (idleThreadStack+STACKSIZE/sizeof(u64)), IDLE_THREAD_PRI);

    /* Run idle thread */
    osStartThread(&idleThread);
}

/*--------------------------
  Idle thread
  ---------------------------*/
static void
idle(void *arg)
{
    /* Set up parallel interface manager */
    osCreatePiManager((OSPri)OS_PRIORITY_PIMGR, &PiMessageQ, PiMessages, NUM_PI_MSGS);

    /* Activate main thread */
    osCreateThread(&mainThread, MAIN_THREAD_ID, mainproc,arg,
		   (mainThreadStack+STACKSIZE/sizeof(u64)),MAIN_THREAD_PRI);
    osStartThread(&mainThread);

    /* This thread will enter infinite loop hereafter */
    for(;;);
}
