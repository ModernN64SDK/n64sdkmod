
#define __GFX_C__

/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	Display list manager
 :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */

#include "pack.h"

/* ===========================================================================
	defines
 =========================================================================== */

/* capacity of fifo buffer (1K unit) */
#define GTSK_FIFOLEN 65536

#define GFX_REGULAR_HEAP 4096;

/* reference from data.c */
extern Gfx gfxinit_dl[];

/* --------------------------------------------------------------
		Cautions for N64

	Two types of address are necessary for frame buffer

    osViSwapBuffer() requests a virtual address.  Segment address
    specified by DL is a physical address.

	Be careful with a memory alignment!

    Color frame buffer, z buffer      		... 64 byte
    FIFO buffer  					... 16 byte
    others					... 8 byte
 -------------------------------------------------------------- */

/* ===========================================================================

   static variables
 =========================================================================== */

static Gfx ForceAlignmentPadding[] = { gsSPEndDisplayList() };

static u64 dramstack[ SP_DRAM_STACK_SIZE64 ];

static OSTask tl_default =
{
    M_GFXTASK,			/* task type */
    OS_TASK_DP_WAIT,		/* task flags */
    NULL,			/* boot ucode pointer (fill in later) */
    0,				/* boot ucode size (fill in later) */
    NULL,			/* task ucode pointer (fill in later) */
    SP_UCODE_SIZE,		/* task ucode size */
    NULL,			/* task ucode data pointer (fill in later) */
    SP_UCODE_DATA_SIZE,		/* task ucode data size */
    (void*)dramstack,		/* task dram stack pointer */
    SP_DRAM_STACK_SIZE8,	/* task dram stack size */
    NULL,			/* task output buffer ptr (not always used) */
    0,				/* task output buffer size ptr */
    NULL,			/* task data pointer (fill in later) */
    0,				/* task data size (fill in later) */
    NULL,			/* task yield buffer ptr (not used here) */
    0				/* task yield buffer size (not used here) */
};

static OSMesgQueue rspMessageQ, rdpMessageQ, retraceMessageQ;
static OSMesg rspMessageBuf, rdpMessageBuf, retraceMessageBuf;

/* ---------------------------------------------------------------------------
	Buffer pointer for each condition
 --------------------------------------------------------------------------- */
/* buffer being created by CPU */
static int activeHeapLength;
static void *activeHeap;
static Gfx *activeGfx;

/* Creating by CPU ended and buffer is standing by for rendering */
static void *pendingHeap;
static OSTask *pendingOSTask;

/* buffer being rendered by SP and DP */
static void *drawingHeap;
static OSTask *drawingOSTask;
static void *drawingCFB;
static void *drawingZBUF;

/* buffer being displayed on the screen */
static void *viewingCFB;

/* buffer standing by for release */
static void *dumpCFB;

/* ===========================================================================
	public functions
 =========================================================================== */

/* ---------------------------------------------------------------------------
	Initialize module
 --------------------------------------------------------------------------- */
void gfxInit(void)
{
    u64 *ptr;

    osCreateMesgQueue(&rspMessageQ, &rspMessageBuf, 1);
    osSetEventMesg(OS_EVENT_SP, &rspMessageQ, NULL);

    osCreateMesgQueue(&rdpMessageQ, &rdpMessageBuf, 1);
    osSetEventMesg(OS_EVENT_DP, &rdpMessageQ, NULL);

    osCreateMesgQueue(&retraceMessageQ, &retraceMessageBuf, 1);
    osViSetEvent(&retraceMessageQ, NULL, 1);

    tl_default.t.ucode_boot = (u64 *) rspbootTextStart;
    tl_default.t.ucode_boot_size = (u32)rspbootTextEnd-(u32)rspbootTextStart;
    tl_default.t.ucode = (u64 *)gspF3DEX2_NoN_fifoTextStart;
    tl_default.t.ucode_data = (u64 *)gspF3DEX2_NoN_fifoDataStart;
    ptr = ALIGNER(malloc(GTSK_FIFOLEN + 16), 16);
    tl_default.t.output_buff = ptr;
    tl_default.t.output_buff_size = (u64*)((u32)ptr + GTSK_FIFOLEN);
    activeHeapLength = 0;
    activeHeap = activeGfx = NULL;
    pendingHeap = pendingOSTask = NULL;
    drawingHeap = drawingOSTask = drawingCFB = drawingZBUF = NULL;
    viewingCFB = dumpCFB = NULL;
}

