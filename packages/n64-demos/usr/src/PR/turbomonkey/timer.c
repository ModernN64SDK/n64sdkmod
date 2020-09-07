
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

#include <ultra64.h>

#include "block.h"
#include "static.h"
#include "timer.h"

extern gtGlobState	*ggsp;

Gfx	timerMoreRDPCMDS[32];

/* this init's the DP and clears the screen: */
gtState	timerObj =
{
    0x0,	/* renderState */
    0x0,	/* textureState */
    0,		/* vtxCount */
    0,		/* vtxV0 */
    0,		/* triCount */
    0x0, 	/* pad1 */
    &(timerMoreRDPCMDS[0]),
    gsDPClearOtherMode(),				/* rdpOthermode */
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

void SelfScaleTimerBar(void)
{
  /* self scaling counter stuff, take out if it's too obnoxious */
  
  if ((1.0/TIMERUNDERFLOW) * TimePerFrame / TIMERINTERVAL + 1.0 < TimerTicks)
    while (((1.0/TIMERUNDERFLOW) * TimePerFrame / TIMERINTERVAL + 1.0 < TimerTicks) &&
	   (TimerTicks != MINTIMERTICKS))
      TimerTicks-=2;
  else if ((1.0/TIMEROVERFLOW) * TimePerFrame / TIMERINTERVAL + 1.0 > TimerTicks)
    while (((1.0/TIMEROVERFLOW) * TimePerFrame / TIMERINTERVAL + 1.0 > TimerTicks) &&
	   (TimerTicks != MAXTIMERTICKS))
      TimerTicks+=2;
}

void WriteTimerBar(void)
{
    float TickSpacing, BarPercent;
    int i;
    Gfx	*gptr;
    
    if (SelfScaleTimer) SelfScaleTimerBar();
    
    TickSpacing = (TIMERMAXX - TIMERMINX) / (TimerTicks - 1.0);
    
    gtStateSetOthermode(&(timerObj.sp.rdpOthermode), GT_RENDERMODE,
			(G_RM_OPA_SURF | G_RM_OPA_SURF2));
    gtStateSetOthermode(&(timerObj.sp.rdpOthermode), GT_CYCLETYPE, 
			G_CYC_FILL);
    gptr = (Gfx *) &(timerObj.sp.rdpCmds[0]);
    
    
    BarPercent = TimePerFrame / (TIMERINTERVAL * (TimerTicks - 1));
    
    if (BarPercent < 1.0) {
	/* Within bar counter */
	gDPSetFillColor(gptr++, 
			GPACK_RGBA5551(0,127,0,1) << 16 | 
			GPACK_RGBA5551(0,127,0,1));
	gDPFillRectangle(gptr++, 
			 TIMERMINX, 
			 TIMERMINY, 
			 TIMERMINX + (int)((TIMERMAXX - TIMERMINX)*BarPercent), 
			 TIMERMAXY);      
    } else {
	/* Overflowed bar counter range */
	gDPSetFillColor(gptr++, 
			GPACK_RGBA5551(127,0,0,1) << 16 | 
			GPACK_RGBA5551(127,0,0,1));
	gDPFillRectangle(gptr++, 
			 TIMERMINX, TIMERMINY, 
			 TIMERMAXX, TIMERMAXY);
    }
    
    gDPSetFillColor(gptr++, 
		    GPACK_RGBA5551(0,0,0,1) << 16 | 
		    GPACK_RGBA5551(0,0,0,1));
    
    for (i=0; i<TimerTicks; i++) {      
	gDPFillRectangle(gptr++, 
			 TIMERMINX + (int) TickSpacing*i, TIMERMINY, 
			 TIMERMINX + (int) TickSpacing*i, TIMERMAXY);
    }
    
    
    gDPPipeSync(gptr++);
    gDPEndDisplayList(gptr++);
    
    /* timer state: */
    gtlistp->obj.gstatep = ggsp;
    gtlistp->obj.statep = &timerObj;
    gtlistp->obj.vtxp = (Vtx *) NULL;
    gtlistp->obj.trip = (gtTriN *) NULL;
    gtlistp++;
}
