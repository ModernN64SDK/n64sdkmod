/*======================================================================*/
/*		NuSYS							*/
/*		nucontrmbstart.c					*/
/*									*/
/*		Copyright (C) 1997, NINTENDO Co,Ltd.			*/
/*									*/
/*----------------------------------------------------------------------*/    
/* Ver 1.0	97/10/9		Created by Kensaku Ohki(SLANP)		*/
/*======================================================================*/
/* $Id: nucontrmbstart.c,v 1.8 1999/01/21 07:21:25 ohki Exp $	*/
/*======================================================================*/
#include <nusys.h>

/*----------------------------------------------------------------------*/
/*	nuContRmbStart - The rumble pak starts to rumble		*/
/*	IN:	contNo	The controller number				*/
/*		freq	The rumble number(1-256)			*/
/*			One pulse for 256 frames occures with 256.	*/
/*			The consecurive action occures with 256.	*/
/*		frame	The rumble time (the frame number)		*/
/*	RET:	None							*/
/*----------------------------------------------------------------------*/
void nuContRmbStart(u32 contNo, u16 freq, u16 frame)
{
    NUContRmbCtl 	rmbCtl;
    NUContRmbMesg	startMesg;
    

    /* Check if it is the forced stop mode */

	if( nuContRmbCtl[contNo].mode & NU_CONT_RMB_MODE_PAUSE){
	return;
    }

    rmbCtl.freq  = freq;
    rmbCtl.frame = frame;
    rmbCtl.state = NU_CONT_RMB_STATE_RUN;
    startMesg.contNo   = contNo;
    startMesg.data     = &rmbCtl;
    nuSiSendMesg(NU_CONT_RMB_START_MSG, (void*)&startMesg); 
}
    