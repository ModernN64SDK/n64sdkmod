/*======================================================================*/
/*		NuSYS							*/
/*		nuauseqplayerseqset.c					*/
/*									*/
/*		Copyright (C) 1997, NINTENDO Co,Ltd.			*/
/*									*/
/*======================================================================*/
/* $Id: nuauseqplayerseqset.c,v 1.2 1998/12/24 09:11:14 ohki Exp $	*/
/*======================================================================*/

#include <nusys.h>
#include <nualsgi.h>

/*----------------------------------------------------------------------*/
/*	nuAuSeqPlayerSeqSet - Initialize audio 					*/
/*	IN:	seq_addr	address where bank file is stored			*/
/*	RET:	None										*/
/*----------------------------------------------------------------------*/
void nuAuSeqPlayerSeqSet(u8* seq_addr)
{
    u8		data[32];
    ALSeqFile*	seqFile_ptr;
    s32		seqFileSize;

    seqFile_ptr = OS_DCACHE_ROUNDUP_ADDR(data);
    nuPiReadRom((u32)seq_addr, seqFile_ptr, 4);

    seqFileSize = 4 + seqFile_ptr->seqCount * sizeof(ALSeqData);
    nuAuSeqFile_ptr = nuAuHeapAlloc(seqFileSize);
    nuPiReadRom((u32)seq_addr, nuAuSeqFile_ptr, seqFileSize);

    alSeqFileNew(nuAuSeqFile_ptr, seq_addr);
	
}
