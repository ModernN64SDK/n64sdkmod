
/*
 * Copyright 1995, Silicon Graphics, Inc.
 * ALL RIGHTS RESERVED
 *
 * UNPUBLISHED -- Rights reserved under the copyright laws of the United
 * States.   Use of a copyright notice is precautionary only and does not
 * imply publication or disclosure.
 *
 * U.S. GOVERNMENT RESTRICTED RIGHTS LEGEND:
 * Use, duplication or disclosure by the Government is subject to restrictions
 * as set forth in FAR 52.227.19(c)(2) or subparagraph (c)(1)(ii) of the Rights
 * in Technical Data and Computer Software clause at DFARS 252.227-7013 and/or
 * in similar or successor clauses in the FAR, or the DOD or NASA FAR
 * Supplement.  Contractor/manufacturer is Silicon Graphics, Inc.,
 * 2011 N. Shoreline Blvd. Mountain View, CA 94039-7311.
 *
 * THE CONTENT OF THIS WORK CONTAINS CONFIDENTIAL AND PROPRIETARY
 * INFORMATION OF SILICON GRAPHICS, INC. ANY DUPLICATION, MODIFICATION,
 * DISTRIBUTION, OR DISCLOSURE IN ANY FORM, IN WHOLE, OR IN PART, IS STRICTLY
 * PROHIBITED WITHOUT THE PRIOR EXPRESS WRITTEN PERMISSION OF SILICON
 * GRAPHICS, INC.
 *
 */

/*
 * File:	timer.c
 * Creator:	hsa@sgi.com
 * Create Date:	Tue Nov  7 14:26:04 PST 1995
 *
 */

#include <ultra64.h>
#include "gtdemo.h"
#include "timer.h"

/* some colors */
#define RED	(GPACK_RGBA5551(255,0,0,1) << 16 | GPACK_RGBA5551(255,0,0,1))
#define GREEN	(GPACK_RGBA5551(0,255,0,1) << 16 | GPACK_RGBA5551(0,255,0,1))
#define BLUE	(GPACK_RGBA5551(0,0,255,1) << 16 | GPACK_RGBA5551(0,0,255,1))
#define LTRED	(GPACK_RGBA5551(255,127,127,1) << 16 | GPACK_RGBA5551(255,127,127,1))
#define LTGREEN	(GPACK_RGBA5551(127,255,127,1) << 16 | GPACK_RGBA5551(127,255,127,1))
#define LTBLUE	(GPACK_RGBA5551(127,127,255,1) << 16 | GPACK_RGBA5551(127,127,255,1))
#define YELLOW	(GPACK_RGBA5551(255,255,0,1) << 16 | GPACK_RGBA5551(255,255,0,1))

extern OSMesgQueue	rdpMessageQ, retraceMessageQ;
extern gtGfx	*gtlistp;

static int	TimerTicks = 7;
static float    TicksPerSec;
static OSTime	l_bg_cpu_start, l_bg_cpu_end, l_fg_cpu_start, l_fg_cpu_end;
static OSTime   l_bg_rcp_start, l_bg_rcp_end, l_fg_rcp_start, l_fg_rcp_end;
static float    BGTimePerFrame =  0, FGTimePerFrame;

OSTime          bg_cpu_start, bg_cpu_end, fg_cpu_start, fg_cpu_end;
OSTime          bg_rcp_start, bg_rcp_end, fg_rcp_start, fg_rcp_end;

/* this turbo object holds the timer bars */
static Gfx	timerMoreRDPCMDS[32];
static gtState	timerObj =
{
    0x0,	/* renderState */
    0x0,	/* textureState */
    0,		/* vtxCount */
    0,		/* vtxV0 */
    0,		/* triCount */
    0x0, 	/* pad1 */
    &(timerMoreRDPCMDS[0]),
    gsDPClearOtherMode(),			/* rdpOthermode */
    {	/* integer portion: */
	0x00010000, 0x00000000,			/* transform */
	0x00000001, 0x00000000,
	0x00000000, 0x00010000,
	0x00000000, 0x00000001,
	/* fractional portion: */
	0x00000000, 0x00000000,
	0x00000000, 0x00000000,
	0x00000000, 0x00000000,
	0x00000000, 0x00000000,
    }
};

