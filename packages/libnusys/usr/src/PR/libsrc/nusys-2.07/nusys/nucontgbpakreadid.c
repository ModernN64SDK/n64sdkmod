/*======================================================================*/
/*		NuSYS					      		*/
/*		nucontgbpakreadid.c		              		*/
/*								        */
/*		Copyright (C) 1997, NINTENDO Co,Ltd.		       	*/
/*								        */
/*----------------------------------------------------------------------*/    
/* Ver 1.2	98/06/08		Created by Kensaku Ohki(SLANP)  */
/*======================================================================*/
/* $Id: nucontgbpakreadid.c,v 1.4 1999/01/21 07:16:29 ohki Exp $	*/
/*======================================================================*/
#include <nusys.h>

/*----------------------------------------------------------------------*/
/*	nuContGBPakReadID - Reads the GB Game Pak ROM registration area.*/
/* 	Reads the ROM registration area of the GB Game Pak inserted     */
/*	in the 64GB Pak and stores the data in the buffer. 		*/
/*	IN:	handle	64GB Pak handler. 			      	*/
/*		id	Buffer for storing the ID area information.     */
/*			(OS_GBPAK_ROM_ID_SIZE byte) 	             	*/
/*		status				                    	*/
/*	RET:	Value returned by osGbpakReadID(). 			*/
/*----------------------------------------------------------------------*/
s32 nuContGBPakReadID(NUContPakFile* handle, OSGbpakId* id, u8* status)
{
    NUContGBPakMesg 	gbpakMesg;

    gbpakMesg.handle   = handle;
    gbpakMesg.data[0]  = (u32)id;
    gbpakMesg.data[1]  = (u32)status;
    handle->error = nuSiSendMesg(NU_CONT_GBPAK_READID_MSG, (void*)&gbpakMesg);
    
    return handle->error;
}
