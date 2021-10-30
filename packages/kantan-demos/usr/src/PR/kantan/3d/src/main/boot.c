

#define __BOOT_C__

/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	Boot strap
 :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */

#include "pack.h"

/* ===========================================================================
	static variables
 =========================================================================== */
static OSThread	idleThread;
static u64 idleThreadStack[STACKSIZE/sizeof(u64)];

static OSThread	mainThread;
static u64 mainThreadStack[STACKSIZE/sizeof(u64)];

#define NUM_PI_MSGS 8

static OSMesg PiMessages[NUM_PI_MSGS];
static OSMesgQueue PiMessageQ;

static OSMesg dmaMessageBuf;
static OSMesgQueue dmaMessageQ;

static int debugflag = 0;
static int draw_buffer = 0;

/* ===========================================================================
	public variables
 =========================================================================== */
/* Boot stack to be used for spec   */
u64 bootStack[STACKSIZE/sizeof(u64)];

/* ===========================================================================
	functions
 =========================================================================== */

/* ----------------------------------------------------------------------------
  Boot strap
---------------------------------------------------------------------------- */
static void idle(void *);
static void mainproc(void *);

void boot(void)
{
    osInitialize();

    osCreateThread(&idleThread, 1, idle, (void *)0,
		   idleThreadStack+STACKSIZE/sizeof(u64), 10);
    osStartThread(&idleThread);
}

/* ----------------------------------------------------------------------------
   Idle thread
---------------------------------------------------------------------------- */
static void idle(void *arg)
{
    /* Run VI manager */
    osCreateViManager(OS_PRIORITY_VIMGR);
    osViSetMode(&osViModeTable[OS_VI_NTSC_LAN1]);
    osViBlack(TRUE);

    /* Run PI manager */
    osCreatePiManager((OSPri)OS_PRIORITY_PIMGR, &PiMessageQ, PiMessages, 
		      NUM_PI_MSGS);

    /* Main thread */
    osCreateThread(&mainThread, 3, mainproc, arg,
		   mainThreadStack+STACKSIZE/sizeof(u64), 10);
    osStartThread(&mainThread);

    /* Keep in the state of idle thread */
    osSetThreadPri(0, 0);
    for (;;);
}

/* ----------------------------------------------------------------------------
   Main thread
---------------------------------------------------------------------------- */
static void mainproc(void *arg)
{
    osCreateMesgQueue(&dmaMessageQ, &dmaMessageBuf, 1);

    /* Initialize memory manager */
    minit();

    /*Initialize graphic module */
    gfxInit();

    /* Initialize controller */
    expadInitControllers( 4, 20, 4 );

    /* Call user routine */
    entry();
}

