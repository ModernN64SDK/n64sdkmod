/*======================================================================*/
/*		NuSYS							*/
/*		nuvrsopen.c						*/
/*									*/
/*		Copyright (C) 1999, NINTENDO Co,Ltd.			*/
/*									*/
/*======================================================================*/
/* $Id: nuvrssetword.c,v 1.1 1999/01/23 05:52:09 ohki Exp $	*/
/*======================================================================*/
#include <nusys.h>

/*----------------------------------------------------------------------*/
/*	nuVrsSetWord - Registers terms in the voice recognition system dictionary		*/
/*									*/
/*	Registers terms in the voice recognition system dictionary.			*/
/*									*/
/*	IN:	handle	Pointer to structure of type NUVrsHandle		*/
/*		word	Term to register					*/
/*	RET:	error							*/
/*----------------------------------------------------------------------*/
s32 nuVrsSetWord(NUVrsHandle* handle, u8* word)
{
    NUVrsMesg	mesg;
    
    mesg.vrsHandle = handle;
    mesg.data[0] = (u32)word;
    return nuSiSendMesg(NU_VRS_SET_WORD_MSG, (void*)&mesg);
}
