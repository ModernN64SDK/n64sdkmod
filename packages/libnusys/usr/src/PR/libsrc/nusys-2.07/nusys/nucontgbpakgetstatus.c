/*======================================================================*/
/*		NuSYS				               		*/
/*		nucontgbpakgetstate.c			                */
/*								        */
/*		Copyright (C) 1997, NINTENDO Co,Ltd.	          	*/
/*									*/
/*----------------------------------------------------------------------*/    
/* Ver 1.2	98/06/08		Created by Kensaku Ohki(SLANP)        	*/
/*======================================================================*/
/* $Id: nucontgbpakgetstatus.c,v 1.4 1999/01/21 07:16:55 ohki Exp $*/
/*======================================================================*/
#include <nusys.h>

/*----------------------------------------------------------------------*/
/*	nuContGBPakGetState - Get the 64GB Pak status.          	*/
/* 	Gets (detects) the status of the 64GB Pak. 	        	*/
/*	IN:	handle	64GB Pak handler. 		        	*/
/*		status	Buffer that stores the status. 			*/
/*	RET:	Value returned by osGbpakGetStatus(). 			*/
/*----------------------------------------------------------------------*/
s32 nuContGBPakGetStatus(NUContPakFile* handle, u8* status)
{
     NUContGBPakMesg 	gbpakMesg;

    gbpakMesg.handle   = handle;
    gbpakMesg.data[0]  = (u32)status;
    handle->error = nuSiSendMesg(NU_CONT_GBPAK_STATUS_MSG, (void*)&gbpakMesg);
    return handle->error;
}
