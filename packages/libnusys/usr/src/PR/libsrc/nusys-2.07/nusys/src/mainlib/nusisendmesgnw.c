/*======================================================================*/
/*		NuSYS							*/
/*		nusisendmesgnw.c					*/
/*									*/
/*		Copyright (C) 1999, NINTENDO Co,Ltd.			*/
/*									*/
/*									*/
/*======================================================================*/
/* $Id: nusisendmesgnw.c,v 1.2 1999/01/26 02:39:34 ohki Exp $		*/
/*======================================================================*/
#include <nusys.h>

/*----------------------------------------------------------------------*/
/*	nuSiSendMesgNW -  Sends message to the SI Manager (asynchronous) */
/*									*/
/*	Sends the message specified by mesg to the SI Manager. 
 	Does not wait for message to be returned in rtnMesg.   
	Thus, synchronization must be achieved by the application
	independently.				*/
/*									*/
/*	IN:	mesg		Message to be sent to the SI Manager	*/
/*		dataPtr		Pointer to the data sent to			*/
/*				the callback function or returned by it. 		*/
/*	RTN:	nothing							*/
/*----------------------------------------------------------------------*/
void nuSiSendMesgNW(NUScMsg mesg, void* dataPtr)
{
    NUSiCommonMesg	siCommonMesg;
    siCommonMesg.mesg     = mesg;
    siCommonMesg.dataPtr  = dataPtr;
    siCommonMesg.rtnMesgQ = NULL;
    osSendMesg(&nuSiMgrMesgQ, (OSMesg*)&siCommonMesg, OS_MESG_BLOCK);
}

