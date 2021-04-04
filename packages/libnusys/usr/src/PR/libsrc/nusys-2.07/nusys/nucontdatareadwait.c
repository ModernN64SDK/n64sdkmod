/*======================================================================*/
/*		NuSYS										*/
/*		nucontdatareadwait.c							*/
/*												*/
/*		Copyright (C) 1997, NINTENDO Co,Ltd.				*/
/*												*/
/*----------------------------------------------------------------------*/    
/* Ver 1.0	97/10/9		Created by Kensaku Ohki(SLANP)		*/
/*======================================================================*/
/* $Id: nucontdatareadwait.c,v 1.2 1997/12/10 08:30:18 ohki Exp $		*/
/*======================================================================*/
#include <nusys.h>

/*----------------------------------------------------------------------*/
/*	nuContDataReadWait - Wait for data reading of the controller 	*/
/*	Wait for the reading end by the retrace message or                */ 
/*    nuContDataReadStart.  								*/
/*												*/
/*	IN:	None 										*/
/*	RET:	None										*/
/*----------------------------------------------------------------------*/
void nuContDataReadWait(void)
{
    osRecvMesg(&nuContWaitMesgQ, NULL, OS_MESG_BLOCK);
}
