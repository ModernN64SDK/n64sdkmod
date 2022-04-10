/*======================================================================*/
/*		NuSYS							*/
/*		nucontrmbforcestop.c					*/
/*									*/
/*		Copyright (C) 1997, NINTENDO Co,Ltd.			*/
/*									*/
/*----------------------------------------------------------------------*/    
/* Ver 1.0	97/10/9		Created by Kensaku Ohki(SLANP)		*/
/*======================================================================*/
/* $Id: nucontrmbforcestop.c,v 1.6 1999/01/21 07:21:11 ohki Exp $	*/
/*======================================================================*/
#include <nusys.h>

/*----------------------------------------------------------------------*/
/*	nuContRmbForceStop - Force to stop the oscillating pack 	*/
/*	IN:	None 							*/
/*	RET:	None 							*/
/*----------------------------------------------------------------------*/
void nuContRmbForceStop(void)
{
    nuSiSendMesg(NU_CONT_RMB_FORCESTOP_MSG, (void*)NULL);
}