/* ---------------------------------------------------------------------------
	Start creating rendering DL
 --------------------------------------------------------------------------- */
Gfx *gfxBegin( int gfxcnt )
{
    void *ap;
    Gfx *gp;

    /* If currently creating buffer, do nothing */
    if(activeHeap) return NULL;

    /* If there is a task that is prepared to execute, proceed to render */
    if(pendingOSTask) gfxFlush();

    /* Reserve buffer */
    activeHeapLength =
      GFX_REGULAR_HEAP + sizeof(OSTask) + (sizeof(Gfx) * gfxcnt);
    activeHeap = ap = acreate( activeHeapLength );
    activeGfx = gp = (Gfx*)K0_TO_K1(amalloc( sizeof(Gfx)*gfxcnt, ap ));

    /* Make a space to assign segment address */
    gDPNoOp(gp++);
    gDPNoOp(gp++);

    /* Call initialized DL */
    gSPSegment(gp++, PHISICAL_SEGMENT, 0);
    gSPDisplayList(gp++, gfxinit_dl);

    return gp;
}

/* ---------------------------------------------------------------------------
	End creating rendering DL
 --------------------------------------------------------------------------- */
void gfxEnd(Gfx *gfx)
{
    void *ap;
    OSTask *tlp;
    u32 gfxlen;

    if(!(ap = activeHeap)) return;

    gDPFullSync(gfx++);
    gSPEndDisplayList(gfx++);

    gfxlen = (u32)((gfx - activeGfx) * sizeof(Gfx));
    tlp = amalloc(sizeof(OSTask), ap);
    memcpy(tlp, &tl_default, sizeof(OSTask));
    tlp->t.data_ptr = (u64*)activeGfx;
    tlp->t.data_size = gfxlen;
    activeGfx = NULL;

    osWritebackDCache(activeHeap, activeHeapLength);
    osWritebackDCache(&tlp, sizeof(OSTask));

    pendingHeap = activeHeap;
    pendingOSTask = tlp;

    activeHeap = NULL;
}

/* ---------------------------------------------------------------------------
	Run rendering task
 --------------------------------------------------------------------------- */
void gfxFlush(void)
{
    Gfx *gp;
    u32 ptr;

    /* If there is no task that is prepared to execute, do nothing. */
    if( !pendingHeap ) return;

    /* If rendering task is running, wait until finisihed */
    if(drawingOSTask){
	(void)osRecvMesg(&rspMessageQ, NULL, OS_MESG_BLOCK);
	(void)osRecvMesg(&rdpMessageQ, NULL, OS_MESG_BLOCK);
	adelete(drawingHeap);
	free(drawingZBUF);
	drawingOSTask = NULL;

	/* Switch display buffer */
	dumpCFB = viewingCFB;
	viewingCFB = drawingCFB;
	osViSwapBuffer(ALIGNER(viewingCFB, 64));
	osViBlack(FALSE);
	drawingCFB = NULL;
    }

    /* rendering task entry */
    gp = (Gfx*)pendingOSTask->t.data_ptr;
    drawingCFB = malloc(CFB_SIZE+64);
    ptr = osVirtualToPhysical(ALIGNER(drawingCFB, 64));
    gSPSegment(gp++, CFB_SEGMENT, ptr);

    drawingZBUF = malloc(ZBUF_SIZE+64);
    ptr = osVirtualToPhysical(ALIGNER(drawingZBUF, 64));
    gSPSegment(gp++, ZBUF_SEGMENT, ptr);

    drawingHeap = pendingHeap;
    drawingOSTask = pendingOSTask;
    pendingHeap = NULL;
    pendingOSTask = NULL;

    osWritebackDCache(drawingOSTask, sizeof(OSTask));

    osSpTaskStart(drawingOSTask);
}

