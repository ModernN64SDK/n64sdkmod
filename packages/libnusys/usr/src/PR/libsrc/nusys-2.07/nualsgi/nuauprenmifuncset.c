/*======================================================================*/
/*		NuSYS										*/
/*		nuauprenmifuncset.c							*/
/*												*/
/*		Copyright (C) 1997, NINTENDO Co,Ltd.				*/
/*												*/
/*======================================================================*/
/* $Id: nuauprenmifuncset.c,v 1.2 1998/12/24 09:02:19 ohki Exp $	*/
/*======================================================================*/
#include <nusys.h>
#include <nualsgi.h>

/*----------------------------------------------------------------------*/
/*	nuAuPreNMIFuncSet - Register PRE NMI processing callback function	*/
/*	IN:	pointer of callback function						*/
/*	RET:	None										*/
/*----------------------------------------------------------------------*/
void nuAuPreNMIFuncSet(NUAuPreNMIFunc func)
{
    OSIntMask	mask;
    
    mask = osSetIntMask(OS_IM_NONE);		/* Interuption not allowed */
    nuAuPreNMIFunc = func;
    osSetIntMask(mask);
}
