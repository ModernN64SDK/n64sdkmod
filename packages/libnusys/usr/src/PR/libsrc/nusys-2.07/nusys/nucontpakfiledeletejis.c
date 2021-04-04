/*======================================================================*/
/*		NuSYS							*/
/*		nucontpakfiledeletejis.c				*/
/*									*/
/*		Copyright (C) 1997, NINTENDO Co,Ltd.			*/
/*									*/
/*----------------------------------------------------------------------*/    
/* Ver 1.0	97/10/9		Created by Kensaku Ohki(SLANP)		*/
/*======================================================================*/
/* $Id: nucontpakfiledeletejis.c,v 1.8 1999/01/21 07:17:51 ohki Exp $*/
/*======================================================================*/
#include <nusys.h>

/*----------------------------------------------------------------------*/
/*	nuContPakFileDeleteJis - Delete the controller pack note	*/
/* 	Open the file of the controller pack				*/
/*	IN:	*file	The controller pack structure			*/
/*		*noteName The note name(JIS) The termination-NULL	*/
/*			Converted JIS->N64code				*/
/*		*extName  The extension(JIS) The termination-NULL	*/
/*			Converted JIS->N64code				*/
/*			Converted ASCII->N64code in inside		*/
/*	RET:	Error							*/
/*----------------------------------------------------------------------*/
s32 nuContPakFileDeleteJis(NUContPakFile *file, u8* noteName, u8* extName)
{
    u8			noteNameN64[32];
    u8			extNameN64[4];
    u32			len;

    bzero(noteNameN64, 32);
    bzero(extNameN64, 4);

    len = 0;
    while(noteName[len]) len++;
    
    /* The note name; The conversion of JIS->N64code */
    nuContPakJisToN64(noteName, noteNameN64, len);

    
    /* The extension; The conversion of JIS->N64code */    
    
    nuContPakJisToN64(extName, extNameN64, 1);

    return nuContPakFileDelete(file, noteNameN64, extNameN64);
}
