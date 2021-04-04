/*======================================================================*/
/*		NuSYS							*/
/*		nucontpakopen.c						*/
/*									*/
/*		Copyright (C) 1997, NINTENDO Co,Ltd.			*/
/*									*/
/*----------------------------------------------------------------------*/    
/* Ver 1.0	97/10/9		Created by Kensaku Ohki(SLANP)		*/
/*======================================================================*/
/* $Id: nucontpakopen.c,v 1.5 1999/01/21 07:20:39 ohki Exp $	*/
/*======================================================================*/
#include <nusys.h>

/*----------------------------------------------------------------------*/
/*	nuContPakOpen - Open the device of the controller pack		*/
/* 	Open the device of the controller pack				*/
/*	IN:	*file	The controller pack structure			*/
/*	RET:	Error							*/
/*----------------------------------------------------------------------*/
s32 nuContPakOpen(NUContPakFile *file, u32 contNo)
{
    file->pfs = &nuContPfs[contNo];
    file->pfs->channel = contNo;
    file->error = nuSiSendMesg(NU_CONT_PAK_OPEN_MSG, (void*)file);
    return file->error;
}
