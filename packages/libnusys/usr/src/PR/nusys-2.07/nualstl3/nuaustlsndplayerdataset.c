/*======================================================================*/
/*		NuSYS							*/
/*		nuaustlsndplayersetdata.c				*/
/*									*/
/*		Copyright (C) 1997, NINTENDO Co,Ltd.			*/
/*									*/
/*======================================================================*/
/* $Id: nuaustlsndplayerdataset.c,v 1.1 1999/01/07 12:33:08 ohki Exp $	*/
/*======================================================================*/
#include <nusys.h>
#include <nualstl.h>

/*----------------------------------------------------------------------*/
/*	nuAuStlSndPlayerDataSet - Reads Sound Player data	*/
/*									*/
/*	Reads the sound (sound effect) bank.				*/
/*									*/
/*	IN:	snd_addr	 	ROM address of the sound effect bank (.bfx) */
/*		snd_size		Data size				*/
/*	RET:	nothing							*/
/*----------------------------------------------------------------------*/
void nuAuStlSndPlayerDataSet(u8* snd_addr, u32 snd_size)
{
    /* PI transfer cannot be performed if size is an odd number so make it even. */
    snd_size = (snd_size + 1) & ~0x1;
    
    nuPiReadRom((u32)snd_addr, (void*)nuAuEffect_ptr, snd_size);
    MusFxBankInitialize((void*)nuAuEffect_ptr);
}
