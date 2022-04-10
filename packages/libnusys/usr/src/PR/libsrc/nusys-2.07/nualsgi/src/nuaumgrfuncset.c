/*======================================================================*/
/*		NuSystem						*/
/*		nuaumgrfuncset.c					*/
/*									*/
/*		Copyright (C) 1998, NINTENDO Co,Ltd.			*
/*									*/
/*	98/11/26	Created  by K.Ohki(SLANP)			*/
/*======================================================================*/
/* $Id: nuaumgrfuncset.c,v 1.3 1998/12/24 08:31:12 ohki Exp $	*/
/*======================================================================*/

#include <nusys.h>
#include <nualsgi.h>

/*----------------------------------------------------------------------*/
/*	nuAuMgrFuncSet - Registers the audio command callback function	*/
/*								*/
/*	Registers a function called in synchrony with audio command creation. 	*/
/*	With audio command creation, the retrace unit or sample number 	*/
/*	 (nuAuFrameSampleSize) is called with the same periodicity 		*/ 
/*	as the playback time.						*/
/*								*/
/*								*/
/*	Sequence and sound control is performed by this callback function. */
/*								*/
/*	IN:	pointer to the callback function				*/
/*	RET:	nothing							*/
/*----------------------------------------------------------------------*/
void nuAuMgrFuncSet(NUAuMgrFunc func)
{
    OSIntMask	mask;
    
    mask = osSetIntMask(OS_IM_NONE);		/* Disable interrupts. */
    nuAuMgrFunc = func;
    osSetIntMask(mask);
}