/* ---------------------------------------------------------------------------
	Retrace stand by
 --------------------------------------------------------------------------- */
void gfxWaitSync(void)
{
    /* Enter buffer that has already created */
    gfxFlush();

    /* Retrace stand by */
    if (MQ_IS_FULL(&retraceMessageQ))
      (void)osRecvMesg(&retraceMessageQ, NULL, OS_MESG_BLOCK);
    (void)osRecvMesg(&retraceMessageQ, NULL, OS_MESG_BLOCK);

    /* Discard dump buffer, if any */
    if(dumpCFB){
	free(dumpCFB);
	dumpCFB = NULL;
    }
}

/* ---------------------------------------------------------------------------
	Operate matrix
	Please note that an argument m is copied and written in Gfx.  
 --------------------------------------------------------------------------- */
Gfx *gfxMatrix( Gfx *gp, Mtx *m, u32 flag )
{
    Mtx *mdest;

    mdest = amalloc(sizeof(Mtx), activeHeap);
    memcpy(mdest, m, sizeof(Mtx) );
    gSPMatrix(gp++, OS_K0_TO_PHYSICAL(mdest), flag);
    return gp;
}

/* ---------------------------------------------------------------------------
	Assign projective conversion matrix
 --------------------------------------------------------------------------- */
Gfx *gfxPerspective( Gfx *gp, float fovy, float aspect, float near, float far, float scale )
{
    u16 perspNorm;
    Mtx m;

    guPerspective( &m, &perspNorm, fovy, aspect, near, far, scale);
    gSPPerspNormalize(gp++, perspNorm);
    return gfxMatrix( gp, &m, G_MTX_PROJECTION|G_MTX_LOAD|G_MTX_NOPUSH);
}

/* ---------------------------------------------------------------------------
	Assign parallel shadowing matrix
 --------------------------------------------------------------------------- */
Gfx *gfxOrtho( Gfx *gp, float left, float right, float bottom, float top, float near, float far, float scale )
{
    Mtx m;

    guOrtho( &m, left, right, bottom, top, near, far, scale);
    return gfxMatrix( gp, &m, G_MTX_PROJECTION|G_MTX_LOAD|G_MTX_NOPUSH);
}

/* ---------------------------------------------------------------------------
	Create model matrix using 3 values - image point, focal point, and 
	upward direction and assign it.
 --------------------------------------------------------------------------- */
Gfx *gfxLookAt( Gfx *gp, float ex, float ey, float ez,
	                 float ax, float ay, float az,
	                 float ux, float uy, float uz)
{
    Mtx m;

    guLookAt( &m, ex, ey, ez, ax, ay, az, ux, uy, uz);
    return gfxMatrix( gp, &m, G_MTX_MODELVIEW|G_MTX_LOAD|G_MTX_NOPUSH);
}

/* ---------------------------------------------------------------------------
	Set to overwrite model matrix
 --------------------------------------------------------------------------- */
Gfx *gfxLoadMatrix( Gfx *gp, Mtx *m )
{
    return gfxMatrix( gp, m, G_MTX_MODELVIEW|G_MTX_LOAD|G_MTX_NOPUSH);
}

/* ---------------------------------------------------------------------------
	Multiply model matrix
 --------------------------------------------------------------------------- */
Gfx *gfxMultiMatrix( Gfx *gp, Mtx *m )
{
    return gfxMatrix( gp, m, G_MTX_MODELVIEW|G_MTX_MUL|G_MTX_NOPUSH);
}

/* ---------------------------------------------------------------------------
	Push model matrix into modeling matrix stack and multiply
 --------------------------------------------------------------------------- */
Gfx *gfxPushMatrix( Gfx *gp, Mtx *m )
{
    return gfxMatrix( gp, m, G_MTX_MODELVIEW|G_MTX_MUL|G_MTX_PUSH);
}

/* ---------------------------------------------------------------------------
	Pop model matrix out of modeling matrix stack.
 --------------------------------------------------------------------------- */
Gfx *gfxPopMatrix( Gfx *gp )
{
    gSPPopMatrix(gp++, G_MTX_MODELVIEW);
    return gp;
}
