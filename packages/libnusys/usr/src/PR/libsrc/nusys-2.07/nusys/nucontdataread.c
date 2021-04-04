/*======================================================================*/
/*		NuSYS							*/
/*		nucontmgr.c						*/
/*									*/
/*		Copyright (C) 1997, NINTENDO Co,Ltd.			*/
/*									*/
/*----------------------------------------------------------------------*/
/* $Id: nucontdataread.c,v 1.4 1999/03/24 11:35:12 ohki Exp $		*/
/*======================================================================*/
#include <nusys.h>

/*----------------------------------------------------------------------*/
/*	nuContDataRead - Read data of the controller 			*/
/*	Send the message of data reading to the controller manager and 	*/
/*	wait for the reading end.  Note that it may take about 2ms.  	*/
/*									*/
/*	IN:	*pad	The reading structure of controller data	*/
/*	RET:	None 							*/
/*----------------------------------------------------------------------*/
void nuContDataRead(OSContPad* pad)
{
    nuSiSendMesg(NU_CONT_READ_MSG, pad);
}
