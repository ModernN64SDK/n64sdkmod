/*======================================================================*/
/*		NuSYS							*/
/*		nuauseqplayersetno.c					*/
/*									*/
/*		Copyright (C) 1997, NINTENDO Co,Ltd.			*/
/*									*/
/*======================================================================*/
/* $Id: nuauseqplayerfadeout.c,v 1.2 1998/12/24 09:06:00 ohki Exp $*/
/*======================================================================*/

#include <nusys.h>
#include <nualsgi.h>


/*----------------------------------------------------------------------*/
/*	nuAuSeqPlayerFadeOut - Fade out						*/
/*	IN:	player_no	Number for sequence player				*/
/*				NU_AU_SEQ_PLAYER0 or NU_AU_SEQ_PLAYER1		*/
/*		speed		fame num							*/
/*	RET:	None										*/
/*----------------------------------------------------------------------*/
void nuAuSeqPlayerFadeOut(u32 player_no ,u32 speed)
{
    s16	vol;

    /* Get current volume */
    vol = nuAuSeqPlayerGetVol(player_no);

    /* If speed is 0, same as stop.  Prevention of divide error */
    if(speed){
	nuAuSeqPlayer[player_no].speed = vol/speed;
	nuAuSeqPlayer[player_no].counter = speed;	    
    } else {
	nuAuSeqPlayer[player_no].speed = vol;
	nuAuSeqPlayer[player_no].counter = 0;
    }
    nuAuSeqPlayer[player_no].mode |= NU_AU_SEQ_MODE_FADEOUT;
}


