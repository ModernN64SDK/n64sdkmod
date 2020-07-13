/*======================================================================*/
/*		NuSYS							*/
/*		nucontdataget.c						*/
/*									*/
/*		Copyright (C) 1997, NINTENDO Co,Ltd.			*/
/*									*/
/*----------------------------------------------------------------------*/    
/* Ver 1.1	97/12/10	Created by Kensaku Ohki(SLANP)		*/
/*======================================================================*/
/* $Id: nucontdataget.c,v 1.2 1997/12/10 08:28:51 ohki Exp $		*/
/*======================================================================*/
#include <nusys.h>


/*----------------------------------------------------------------------*/
/*	nuContDataGet - Acquire controller data					*/
/*  read controller data								*/
/*  read by OSContPad type of N64OS 						*/
/*	IN:	*Pointer of controller structure of contpad OSContData type */
/* 		padNo  controller number						*/
/*----------------------------------------------------------------------*/
void nuContDataGet(OSContPad *contdata, u32 padno)
{

    nuContDataClose();	/* data close */

    /* copy of data */
    bcopy(&nuContData[padno], contdata, sizeof(OSContPad));
    
    nuContDataOpen();	/* data open */
}
