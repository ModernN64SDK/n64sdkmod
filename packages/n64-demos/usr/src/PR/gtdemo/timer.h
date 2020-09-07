
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
 * File:	timer.h
 * Creator:	hsa@sgi.com
 * Create Date:	Tue Nov  7 14:27:36 PST 1995
 *
 */

#define TIMERHALFHT	5
#define TIMERHT		10
#define TIMERMINX       40
#define TIMERMAXX       280
#define TIMERMINY       200
#define TIMERMIDY       (TIMERMINY+TIMERHALFHT)
#define TIMERMAXY       (TIMERMINY+TIMERHT)

#define TIMERINTERVAL   16.67

/* Keep these as odd numbers */
#define MINTIMERTICKS       3
#define MAXTIMERTICKS      11

#define TIMERUNDERFLOW   0.10
#define TIMEROVERFLOW    0.90

extern OSTime		bg_cpu_start, bg_cpu_end, fg_cpu_start, fg_cpu_end;
extern OSTime		bg_rcp_start, bg_rcp_end, fg_rcp_start, fg_rcp_end;

extern void	TimerInit(void);
extern void	WriteTimerBar(Dynamic *dynamicp, gtGfx	**gtlistp);
extern void	TimerFinish(void);

