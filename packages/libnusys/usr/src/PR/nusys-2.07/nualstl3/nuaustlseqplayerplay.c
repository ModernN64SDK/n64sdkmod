/*======================================================================*/
/*		NuSYS							*/
/*		nuaustlseqplayerplay.c					*/
/*									*/
/*		Copyright (C) 1997, NINTENDO Co,Ltd.			*/
/*									*/
/*======================================================================*/
/* $Id: nuaustlseqplayerplay.c,v 1.1 1999/01/07 12:32:56 ohki Exp $	*/
/*======================================================================*/
#include <nusys.h>
#include <nualstl.h>

/*----------------------------------------------------------------------*/
/*	nuAuStlSeqPlayerPlay - Plays Sequence Player sequences	*/
/*	IN:	player_no		Sequence Player number		*/
/*	RET:	sequence handler					*/
/*----------------------------------------------------------------------*/
musHandle nuAuStlSeqPlayerPlay(u32 player_no)
{

    nuAuSeqPlayer[player_no].handle = MusStartSong(nuAuSeqPlayer[player_no].data_ptr);

    return nuAuSeqPlayer[player_no].handle;
}

