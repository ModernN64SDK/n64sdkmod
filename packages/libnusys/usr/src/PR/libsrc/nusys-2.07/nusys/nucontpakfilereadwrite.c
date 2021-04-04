/*======================================================================*/
/*		NuSYS							*/
/*		nucontpakfilereadwrite.c				*/
/*									*/
/*		Copyright (C) 1997, NINTENDO Co,Ltd.			*/
/*									*/
/*----------------------------------------------------------------------*/    
/* Ver 1.0	97/10/9		Created by Kensaku Ohki(SLANP)		*/
/*======================================================================*/
/* $Id: nucontpakfilereadwrite.c,v 1.6 1999/01/21 07:18:34 ohki Exp $*/
/*======================================================================*/
#include <nusys.h>

/*----------------------------------------------------------------------*/
/*	nuContPakFileReadWrite - Read/write the controller pack file	*/
/* 	Read/write the file of the controller pack			*/
/*	IN:	*file	The controller pack structure			*/
/*		offset	The offset position to read/write		*/
/*		size	The size of read/write				*/
/*		buf	Buffers to store the read data			*/
/*			or have the data to write			*/
/*		mode	Specify writing or reading			*/
/*			NU_CONT_PAK_READ	Read data		*/
/*			NU_CONT_PAK_WRITE	Write data		*/
/*	RET:	Error							*/
/*----------------------------------------------------------------------*/
s32 nuContPakFileReadWrite(NUContPakFile *file,
			    s32 offset, s32 size, u8* buf, u32 mode)
{
    
    NUContPakFileRWMesg rwMesg;

#ifdef NU_DEBUG
    if(size & 0x1f){
	osSyncPrintf("nuContPakFileReadWrite: data size(%X) must be  multiple 32.\n", size);
	return 0;
    }
    if(offset & 0x1f){
	osSyncPrintf("nuContPakFileReadWrite: data offset(%X) must be multiple 32.\n");
	return 0;
    }
#endif /* NU_DEBUG */
    rwMesg.file     = file;
    rwMesg.offset   = offset;
    rwMesg.size     = size;
    rwMesg.mode     = mode;
    rwMesg.buf      = buf;
    file->error = nuSiSendMesg(NU_CONT_PAK_READWRITE_MSG, (void*)&rwMesg);
    
    return file->error;
}
