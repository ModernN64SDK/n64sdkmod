/*======================================================================*/
/*		NuSYS							*/
/*		nucontrmbforcestopend.c					*/
/*									*/
/*		Copyright (C) 1997, NINTENDO Co,Ltd.			*/
/*									*/
/*----------------------------------------------------------------------*/    
/* Ver 1.0	97/10/9		Created by Kensaku Ohki(SLANP)		*/
/*======================================================================*/
/* $Id: nucontrmbforcestopend.c,v 1.6 1999/01/21 07:21:33 ohki Exp $*/
/*======================================================================*/
#include <nusys.h>

/*----------------------------------------------------------------------*/
/*	nuContRmbForceStopEnd - Cancel forced stop of oscillating pack	*/
/*	IN:	None 							*/
/*	RET:	None 							*/
/*----------------------------------------------------------------------*/
void nuContRmbForceStopEnd(void)
{
    nuSiSendMesg(NU_CONT_RMB_FORCESTOPEND_MSG, (void*)NULL);
}
