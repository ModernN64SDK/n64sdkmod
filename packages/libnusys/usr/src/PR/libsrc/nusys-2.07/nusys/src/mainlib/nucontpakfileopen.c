/*======================================================================*/
/*		NuSYS							*/
/*		nucontpakfileopen.c					*/
/*									*/
/*		Copyright (C) 1997, NINTENDO Co,Ltd.			*/
/*									*/
/*----------------------------------------------------------------------*/    
/* Ver 1.0	97/10/9		Created by Kensaku Ohki(SLANP)		*/
/*======================================================================*/
/* $Id: nucontpakfileopen.c,v 1.6 1999/01/21 07:18:10 ohki Exp $	*/
/*======================================================================*/
#include <nusys.h>

/*----------------------------------------------------------------------*/
/*	nuContPakFileOpen - Open Controller Pak note    		*/
/* 	Open Controller Pak file         				*/
/*	IN:	*file	Controller Pak structure			*/
/*		*noteName note name (16 bytes by N64 code)		*/
/*		*extName  extension (1 byte by N64 code)			*/
/*		mode	NU_CONT_PAK_MODE_CREATE  Creates if no note  	*/
/*			NU_CONT_PAK_MODE_NOCREATE   Error if no note    */
/*		size	Specifies the size of note 			*/
/*			Only when NU_CONT_PAK_MODE_CREATE is specified, valid.*//*	RET:	Error							*/
/*----------------------------------------------------------------------*/
s32 nuContPakFileOpen(NUContPakFile *file, u8* noteName, u8* extName,
		       u32 mode, s32 size)
{
    NUContPakFileOpenMesg fopenMesg;
    u8			noteNameBuf[16];
    u8			extNameBuf[4];

    
    /* For nuContPakOpen, the process itself will not be performed unless */
    /* it is Controller Pak.   */
    /* This is to avoid a hang-up of osPfsFileFind that occurs when there is */
    /* no Controller Pak. 	*/
    if(file->type == NU_CONT_PAK_TYPE_NONE){
	file->error = PFS_ERR_NOPACK;
	return file->error;
    }
    /* Copy game note name 	*/
    bcopy(noteName, noteNameBuf, 16);

    /* Process to make only head byte of note extension valid   	*/
    bzero(extNameBuf, 4);
    extNameBuf[0] = *extName;
    
    fopenMesg.file     = file;
    fopenMesg.noteName = noteNameBuf;
    fopenMesg.extName  = extNameBuf;
    fopenMesg.size     = size;
    fopenMesg.mode     = mode;
    file->error = nuSiSendMesg(NU_CONT_PAK_FOPEN_MSG, (void*)&fopenMesg);
    
    return file->error;
    
}
