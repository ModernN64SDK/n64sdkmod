/*======================================================================*/
/*		NuSYS							*/
/*		nucontpakfileopenjis.c					*/
/*									*/
/*		Copyright (C) 1997, NINTENDO Co,Ltd.			*/
/*									*/
/*----------------------------------------------------------------------*/    
/* Ver 1.0	97/10/9		Created by Kensaku Ohki(SLANP)		*/
/*======================================================================*/
/* $Id: nucontpakfileopenjis.c,v 1.6 1999/01/21 07:20:18 ohki Exp $*/
/*======================================================================*/
#include <nusys.h>

/*----------------------------------------------------------------------*/
/*	nuContPakFileOpenJis - Open the controller pack note		*/
/* 	Open the file of the controller pack				*/
/*	IN:	*file	The controller pack structure			*/
/*		*noteName The note name(JIS) The termination-NULL	*/
/*			Converted JIS->N64code				*/
/*		*extName  The extension(JIS) Ther termination-NULL	*/
/*			Converted JIS->N64code				*/
/*		mode	NU_CONT_PAK_CREATE  Create the note if no note there*/
/*			NU_CONT_PAK_NOCREATE The error if there is no note*/
/*		size	Specify the note size				*/
/*			Valie only when NU_CONT_PAK_CREATE is specified	*/
/*	RET:	Error							*/
/*----------------------------------------------------------------------*/
s32 nuContPakFileOpenJis(NUContPakFile *file, u8* noteName, u8* extName,
		       u32 mode, s32 size)
{
    u8		noteNameN64[32];
    u8		extNameN64[4];
    u32		len;

    /* It doesn't provide the process if nuContPakOpen doesn't find the */
    /* controller pack.  This is for avoiding osPfsFileFind hanging up 	*/
    /* because of no controller pack. 					*/
    if(file->type == NU_CONT_PAK_TYPE_NONE){
	file->error = PFS_ERR_NOPACK;
	return file->error;
    }
    
    bzero(noteNameN64, 32);
    bzero(extNameN64, 4);

    /* Get the number of characters */
    len = 0;
    while(noteName[len]) len++;

    
    /* The note name; converted JIS->N64code */
    nuContPakJisToN64(noteName, noteNameN64, len);

    /* The extension; converted JIS->N64code */    
    nuContPakJisToN64(extName, extNameN64, 1);

    return nuContPakFileOpen(file, noteNameN64, extNameN64, mode, size);
}
