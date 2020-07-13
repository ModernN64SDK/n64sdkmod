/*======================================================================*/
/*		NuSYS							*/
/*		nuvrscontrolgain.c					*/
/*									*/
/*		Copyright (C) 1999, NINTENDO Co,Ltd.			*/
/*									*/
/*======================================================================*/
/* $Id: nuvrscontrolgain.c,v 1.1 1999/01/23 05:51:11 ohki Exp $	*/
/*======================================================================*/
#include <nusys.h>

/*----------------------------------------------------------------------*/
/*	nuVrsControlGain - Adjusts input gain for voice recognition system	*/
/*									*/
/*	Adjusts the gain of voice input to the voice recognition system.	*/
/*	IN:	handle	Structure of type NUVrsHandle				*/
/*		analog	Analog gain of transmission system			*/
/*		digital	Digital gain of transmission system					*/
/*	RET:	Error							*/
/*----------------------------------------------------------------------*/
s32 nuVrsControlGain(NUVrsHandle* handle, s32 analog, s32 digital)
{
    NUVrsMesg	mesg;
    
    mesg.vrsHandle = handle;
    mesg.data[0] = analog;
    mesg.data[1] = digital;
    return nuSiSendMesg(NU_VRS_CONTROL_GAIN_MSG, (void*)&mesg);

}
