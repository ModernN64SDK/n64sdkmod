/*======================================================================*/
/*		NuSYS							*/
/*		nuaustlplayerinit.c					*/
/*									*/
/*		Copyright (C) 1997, NINTENDO Co,Ltd.			*/
/*									*/
/*======================================================================*/
/* $Id: nuaustlseqplayerinit.c,v 1.1 1999/01/07 12:32:35 ohki Exp $	*/
/*======================================================================*/
#include <nusys.h>
#include <nualstl.h>

NUAuSeqPlayer		nuAuSeqPlayer[NU_AU_SEQPLAYER_NUM];

/*----------------------------------------------------------------------*/
/*	nuAuStlSeqPlayerInit - Initialize the Sequence Player.		*/
/*									*/
/*	IN:	player_no		Sequence Player number		*/
/*		size		Size of buffer for storing sequence data	*/
/*	RET:	nothing							*/
/*----------------------------------------------------------------------*/
void nuAuStlSeqPlayerInit(u32 player_no, u32 size)
{
    nuAuSeqPlayer[player_no].data_ptr = nuAuStlHeapAlloc(size);
}
