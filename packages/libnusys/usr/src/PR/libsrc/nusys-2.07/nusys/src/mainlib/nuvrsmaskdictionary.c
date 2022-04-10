/*======================================================================*/
/*		NuSYS							*/
/*		nuvrsmaskdictionary.c					*/
/*									*/
/*		Copyright (C) 1999, NINTENDO Co,Ltd.			*/
/*									*/
/*======================================================================*/
/* $Id: nuvrsmaskdictionary.c,v 1.1 1999/01/23 05:51:38 ohki Exp $	*/
/*======================================================================*/
#include <nusys.h>

/*----------------------------------------------------------------------*/
/*	nuVrsMaskDictionary - Toggles on/off whether the terms registered */
/*	in the 	dictionary are targets for recognition.	*/
/*	Determines whether the terms registered in the dictionary
	are target terms for voice recognition .				*/
/*									*/
/*	IN:	handle		Structure of type NUVrsHandle	*/
/*		maskpattern	Mask all terms			*/
/*		size		Number of bytes in maskpattern	*/
/*	RET:	error							*/
/*----------------------------------------------------------------------*/
s32 nuVrsMaskDictionary(NUVrsHandle* handle, u8* maskpattern, s32 size)
{
    NUVrsMesg	mesg;
    
    mesg.vrsHandle = handle;
    mesg.data[0] = (s32)maskpattern;
    mesg.data[1] = size;
    return nuSiSendMesg(NU_VRS_MASK_DICTIONARY_MSG, (void*)&mesg);
}
