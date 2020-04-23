/**************************************************************************
 *                                                                        *
 *               Copyright (C) 1995, Silicon Graphics, Inc.               *
 *                                                                        *
 *  These coded instructions, statements, and computer programs  contain  *
 *  unpublished  proprietary  information of Silicon Graphics, Inc., and  *
 *  are protected by Federal copyright  law.  They  may not be disclosed  *
 *  to  third  parties  or copied or duplicated in any form, in whole or  *
 *  in part, without the prior written consent of Silicon Graphics, Inc.  *
 *                                                                        *
 *************************************************************************/

/*
 * File:	onetri.c
 * Create Date:	Mon Apr 17 11:45:57 PDT 1995
 *
 * VERY simple app, draws a couple triangles spinning.
 *
 */

#include <ultra64.h>
#include <PR/ramrom.h>	/* needed for argument passing into the app */
#include <assert.h>

#include "main.h"
#include "cont.h"
/* #include "diskproc.h" */
#include "hdd.h"

/*
 * Stacks for the threads as well as message queues for synchronization
 * This stack is ridiculously large, and could also be reclaimed once
 * the main thread is started.
 */
u64	bootStack[STACKSIZE/sizeof(u64)];

static void	idle(void *);
static void	mainproc(void *);

static OSThread	idleThread;
static u64	idleThreadStack[STACKSIZE/sizeof(u64)];

static OSThread	mainThread;
static u64	mainThreadStack[STACKSIZE/sizeof(u64)];

#define	DISK_MSG_NUM 1
OSMesgQueue	diskMsgQ;
static OSMesg	diskMsgBuf[DISK_MSG_NUM];

/* this number (the depth of the message queue) needs to be equal
 * to the maximum number of possible overlapping PI requests.
 * For this app, 1 or 2 is probably plenty, other apps might
 * require a lot more.
 */
#define NUM_PI_MSGS     8

static OSMesg PiMessages[NUM_PI_MSGS];
static OSMesgQueue PiMessageQ;

OSMesgQueue	dmaMessageQ, rdpMessageQ;
OSMesgQueue	retraceMessageQ, siMessageQ;
OSMesg		dmaMessageBuf, rdpMessageBuf;
OSMesg		retraceMessageBuf, siMessageBuf;
OSIoMesg	dmaIOMessageBuf;	/* see man page to understand this */
OSMesgQueue     rstMessageQ;
OSMesg          rstMessageBuf;
s16		resetflag = 0;

/*
 * Dynamic data.
 */
Dynamic dynamic;

/*
 * Task descriptor.
 */
OSTask	tlist =
{
    M_GFXTASK,			/* task type */
    OS_TASK_DP_WAIT,		/* task flags */
    NULL,			/* boot ucode pointer (fill in later) */
    0,				/* boot ucode size (fill in later) */
    NULL,			/* task ucode pointer (fill in later) */
    SP_UCODE_SIZE,		/* task ucode size */
    NULL,			/* task ucode data pointer (fill in later) */
    SP_UCODE_DATA_SIZE,		/* task ucode data size */
    &(dram_stack[0]),		/* task dram stack pointer */
    SP_DRAM_STACK_SIZE8,	/* task dram stack size */
    &(rdp_output[0]),		/* task output start buffer ptr */
    &rdp_output[0]+RDP_OUTPUT_LEN, /* task output end buffer ptr */
    NULL,			/* task data pointer (fill in later) */
    0,				/* task data size (fill in later) */
    NULL,			/* task yield buffer ptr (not used here) */
    0				/* task yield buffer size (not used here) */
};

Gfx		*glistp;	/* global for test case procs */
    
/*
 * global variables
 */
static float	theta = 0.0;
static int	rdp_flag = 0;
int      draw_buffer = 0;

void boot(void);

void
boot(void)
{
    /* notice that you can't call osSyncPrintf() until you set
     * up an idle thread.
     */
    
    osInitialize();

    osCreateThread(&idleThread, 1, idle, (void *)0,
		   idleThreadStack+STACKSIZE/sizeof(u64), 10);
    osStartThread(&idleThread);

    /* never reached */
}

static void
idle(void *arg)
{
    /* Initialize video */
    osCreateViManager(OS_PRIORITY_VIMGR);
    osViSetMode(&osViModeTable[OS_VI_NTSC_LAN1]);

    osViBlack(1);
    
    osViSetSpecialFeatures( OS_VI_GAMMA_OFF | OS_VI_GAMMA_DITHER_OFF );
    osViSetSpecialFeatures( OS_VI_DITHER_FILTER_ON );

    bzero(cfb, SCREEN_WD*SCREEN_HT*4);
    osViSwapBuffer(cfb[draw_buffer]);


    /* SI */
    osCreateMesgQueue(&siMessageQ, &siMessageBuf, 1);
    osSetEventMesg(OS_EVENT_SI, &siMessageQ, (OSMesg)1);	/* SI */

    osCreateMesgQueue(&rstMessageQ, &rstMessageBuf, 1);
    osSetEventMesg(OS_EVENT_PRENMI, &rstMessageQ, NULL);


    /*
     * Start PI Mgr for access to cartridge
     */
    osCreatePiManager((OSPri)OS_PRIORITY_PIMGR, &PiMessageQ, PiMessages, 
		      NUM_PI_MSGS);
    
    /*
     * Create main thread
     */
    osCreateThread(&mainThread, 3, mainproc, arg,
		   mainThreadStack+STACKSIZE/sizeof(u64), 10);
    
    osStartThread(&mainThread);

    /*
     * Become the idle thread
     */
    osSetThreadPri(0, 0);

    for (;;);
}

