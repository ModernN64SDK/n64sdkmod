/*======================================================================*/
/*		NuSYS							*/
/*		nuauseqplayerinit.c					*/
/*									*/
/*		Copyright (C) 1997, NINTENDO Co,Ltd.			*/
/*									*/
/*======================================================================*/
/* $Id: nuauseqplayerinit.c,v 1.2 1998/12/24 09:06:46 ohki Exp $	*/
/*======================================================================*/
#include <nusys.h>
#include <nualsgi.h>

/*----------------------------------------------------------------------*/
/*	nuAuSeqPlayerInit - Initialize sequence player 				*/
/*	IN:	seqpconfig 	  sequence player structure				*/
/*		size	        sequence buffer						*/
/*		player_no     number for sequence player				*/
/*					NU_AU_SEQ_PLAYER0 or NU_AU_SEQ_PLAYER1	*/
/*	RET:	None										*/
/*----------------------------------------------------------------------*/
void nuAuSeqPlayerInit(ALSeqpConfig* seqpconfig, u32 size, u32 player_no)
{
    seqpconfig->heap = &nuAuHeap;
    nuAuSeqPlayer[player_no].data_ptr = nuAuHeapAlloc(size);
    alCSPNew(&nuAuSeqPlayer[player_no].player, seqpconfig);
}
