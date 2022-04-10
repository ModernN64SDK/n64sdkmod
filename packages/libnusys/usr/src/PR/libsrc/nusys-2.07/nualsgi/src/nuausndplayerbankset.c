/*======================================================================*/
/*		NuSYS							*/
/*		nuausndplayerbankset.c					*/
/*									*/
/*		Copyright (C) 1997, NINTENDO Co,Ltd.			*/
/*======================================================================*/
/* $Id: nuausndplayerbankset.c,v 1.2 1998/12/24 10:52:59 ohki Exp $	*/
/*======================================================================*/

#include <nusys.h>
#include <nualsgi.h>

/*----------------------------------------------------------------------*/
/*	nuAuSndqPlayerBankSet - Set Bank				*/
/*	IN:	bank_addr	address where bank file is stored	*/
/*		bank_size 	bank file size				*/
/*		table_addr	head address of ROM of wave table file	*/
/*	RET:	None							*/
/*----------------------------------------------------------------------*/
void nuAuSndPlayerBankSet(u8* bank_addr, u32 bank_size, u8* table_addr)
{
    int i;

    /* Reserve buffer for sound bank and read bank file */
    nuAuSndBank_ptr = nuAuHeapAlloc(bank_size);
    nuPiReadRom((u32)bank_addr, nuAuSndBank_ptr, bank_size);

    alBnkfNew(nuAuSndBank_ptr, table_addr);

    /* Provide array of max number of sounds and initialize with -1, meaning not used */
    nuAuSndId = nuAuHeapAlloc(nuAuSndPlayer.maxSounds * sizeof(ALSndId));
    for(i = 0; i < nuAuSndPlayer.maxSounds; i++)
      nuAuSndId[i] = -1;
}
