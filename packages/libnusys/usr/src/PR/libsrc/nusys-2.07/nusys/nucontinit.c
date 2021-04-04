/*======================================================================*/
/*		NuSYS							*/
/*		nucontinit.c						*/
/*									*/
/*		Copyright (C) 1997, NINTENDO Co,Ltd.			*/
/*									*/
/*----------------------------------------------------------------------*/    
/* Ver 1.0	97/10/9		Created by Kensaku Ohki(SLANP)		*/
/*======================================================================*/
/* $Id: nucontinit.c,v 1.3 1998/07/07 12:35:42 ohki Exp $		*/
/*======================================================================*/
#include <nusys.h>

/*----------------------------------------------------------------------*/
/*	nuContInit - Initialize the controller manager 				*/
/*	IN:	None 										*/
/*	RET:	The bit pattern of the connected controller 			*/
/*----------------------------------------------------------------------*/
u8 nuContInit(void)
{
    u8 pattern;

    /* Initialize and start SI manager */
    pattern = nuSiMgrInit();

    /* Initialize and register Controller manager */
    nuContMgrInit();

    /* Initialize and register the controller pack manager */
    nuContPakMgrInit();
    
    /* Initialize and register the oscillating pack manager */
    nuContRmbMgrInit();

    return pattern;
}
