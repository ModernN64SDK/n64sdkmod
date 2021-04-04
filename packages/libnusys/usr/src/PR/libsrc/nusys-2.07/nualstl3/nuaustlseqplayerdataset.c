/*======================================================================*/
/*		NuSYS							*/
/*		nuaustlseqplayerdataset.c				*/
/*									*/
/*		Copyright (C) 1997, NINTENDO Co,Ltd.			*/
/*									*/
/*======================================================================*/
/* $Id: nuaustlseqplayerdataset.c,v 1.1 1999/01/07 12:32:26 ohki Exp $	*/
/*======================================================================*/
#include <nusys.h>
#include <nualstl.h>

/*----------------------------------------------------------------------*/
/*	nuAuStlSeqPlayerDataSet -Read sequence of sequence player	*/
/*									*/
/*	Read sequence data (.bin)					*/
/*									*/
/*	IN:	player_no	Sequence player number			*/
/*		seq_addr	ROM address of sequence data		*/
/*		seq_size	sequence data size			*/
/*	RET:	None							*/
/*----------------------------------------------------------------------*/
void nuAuStlSeqPlayerDataSet(u32 player_no, u8 *seq_addr, u32 seq_size)
{
    /* If size is odd, it can't be transferred by PI, so make it even */
    seq_size = (seq_size + 1) & ~0x1;
    
    nuPiReadRom((u32)seq_addr, (void*)nuAuSeqPlayer[player_no].data_ptr, seq_size);
}
