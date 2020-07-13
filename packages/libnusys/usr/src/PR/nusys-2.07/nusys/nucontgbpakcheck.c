/*======================================================================*/
/*		NuSYS							*/
/*		nucontgbpakcheck.c					*/
/*									*/
/*		Copyright (C) 1997, NINTENDO Co,Ltd.			*/
/*									*/
/*----------------------------------------------------------------------*/    
/* Ver 1.2	98/06/08		Created by Kensaku Ohki(SLANP)	*/
/*======================================================================*/
/* $Id: nucontgbpakcheck.c,v 1.4 1999/01/21 07:15:13 ohki Exp $ */
/*======================================================================*/
#include <nusys.h>

/*----------------------------------------------------------------------*/
/*	nuContGBPakCheckConnector - Detects poor connection.            */
/*	  Detects  poor contact of the GB Game Pak connector.           */
/*	  Accesses GB Game Pak's special ROM and RAM areas to verify    */
/*	operation of all address lines.               		 	*/
/*	IN:	handle	64GB Pak handler.          			*/
/*		status	Status	              				*/
/*	RET:	Value returned from osGbpakCheckConnector()		*/
/*----------------------------------------------------------------------*/
s32 nuContGBPakCheckConnector(NUContPakFile* handle, u8* status)
{
    NUContGBPakMesg 	gbpakMesg;

    gbpakMesg.handle   = handle;
    gbpakMesg.data[0]  = (u32)status;
    handle->error = nuSiSendMesg(NU_CONT_GBPAK_CHECKCONNECTOR_MSG, (void*)&gbpakMesg);

    return handle->error;
}