/* called once at init time, gets proper CPU clockspeed */
static void 
ComputeClockSpeed(void)
{
    u32 count0, count1;

    /* wait for several retraces so the VI MGR settles down
     * before we time one.
     */
    osRecvMesg(&retraceMessageQ, 0, OS_MESG_BLOCK);
    osRecvMesg(&retraceMessageQ, 0, OS_MESG_BLOCK);
    osRecvMesg(&retraceMessageQ, 0, OS_MESG_BLOCK);
    osRecvMesg(&retraceMessageQ, 0, OS_MESG_BLOCK);
    osRecvMesg(&retraceMessageQ, 0, OS_MESG_BLOCK);

    osRecvMesg(&retraceMessageQ, 0, OS_MESG_BLOCK);
    count0 = osGetCount();
    osRecvMesg(&retraceMessageQ, 0, OS_MESG_BLOCK);
    count1 = osGetCount();

    TicksPerSec = ((osTvType == 0) ? 50.0 : 60.0) * (count1 - count0);
}

/* called once at init time */
void
TimerInit(void)
{
    ComputeClockSpeed();
    bg_rcp_start = 0;
    bg_rcp_end = 0;
    fg_rcp_start = 0;
    fg_rcp_end = 0;
    bg_cpu_start = 0;
    bg_cpu_end = 0;
    fg_cpu_start = 0;
    fg_cpu_end = 0;
    TimerFinish();
}

/* called at the end of every frame. The timer bars really display
 * data from the previous frame, so we must copy the current frame
 * to the last frame to be displayed at the next frame...
 */
void
TimerFinish(void)
{
    l_bg_rcp_start = bg_rcp_start;
    l_bg_rcp_end = bg_rcp_end;
    l_fg_rcp_start = fg_rcp_start;
    l_fg_rcp_end = fg_rcp_end;
    l_bg_cpu_start = bg_cpu_start;
    l_bg_cpu_end = bg_cpu_end;
    l_fg_cpu_start = fg_cpu_start;
    l_fg_cpu_end = fg_cpu_end;
}

/*
 * create a turbo object that holds the timer display bars.
 */
