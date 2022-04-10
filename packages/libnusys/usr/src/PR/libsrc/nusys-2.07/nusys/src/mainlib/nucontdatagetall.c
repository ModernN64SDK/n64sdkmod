/*======================================================================*/
/*		NuSYS										*/
/*		nucontdatagetall.c							*/
/*												*/
/*		Copyright (C) 1997, NINTENDO Co,Ltd.				*/
/*												*/
/*----------------------------------------------------------------------*/    
/* Ver 1.1	97/12/10	Created by Kensaku Ohki(SLANP)			*/
/*======================================================================*/
/* $Id: nucontdatagetall.c,v 1.2 1997/12/10 08:28:55 ohki Exp $		*/
/*======================================================================*/
#include <nusys.h>


/*----------------------------------------------------------------------*/
/*	nuContDataGetAll - Acquire controller data				*/
/*  read controller data								*/
/*  read all the four by OSContPad type of N64OS 				*/
/*	IN:	*Pointer of controller structure of contpad OSContData type */
/* 		padNo  controller number 						*/
/*----------------------------------------------------------------------*/
void nuContDataGetAll(OSContPad *contdata)
{
    nuContDataClose();	/* data close */

    /* copy of data */
    bcopy(&nuContData[0], contdata, sizeof(OSContPad)*MAXCONTROLLERS);
    
    nuContDataOpen();	/* data open */
}
