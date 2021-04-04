/*======================================================================*/
/*		NuSYS										*/
/*		nucontdatagetexall.c							*/
/*												*/
/*		Copyright (C) 1997, NINTENDO Co,Ltd.				*/
/*												*/
/*----------------------------------------------------------------------*/    
/* Ver 1.0	97/10/9		Created by Kensaku Ohki(SLANP)		*/
/*======================================================================*/
/* $Id: nucontdatagetexall.c,v 1.2 1997/12/10 08:29:59 ohki Exp $		*/
/*======================================================================*/
#include <nusys.h>


/*----------------------------------------------------------------------*/
/*	nuContDataGetExAll - Get controller data 					*/
/*  Read extended controller data 							*/
/*  Read with adding trigger data to the OSContPad type of N64OS 		*/
/*	IN:	*contpad controller structure pointer of NUContData type 	*/
/*----------------------------------------------------------------------*/
void nuContDataGetExAll(NUContData *contdata)
{
    u16	button;
    u32	cnt;
    
    nuContDataClose();	/* data close */    
    for(cnt =0; cnt < NU_CONT_MAXCONTROLLERS; cnt++){
	button = contdata[cnt].button;
	bcopy(&nuContData[cnt], &contdata[cnt], sizeof(OSContPad));
	
	/* Create trigger data */
	contdata[cnt].trigger = nuContData[cnt].button & (~button);
    }
    nuContDataOpen();	/* data open */
}
