/*======================================================================*/
/*		NuSYS							*/
/*		nucontpakfilenum.c					*/
/*									*/
/*		Copyright (C) 1997, NINTENDO Co,Ltd.			*/
/*									*/
/*----------------------------------------------------------------------*/    
/* Ver 1.0	97/10/9		Created by Kensaku Ohki(SLANP)		*/
/*======================================================================*/
/* $Id: nucontpakfilenum.c,v 1.6 1999/01/21 07:18:00 ohki Exp $	*/
/*======================================================================*/
#include <nusys.h>

/*----------------------------------------------------------------------*/
/*	nuContPakFileNum - Get the maximum and using note numbers of	*/
/*				the controller pack			*/
/*	IN:	*file	The controller pack structure			*/
/*		*max_files	The maximum note number of the pack	*/
/*		*used_files	The using note number of the pack	*/
/*	RET:	Error							*/
/*----------------------------------------------------------------------*/
s32 nuContPakFileNum(NUContPakFile *file, s32 *max_files, s32 *used_files)
{
    NUContPakFileNumMesg numMesg;

    numMesg.file       = file;
    numMesg.max_files  = max_files;
    numMesg.used_files = used_files;
    file->error = nuSiSendMesg(NU_CONT_PAK_FILENUM_MSG, (void*)&numMesg);
    return file->error;
}