Dynamic		*dynamicp;
int     	kanadr=0;

/*
 * This is the main routine of the app.
 */
static void
mainproc(void *arg)
{
    OSTask		*tlistp;
    void sp_do_work( Gfx ** );

    arg = arg;/* never mind! :-P */

    /*
     * Setup the message queues
     */
    osCreateMesgQueue(&dmaMessageQ, &dmaMessageBuf, 1);
    
    osCreateMesgQueue(&rdpMessageQ, &rdpMessageBuf, 1);
    osSetEventMesg(OS_EVENT_DP, &rdpMessageQ, NULL);
    
    osCreateMesgQueue(&retraceMessageQ, &retraceMessageBuf, 1);
    osViSetEvent(&retraceMessageQ, NULL, 1);

    osCreateMesgQueue(&diskMsgQ, diskMsgBuf, DISK_MSG_NUM);

#if 0

    osCreateThread(&diskThread, 8, (void *)diskproc, 0,
		   diskStack+STACKSIZE/sizeof(u64), 12);
    osStartThread(&diskThread);

    handler = osDriveRomInit();
    for ( i = 0 ; i < TEST_BYTE_SIZE ; i ++ ) {
      readwritebuf[i] = i;
    }

#endif

    initcont(&siMessageQ);	/* Initialize controllers */

#if 0
    for ( i = 0 ; i < ERROR_MES_SIZE ; i ++ ) {
      kaddr[i] = kptr;
      kptr = initkanchr(message[i], kptr);
    } 
#endif

    osViBlack(0);

/* end add */

    /*
     * Main game loop
     */
    while (1) {

	  dd_flush();

	/*
	 * pointers to build the display list.
	 */
	tlistp = &tlist;
	dynamicp = &dynamic;

	guOrtho(&dynamicp->projection,
		-(float)SCREEN_WD/2.0F, (float)SCREEN_WD/2.0F,
		-(float)SCREEN_HT/2.0F, (float)SCREEN_HT/2.0F,
		1.0F, 10.0F, 1.0F);
	guRotate(&dynamicp->modeling, theta, 0.0F, 0.0F, 1.0F);

	glistp = dynamicp->glist;

	/*
	 * Tell RCP where each segment is
	 */
	gSPSegment(glistp++, 0, 0x0);	/* Physical address segment */
	gSPSegment(glistp++, CFB_SEGMENT, OS_K0_TO_PHYSICAL(cfb[draw_buffer]));

	/*
	 * Initialize RDP state.
	 */
	gSPDisplayList(glistp++, rdpinit_dl);

	/*
	 * Initialize RSP state.
	 */
	gSPDisplayList(glistp++, rspinit_dl);

	/*
	 * Clear color framebuffer.
	 */
	gSPDisplayList(glistp++, clearcfb_dl);

	/*
	 * Work for Sprite & Test 64DD 
	 */
	sp_do_work( &glistp );

	gDPFullSync(glistp++);
	gSPEndDisplayList(glistp++);

#ifdef DEBUG
	assert((glistp-dynamicp->glist) < GLIST_LEN);
#endif

	/* 
	 * Build graphics task:
	 *
	 */
	tlistp->t.ucode_boot = (u64 *) rspbootTextStart;
	tlistp->t.ucode_boot_size = (u32)rspbootTextEnd - (u32)rspbootTextStart;

	/*
	 * choose which ucode to run:
	 */
	if( conts[0].nowtrg & L_TRIG ) rdp_flag = 0;
	if( conts[0].nowtrg & R_TRIG ) rdp_flag = 1;
	if (rdp_flag) {
	    /* RSP output over FIFO to RDP: */
		tlistp->t.ucode = (u64 *) gspF3DEX2_fifoTextStart;
		tlistp->t.ucode_data = (u64 *) gspF3DEX2_fifoDataStart; 
	} else {
	    /* RSP output over XBUS to RDP: */
		tlistp->t.ucode = (u64 *) gspF3DEX2_xbusTextStart;
		tlistp->t.ucode_data = (u64 *) gspF3DEX2_xbusDataStart;
	}
	
	/* initial display list: */
	tlistp->t.data_ptr = (u64 *) dynamicp->glist;
	tlistp->t.data_size = (u32)((glistp - dynamicp->glist) * sizeof(Gfx));

	/*
	 * Write back dirty cache lines that need to be read by the RCP.
	 */
	osWritebackDCache(&dynamic, sizeof(dynamic));
	
	/*
	 * start up the RSP task
	 */

	osSpTaskStart(tlistp);
	
	/* wait for RDP completion */
	(void)osRecvMesg(&rdpMessageQ, NULL, OS_MESG_BLOCK);

	osContStartReadData(&siMessageQ);

	if(MQ_IS_FULL(&rstMessageQ)){
	  osRecvMesg(&rstMessageQ, NULL, OS_MESG_BLOCK);
	  resetflag = 1;
	  LeoReset();
	}

	/* setup to swap buffers */
	osViSwapBuffer(cfb[draw_buffer]);

	/* Make sure there isn't an old retrace in queue 
	 * (assumes queue has a depth of 1) 
	 */
	if (MQ_IS_FULL(&retraceMessageQ))
	    (void)osRecvMesg(&retraceMessageQ, NULL, OS_MESG_BLOCK);
	
	/* Wait for Vertical retrace to finish swap buffers */
	(void)osRecvMesg(&retraceMessageQ, NULL, OS_MESG_BLOCK);

	osRecvMesg(&siMessageQ, NULL, OS_MESG_BLOCK);
	readcont();

	draw_buffer ^= 1;
    }
}
