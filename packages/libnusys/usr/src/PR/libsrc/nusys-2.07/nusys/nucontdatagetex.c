/*======================================================================*/
/*		NuSYS										*/
/*		nucontdatagetex.c								*/
/*												*/
/*		Copyright (C) 1997, NINTENDO Co,Ltd.				*/
/*												*/
/*----------------------------------------------------------------------*/    
/* Ver 1.0	97/10/9		Created by Kensaku Ohki(SLANP)		*/
/*======================================================================*/
/* $Id: nucontdatagetex.c,v 1.2 1997/12/10 08:29:37 ohki Exp $		*/
/*======================================================================*/
#include <nusys.h>


/*----------------------------------------------------------------------*/
/*	nuContDataGetEx - Get the controller data 				*/
/*  Read the extended controller data 						*/
/*  Read with adding the trigger data to the OSContPad type of N64OS 	*/
/*	IN:	*contpad  controller structure pointer of NUContData type 	*/
/* 		padNo     The controller number 					*/
/*----------------------------------------------------------------------*/
void nuContDataGetEx(NUContData *contdata, u32 padno)
{
    u16	button;

    button = contdata->button;
    nuContDataClose();	/* data close */    
    bcopy(&nuContData[padno], contdata, sizeof(OSContPad));

    /* Create the trigger data */
    contdata->trigger = nuContData[padno].button & (~button);
    nuContDataOpen();	/* data open */
}
