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

/*---------------------------------------------------------------------*
        Copyright (C) 1998 Nintendo.(Originated by SGI)
        
        $RCSfile: hello_world.c,v $
        $Revision: 1.1 $
        $Date: 1999/07/05 02:19:36 $
 *---------------------------------------------------------------------*/

#include <ultra64.h>
#include <PR/ramrom.h>	/* needed for argument passing into the app */
#include <assert.h>

#include "hello_world.h"

extern char _codeSegmentEnd[];

extern char _staticSegmentRomStart[], _staticSegmentRomEnd[];

u64	bootStack[STACKSIZE/sizeof(u64)];

static void	idle(void *);
static void	mainproc(void *);

static OSThread	idleThread;
static u64	idleThreadStack[STACKSIZE/sizeof(u64)];

static OSThread	mainThread;
static u64	mainThreadStack[STACKSIZE/sizeof(u64)];

#define NUM_PI_MSGS     8

static OSMesg PiMessages[NUM_PI_MSGS];
static OSMesgQueue PiMessageQ;

OSMesgQueue	dmaMessageQ, rspMessageQ, rdpMessageQ, retraceMessageQ;
OSMesg		dmaMessageBuf, rspMessageBuf, rdpMessageBuf, retraceMessageBuf;
OSIoMesg	dmaIOMessageBuf;	/* see man page to understand this */

Dynamic dynamic;

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
    &(rdp_output[0]),		/* task output buffer ptr (not always used) */
    &rdp_output_len,		/* task output buffer size ptr */
    NULL,			/* task data pointer (fill in later) */
    0,				/* task data size (fill in later) */
    NULL,			/* task yield buffer ptr (not used here) */
    0				/* task yield buffer size (not used here) */
};

Gfx		*glistp;	/* global for test case procs */
    
static float	theta = 0.0;
static int	rdp_DRAM_io = 0;
int	do_texture = 0;
static int	debugflag = 0;
static int      draw_buffer = 0;

void boot(void);

OSPiHandle      *handler;

void
boot(void)
{
#ifdef DEBUG
    int i;
    char *ap;
    u32 *argp;
    u32 argbuf[16];
#endif
    
    osInitialize();

    handler = osCartRomInit();

#ifdef DEBUG
    argp = (u32 *)RAMROM_APP_WRITE_ADDR;
    for (i=0; i<(s32)sizeof(argbuf)/4; i++, argp++) {
      osEPiReadIo(handler, (u32)argp, &argbuf[i]); /* Assume no DMA */
    }
    /* Parse the options */
    ap = (char *)argbuf;
    while (*ap != '\0') 
    {
	while (*ap == ' ')
	    ap++;
	if ( *ap == '-' && *(ap+1) == 'd') 
	{
	    debugflag = 1;
	    ap += 2;
	} 
	else if ( *ap == '-' && *(ap+1) == 't') 
	{
	    do_texture = 1;
	    ap += 2;
	} 
	else if ( *ap == '-' && *(ap+1) == 'r') 
	{
	    rdp_DRAM_io = 1;
	    ap += 2;
	}
	else   /* don't know what it is, but move past it. */
	    ap ++;
    }
#endif
    
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
    
    osCreatePiManager((OSPri)OS_PRIORITY_PIMGR, &PiMessageQ, PiMessages, 
		      NUM_PI_MSGS);
    
    osCreateThread(&mainThread, 3, mainproc, arg,
		   mainThreadStack+STACKSIZE/sizeof(u64), 10);
    
    if (!debugflag)
	osStartThread(&mainThread);

    osSetThreadPri(0, 0);

    for (;;);
}

static void
mainproc(void *arg)
{
    OSTask		*tlistp;
    Dynamic		*dynamicp;
/*     char		*staticSegment; */
    
	arg=arg; /*  no means  */

    osCreateMesgQueue(&dmaMessageQ, &dmaMessageBuf, 1);
    
    osCreateMesgQueue(&rspMessageQ, &rspMessageBuf, 1);
    osSetEventMesg(OS_EVENT_SP, &rspMessageQ, NULL);
    
    osCreateMesgQueue(&rdpMessageQ, &rdpMessageBuf, 1);
    osSetEventMesg(OS_EVENT_DP, &rdpMessageQ, NULL);
    
    osCreateMesgQueue(&retraceMessageQ, &retraceMessageBuf, 1);
    osViSetEvent(&retraceMessageQ, NULL, 1);
  
/*  add */
	osViBlack(1);
    
    while (1) {

	tlistp = &tlist;
	dynamicp = &dynamic;

	glistp = dynamicp->glist;

	gSPSegment(glistp++, 0, 0x0);	/* Physical address segment */
	gSPSegment(glistp++, CFB_SEGMENT, OS_K0_TO_PHYSICAL(cfb[draw_buffer]));

	gSPDisplayList(glistp++, rdpinit_dl);

	gSPDisplayList(glistp++, rspinit_dl);

	gSPDisplayList(glistp++, clearcfb_dl);

	/* simple triangle: */

	dd_proc();

	gDPFullSync(glistp++);
	gSPEndDisplayList(glistp++);

	tlistp->t.ucode_boot = (u64 *) rspbootTextStart;
	tlistp->t.ucode_boot_size = (u32)rspbootTextEnd - (u32)rspbootTextStart;

	if (rdp_DRAM_io) {
	    /* re-direct output to DRAM: */
		tlistp->t.ucode = (u64 *) gspFast3D_dramTextStart;
		tlistp->t.ucode_data = (u64 *) gspFast3D_dramDataStart; 
	} else {
	    /* RSP output over XBUS to RDP: */
		tlistp->t.ucode = (u64 *) gspFast3DTextStart;
		tlistp->t.ucode_data = (u64 *) gspFast3DDataStart;
	}
	
	/* initial display list: */
	tlistp->t.data_ptr = (u64 *) dynamicp->glist;
	tlistp->t.data_size = (u32)((glistp - dynamicp->glist) * sizeof(Gfx));

	osWritebackDCache(&dynamic, sizeof(dynamic));
	
	osSpTaskStart(tlistp);
	
	/* wait for SP completion */
	(void)osRecvMesg(&rspMessageQ, NULL, OS_MESG_BLOCK);

	if (rdp_DRAM_io) {
		/* guParseRdpDL(rdp_output, rdp_output_len, 0); */

	    osDpSetNextBuffer(&(rdp_output[0]), rdp_output_len);
	}

	(void)osRecvMesg(&rdpMessageQ, NULL, OS_MESG_BLOCK);

	/* setup to swap buffers */
	osViSwapBuffer(cfb[draw_buffer]);

	if (MQ_IS_FULL(&retraceMessageQ))
	    (void)osRecvMesg(&retraceMessageQ, NULL, OS_MESG_BLOCK);

	/* Wait for Vertical retrace to finish swap buffers */
	(void)osRecvMesg(&retraceMessageQ, NULL, OS_MESG_BLOCK);
	draw_buffer ^= 1;

	theta += 1.0F;
    }
}
