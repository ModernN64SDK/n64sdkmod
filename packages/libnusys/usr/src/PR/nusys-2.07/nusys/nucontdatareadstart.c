/*======================================================================*/
/*		NuSYS							*/
/*		nucontdatareadstart.c					*/
/*									*/
/*		Copyright (C) 1997, NINTENDO Co,Ltd.			*/
/*									*/
/*----------------------------------------------------------------------*/
/* $Id: nucontdatareadstart.c,v 1.6 1999/03/24 11:35:39 ohki Exp $	*/
/*======================================================================*/
#include <nusys.h>

/*----------------------------------------------------------------------*/
/*	nuContDataReadStart - Start to read the controller data 	*/
/*	Send the message of data reading to the controller manager. 	*/
/* 	Because it is different from nuContDataRead and does not wait for*/
/*	the reading end, be sure to check the reading end by 		*/
/* 	nuContDataReadWait.  It has used the wait message queue inside, */
/*	which is the same as "retrace"; thus, you need to be careful of */
/*	using nuContDataReadWait in separate threads. 			*/
/*	IN:	None 							*/
/*	RET:	None 							*/
/*----------------------------------------------------------------------*/

void nuContDataReadStart(void)
{
    static NUSiCommonMesg siCommonMesg = {
	NU_CONT_READ_NW_MSG, NULL
    };
    osSendMesg(&nuSiMgrMesgQ, (OSMesg*)&siCommonMesg, OS_MESG_BLOCK);
}
