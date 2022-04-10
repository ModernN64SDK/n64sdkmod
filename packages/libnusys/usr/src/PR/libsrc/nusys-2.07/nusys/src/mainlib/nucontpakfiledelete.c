/*======================================================================*/
/*		NuSYS							*/
/*		nucontpakfiledelete.c					*/
/*									*/
/*		Copyright (C) 1997, NINTENDO Co,Ltd.			*/
/*									*/
/*----------------------------------------------------------------------*/    
/* Ver 1.0	97/10/9		Created by Kensaku Ohki(SLANP)		*/
/*======================================================================*/
/* $Id: nucontpakfiledelete.c,v 1.5 1999/01/21 07:17:31 ohki Exp $	*/
/*======================================================================*/
#include <nusys.h>

/*----------------------------------------------------------------------*/
/*	nuContPakFileDelete - Delete the controller pack note		*/
/* 	Open the file of the controller pack				*/
/*	IN:	*file	The controller pack structure			*/
/*		noteName 	The note name				*/
/*		extName		The extension				*/
/*	RET:	Error							*/
/*----------------------------------------------------------------------*/
s32 nuContPakFileDelete(NUContPakFile *file, u8* noteName, u8* extName)
{
    NUContPakFileOpenMesg fopenMesg;

    fopenMesg.file     = file;
    fopenMesg.noteName = noteName;
    fopenMesg.extName  = extName;
    file->error = nuSiSendMesg(NU_CONT_PAK_FILEDELETE_MSG, (void*)&fopenMesg);
    return file->error;
}

    