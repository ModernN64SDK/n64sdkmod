/*======================================================================*/
/*		NuSYS							*/
/*		nuvrscleardictionary.c					*/
/*									*/
/*		Copyright (C) 1999, NINTENDO Co,Ltd.			*/
/*									*/
/*======================================================================*/
/* $Id: nuvrscleardictionary.c,v 1.1 1999/01/23 05:50:59 ohki Exp $	*/
/*======================================================================*/
#include <nusys.h>

/*----------------------------------------------------------------------*/
/*	nuVrsClearDictionary - Initializes vocabulary registration 
	dictionary for the voice recognition system 				*/
/*									*/
/*	Initializes the vocabulary registration dictionary for the voice */
/*	 recognition system.						*/
/*	IN:	handle	Structure of type NUVrsHandle			*/
/*		words	Number of terms to register (1-255)		*/
/*	RET:	Error							*/
/*----------------------------------------------------------------------*/
s32 nuVrsClearDictionary(NUVrsHandle* handle, u8 words)
{
    NUVrsMesg	mesg;
    
    mesg.vrsHandle = handle;
    mesg.data[0] = words;
    return nuSiSendMesg(NU_VRS_CLEAR_DICTIONARY_MSG, (void*)&mesg);
}
