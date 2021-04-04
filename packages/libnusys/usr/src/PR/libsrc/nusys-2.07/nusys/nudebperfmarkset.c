/*======================================================================*/
/*		NuSYS							*/
/*		nudebperfmarker.c					*/
/*									*/
/*		Copyright (C) 1999, NINTENDO Co,Ltd.			*/
/*									*/
/*======================================================================*/
/* $Id: nudebperfmarkset.c,v 1.2 1999/06/10 04:30:08 ohki Exp $	*/
/*======================================================================*/
#ifdef NU_DEBUG
#include <nusys.h>


/*--------------------------------------------------------------------*/
/*	nuDebPerfMarkSet      Sets a mark                               */
/*                                                                    */
/*     Indicates on the performance meter the location of a           */
/*   process at the time this function is called.                     */
/*     To actually show the mark on the screen, you must call either  */
/*    nuDebTaskPerfBar0 or nuDebTaskPerfbBar1 after calling this      */
/*    function                                                        */
/*                                                                    */
/*	IN:	markNo   The mark number (specify 0 - 9)                  */
/*	RET:	The time that has passed since the retrace                */
/*----------------------------------------------------------------------*/
u32 nuDebPerfMarkSet(s32 markNo)
{

    
    if(markNo >= NU_DEB_MARKER_NUM){
	return 0;
    }
    
    /* Check value */
    if(nuDebTaskPerfEnd == NU_DEB_PERF_RUNNING){
       OSIntMask mask;
       
       mask = osSetIntMask(OS_IM_NONE);
       nuDebTaskPerf[nuDebTaskPerfCnt].markerTime[markNo] = OS_CYCLES_TO_USEC(osGetTime());
       osSetIntMask(mask);
    }
    
    return( nuDebTaskPerf[nuDebTaskPerfCnt].markerTime[markNo]
	- nuDebTaskPerf[nuDebTaskPerfCnt].retraceTime);
}
#endif	/* NU_DEBUG */
