/*======================================================================*/
/*		NuSYS							*/
/*		nuauprenmifuncset.c					*/
/*									*/
/*		Copyright (C) 1997, NINTENDO Co,Ltd.			*/
/*									*/
/*======================================================================*/
/* $Id: nuauprenmifuncset.c,v 1.1 1999/01/07 12:28:47 ohki Exp $	*/
/*======================================================================*/
#include <nusys.h>
#include <nualstl.h>

/*----------------------------------------------------------------------*/
/*	nuAuPreNMIFuncSet - Register PRE NMI processing callback function	*/
/*	IN:	pointer of callback function						*/
/*	RET:	None										*/
/*----------------------------------------------------------------------*/
void nuAuPreNMIFuncSet(NUAuPreNMIFunc func)
{
    OSIntMask	mask;
    
    mask = osSetIntMask(OS_IM_NONE);		/* Interruption not allowed */
    nuAuPreNMIFunc = func;
    osSetIntMask(mask);
}
