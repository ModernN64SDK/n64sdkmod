
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

#include "fill.h"
#include "static.h"
#include "timer.h"

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

  if (SelfScaleTimer) SelfScaleTimerBar();

  TickSpacing = (TIMERMAXX - TIMERMINX) / (TimerTicks - 1.0);
  
  gDPPipeSync(glistp++);
  gDPSetCycleType(glistp++, G_CYC_FILL);

  gDPSetRenderMode(glistp++, G_RM_OPA_SURF, G_RM_OPA_SURF2);

  BarPercent = TimePerFrame / (TIMERINTERVAL * (TimerTicks - 1));

  if (BarPercent < 1.0)
    {
      /* Within bar counter */

      gDPSetFillColor(glistp++, 
		      GPACK_RGBA5551(0,127,0,1) << 16 | 
		      GPACK_RGBA5551(0,127,0,1));
      
      
      gDPFillRectangle(glistp++, 
		       TIMERMINX, 
		       TIMERMINY, 
		       TIMERMINX + (int) ((TIMERMAXX - TIMERMINX) * BarPercent), 
		       TIMERMAXY);      
    }
  else
    {
      /* Overflowed bar counter range */
      
      gDPSetFillColor(glistp++, 
		      GPACK_RGBA5551(127,0,0,1) << 16 | 
		      GPACK_RGBA5551(127,0,0,1));
      
      gDPFillRectangle(glistp++, 
		       TIMERMINX, TIMERMINY, 
		       TIMERMAXX, TIMERMAXY);
    }
  
  
  gDPSetFillColor(glistp++, 
		  GPACK_RGBA5551(0,0,0,1) << 16 | 
		  GPACK_RGBA5551(0,0,0,1));
  
  for (i=0; i<TimerTicks; i++)
    {      
      gDPFillRectangle(glistp++, 
		       TIMERMINX + (int) TickSpacing*i, TIMERMINY, 
		       TIMERMINX + (int) TickSpacing*i, TIMERMAXY);
    }

  gDPPipeSync(glistp++);
}