void
WriteTimerBar(Dynamic *dynamicp, gtGfx	**gtlistp)
{
    float 	TickSpacing, BGBarPercent, FGBarPercent;
    float 	BGCPUBarPercent, FGCPUBarPercent;
    float	BGRCPTime, FGRCPTime, BGCPUTime, FGCPUTime;
    int		bg_cpu_x1, bg_cpu_x2, bg_rcp_x1, bg_rcp_x2;
    int		fg_cpu_x1, fg_cpu_x2, fg_rcp_x1, fg_rcp_x2;
    int 	i;
    Gfx		*gptr;
    
    BGRCPTime = (1000.0 * (float) (l_bg_rcp_end - l_bg_rcp_start) / 
	      ((OSTime) (TicksPerSec)));
    FGRCPTime = (1000.0 * (float) (l_fg_rcp_end - l_fg_rcp_start) / 
	      ((OSTime) (TicksPerSec)));
    BGBarPercent = BGRCPTime / (TIMERINTERVAL * (TimerTicks - 1));
    FGBarPercent = FGRCPTime / (TIMERINTERVAL * (TimerTicks - 1));

    BGCPUTime = (1000.0 * (float) (l_bg_cpu_end - l_bg_cpu_start) / 
	      ((OSTime) (TicksPerSec)));
    FGCPUTime = (1000.0 * (float) (l_fg_cpu_end - l_fg_cpu_start) / 
	      ((OSTime) (TicksPerSec)));
    BGCPUBarPercent = BGCPUTime / (TIMERINTERVAL * (TimerTicks - 1));
    FGCPUBarPercent = FGCPUTime / (TIMERINTERVAL * (TimerTicks - 1));

    bg_cpu_x1 = TIMERMINX;
    bg_cpu_x2 = bg_cpu_x1 + (int)((TIMERMAXX - TIMERMINX)*BGCPUBarPercent);
    bg_rcp_x1 = bg_cpu_x2 + 1;
    bg_rcp_x2 = bg_rcp_x1 + (int)((TIMERMAXX - TIMERMINX)*BGBarPercent);

    fg_cpu_x1 = bg_rcp_x1;	/* same as bg_rcp */
    fg_cpu_x2 = fg_cpu_x1 + (int)((TIMERMAXX - TIMERMINX)*FGCPUBarPercent);

    fg_rcp_x1 = MAX(bg_rcp_x2, fg_cpu_x2) + 1;
    fg_rcp_x2 = fg_rcp_x1 + (int)((TIMERMAXX - TIMERMINX)*FGBarPercent);

    gtStateSetOthermode(&(timerObj.sp.rdpOthermode), GT_RENDERMODE,
			(G_RM_OPA_SURF | G_RM_OPA_SURF2));
    gtStateSetOthermode(&(timerObj.sp.rdpOthermode), GT_CYCLETYPE, 
			G_CYC_FILL);
    gptr = (Gfx *) &(timerMoreRDPCMDS[0]);

    if ((FGCPUBarPercent+FGCPUBarPercent+BGBarPercent+FGBarPercent) < 1.0) {

	/* background CPU time: */
	gDPSetFillColor(gptr++, BLUE);
	gDPFillRectangle(gptr++, bg_cpu_x1, TIMERMINY, bg_cpu_x2, TIMERMAXY);
	gDPPipeSync(gptr++);

	/* background RCP time: */
	gDPSetFillColor(gptr++, LTBLUE);
	gDPFillRectangle(gptr++, bg_rcp_x1, TIMERMINY, bg_rcp_x2, TIMERMIDY);
	gDPPipeSync(gptr++);

	/* foreground CPU time: */
	gDPSetFillColor(gptr++, RED);
	gDPFillRectangle(gptr++, fg_cpu_x1, TIMERMIDY, fg_cpu_x2, TIMERMAXY);
	gDPPipeSync(gptr++);

	/* foreground RCP time: */
	gDPSetFillColor(gptr++, LTRED);
	gDPFillRectangle(gptr++, fg_rcp_x1, TIMERMINY, fg_rcp_x2, TIMERMAXY);
	gDPPipeSync(gptr++);

    } else {	/* Overflowed bar counter range */
	gDPSetFillColor(gptr++, RED);
	gDPFillRectangle(gptr++, TIMERMINX, TIMERMINY, TIMERMAXX, TIMERMAXY);
	gDPPipeSync(gptr++);
    }
    
    gDPSetFillColor(gptr++, GREEN);

    TickSpacing = (TIMERMAXX - TIMERMINX) / (TimerTicks - 1.0);
    for (i=0; i<TimerTicks; i++) {
	gDPFillRectangle(gptr++, 
			 TIMERMINX + (int) TickSpacing*i, TIMERMINY, 
			 TIMERMINX + (int) TickSpacing*i, TIMERMAXY);
    }
    
    gDPPipeSync(gptr++);
    gDPEndDisplayList(gptr++);

    /* timer state: */
    (*gtlistp)->obj.gstatep = (gtGlobState *) NULL;
    (*gtlistp)->obj.statep = &timerObj;
    (*gtlistp)->obj.vtxp = (Vtx *) NULL;
    (*gtlistp)->obj.trip = (gtTriN *) NULL;
    (*gtlistp)++;
}
