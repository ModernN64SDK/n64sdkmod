/*======================================================================*/
/*		NuSYS							*/
/*		nuauseqplayerbankset.c					*/
/*									*/
/*		Copyright (C) 1997, NINTENDO Co,Ltd.			*/
/*									*/
/*======================================================================*/
/* $Id: nuauseqplayerbankset.c,v 1.4 1999/06/10 05:04:08 ohki Exp $*/
/*======================================================================*/
#include <nusys.h>
#include <nualsgi.h>

/*----------------------------------------------------------------------*/
/*	nuAuSeqPlayerBankSet - Set Bank					*/
/*	IN:	bank_addr	address where bank file is stored 	*/
/*		bank_size 	bank file size				*/
/*		table_addr	the head address of ROM of wave table file*/
/*	RET:	None							*/
/*----------------------------------------------------------------------*/
void nuAuSeqPlayerBankSet(u8* bank_addr, u32 bank_size, u8* table_addr)
{
    ALBank*	bank_ptr;
    s32		cnt;

    nuAuSeqBank_ptr = nuAuHeapAlloc(bank_size);
    nuPiReadRom((u32)bank_addr, nuAuSeqBank_ptr, bank_size);
    
    alBnkfNew(nuAuSeqBank_ptr, table_addr);
    bank_ptr = nuAuSeqBank_ptr->bankArray[0];

    /* Set the bank in every sequence player               	*/
    /* Check whether data_ptr	is NULL to determine whether 	*/
    /* the sequence player is initialized or not.         	*/
    for(cnt = 0; cnt < 2; cnt++){
	if(nuAuSeqPlayer[cnt].data_ptr != NULL){
	    alCSPSetBank(&nuAuSeqPlayer[cnt].player, bank_ptr);
	}
    }
}
