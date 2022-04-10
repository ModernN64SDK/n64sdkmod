/*======================================================================*/
/*		NuSYS							*/
/*		nucontpakfilestate.c					*/
/*									*/
/*		Copyright (C) 1997, NINTENDO Co,Ltd.			*/
/*									*/
/*----------------------------------------------------------------------*/    
/* Ver 1.0	97/10/9		Created by Kensaku Ohki(SLANP)		*/
/*======================================================================*/
/* $Id: nucontpakfilestate.c,v 1.7 1999/01/21 07:18:44 ohki Exp $	*/
/*======================================================================*/
#include <nusys.h>

/*----------------------------------------------------------------------*/
/*	nuContPakFileState - Get the state of the file of the controller pack*/
/* 	Get the state of the file of the controller pack		*/
/*	IN:	*File	The controller pack structure			*/
/*		*state	The pointer of the OSPfsState structure			*/
/*	RET:	Error							*/
/*----------------------------------------------------------------------*/
s32 nuContPakFileState(NUContPakFile *file, OSPfsState* state)
{
    NUContPakFileStateMesg stateMesg;

    stateMesg.file     = file;
    stateMesg.state    = state;
    file->error = nuSiSendMesg(NU_CONT_PAK_FILESTATE_MSG, (void*)&stateMesg);

    return file->error;

}
