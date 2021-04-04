/*======================================================================*/
/*		NuSYS				                   	*/
/*		nucontgbpakpower.c		               		*/
/*							                */
/*		Copyright (C) 1997, NINTENDO Co,Ltd.	           	*/
/*								        */
/*----------------------------------------------------------------------*/    
/* Ver 1.2	98/06/08		Created by Kensaku Ohki(SLANP)	*/
/*======================================================================*/
/* $Id: nucontgbpakpower.c,v 1.4 1999/01/21 07:16:33 ohki Exp $	*/
/*======================================================================*/
#include <nusys.h>

/*----------------------------------------------------------------------*/
/*	nuContGBPakPower - Control power to the 64GB Pak. 		*/
/* 	Turns power ON/OFF to the 64GB Pak. 				*/
/*	IN:	handle	64GB Pak handler. 	        		*/
/*		flag	Specifies power ON/OFF. 	        	*/
/*			NU_CONT_GBPAK_POWER_OFF	0x00	            	*/
/*			NU_CONT_GBPAK_POWER_ON	0x01	            	*/
/*			or					        */
/*			OS_GBPAK_POWER_OFF	0x00	               	*/
/*			OS_GBPAK_POWER_ON	0x01		        */
/*	RET:	Value returned by osGbpakPower(). 			*/
/*----------------------------------------------------------------------*/
s32 nuContGBPakPower(NUContPakFile* handle, s32 flag)
{
    NUContGBPakMesg 	gbpakMesg;

    gbpakMesg.handle   = handle;
    gbpakMesg.data[0]  = flag;
    handle->error = nuSiSendMesg(NU_CONT_GBPAK_POWER_MSG, (void*)&gbpakMesg);
    
    return handle->error;
}
