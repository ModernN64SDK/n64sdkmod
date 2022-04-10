/*======================================================================*/
/*		NuSYS							*/
/*		nucontpakgetfree.c					*/
/*									*/
/*		Copyright (C) 1997, NINTENDO Co,Ltd.			*/
/*									*/
/*----------------------------------------------------------------------*/    
/* Ver 1.0	97/10/9		Created by Kensaku Ohki(SLANP)		*/
/*======================================================================*/
/* $Id: nucontpakgetfree.c,v 1.5 1999/01/21 07:20:30 ohki Exp $	*/
/*======================================================================*/
#include <nusys.h>

/*----------------------------------------------------------------------*/
/*	nuContPakGetFree - Get the space capacity of the controller pack*/
/* 	Get the space capacity of the controller pack. 			*/
/*	If it is the error, the error code enters into "error" 		*/
/*	of NUContPakFile.						*/
/*	IN:	*file	The controller pack structure 			*/
/*	RET:	Return the space capacity. 				*/
/*----------------------------------------------------------------------*/
s32 nuContPakGetFree(NUContPakFile *file)
{
    NUContPakFreeMesg	freeMesg;

    freeMesg.file = file;
    file->error = nuSiSendMesg(NU_CONT_PAK_FREE_MSG, (void*)&freeMesg);
    return freeMesg.size;
}
