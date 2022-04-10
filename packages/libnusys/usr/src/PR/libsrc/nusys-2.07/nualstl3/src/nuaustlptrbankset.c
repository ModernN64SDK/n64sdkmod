/*======================================================================*/
/*		NuSYS							*/
/*		nuaustlptrbankset.c					*/
/*									*/
/*		Copyright (C) 1997, NINTENDO Co,Ltd.			*/
/*									*/
/*======================================================================*/
/* $Id: nuaustlptrbankset.c,v 1.1 1999/01/07 12:30:02 ohki Exp $		*/
/*======================================================================*/
#include <nusys.h>
#include <nualstl.h>

/*----------------------------------------------------------------------*/
/*	nuAuStlPtrBankSet - Set the sample bank.			*/
/*									*/
/*	Transfers the sample pointer bank (.ptr) to memory and initialize it. */
/*	IN:	pbk_addr	 	sample pointer bank ROM address	*/
/*		pbk_size		size of the sample pointer bank		*/
/*		wbk_addr		sample pointer bank ROM address	*/
/*	RET:	nothing							*/
/*----------------------------------------------------------------------*/
void nuAuStlPtrBankSet(u8* pbk_addr, u32 pbk_size, u8* wbk_addr)
{
    /*  PI transfer cannot be performed if size is an odd number so make it even. */    
    pbk_size = (pbk_size + 1) & ~0x1;

    nuPiReadRom((u32)pbk_addr, nuAuPtrBank_ptr, pbk_size);
    MusPtrBankInitialize(nuAuPtrBank_ptr, wbk_addr);
}
