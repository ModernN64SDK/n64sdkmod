/*======================================================================*/
/*		NuSYS				                   	*/
/*		nucontgbpakopen.c			               	*/
/*							                */
/*		Copyright (C) 1997, NINTENDO Co,Ltd.	         	*/
/*								        */
/*----------------------------------------------------------------------*/    
/* Ver 1.2	98/06/08		Created by Kensaku Ohki(SLANP)  */
/*======================================================================*/
/* $Id: nucontgbpakopen.c,v 1.4 1999/01/21 07:16:39 ohki Exp $	*/ 
/*======================================================================*/
#include <nusys.h>

/*----------------------------------------------------------------------*/
/*	nuContGBPakOpen - Check for 64GB Pak. 		      		*/
/* 	Checks whether a 64GB Pak has been inserted.  Other    		*/
/*	types of Paks are not detected. 	                	*/
/*	IN:	contNo	Controller number. 		      		*/
/*	RET:	Value returned by osGbpakInit(). 			*/
/*----------------------------------------------------------------------*/
s32 nuContGBPakOpen(NUContPakFile* handle, s32 contNo)
{
        NUContGBPakMesg 	gbpakMesg;

    handle->pfs = &nuContPfs[contNo];
    handle->pfs->channel = contNo;
    
    gbpakMesg.handle   = handle;
    handle->error =nuSiSendMesg(NU_CONT_GBPAK_OPEN_MSG, (void*)&gbpakMesg);
    return handle->error;
}
