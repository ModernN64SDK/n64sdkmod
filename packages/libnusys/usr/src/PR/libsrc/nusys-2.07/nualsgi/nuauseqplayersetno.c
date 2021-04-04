/*======================================================================*/
/*		NuSYS							*/
/*		nuauseqplayersetno.c					*/
/*									*/
/*		Copyright (C) 1997, NINTENDO Co,Ltd.			*/
/*									*/
/*======================================================================*/
/* $Id: nuauseqplayersetno.c,v 1.2 1998/12/24 09:11:55 ohki Exp $	*/
/*======================================================================*/

#include <nusys.h>
#include <nualsgi.h>

/*----------------------------------------------------------------------*/
/*	nuAuSeqPlayerSetNo - Read sequence of sequence player			*/
/*	IN:	player_no	   number for sequence player				*/
/*					NU_AU_SEQ_PLAYER0 or NU_AU_SEQ_PLAYER1	*/
/*		seq_no	   sequence number					*/
/*	RET:	None										*/
/*----------------------------------------------------------------------*/
void nuAuSeqPlayerSetNo(u32 player_no ,u32 seq_no)
{
    s32	dataLen;
    u8*	dataOffset;

#ifdef NU_DEBUG
    if(seq_no >=  nuAuSeqFile_ptr->seqCount){
	osSyncPrintf("nuAuSeqPlayerSetNo: seq_no %d is too big.\n", seq_no);
	return;
    }
#endif /* NU_DEBUG */

    nuAuSeqPlayer[player_no].seqno = seq_no;

    /* Get address and size of sequence data from header */
    dataOffset = nuAuSeqFile_ptr->seqArray[seq_no].offset;
    dataLen    = nuAuSeqFile_ptr->seqArray[seq_no].len;

    /* If size is odd number it can't be transferred by PI, so make it an even number */
    if(dataLen & 0x00000001) dataLen++;

    nuPiReadRom((u32)dataOffset, nuAuSeqPlayer[player_no].data_ptr, dataLen);
}
