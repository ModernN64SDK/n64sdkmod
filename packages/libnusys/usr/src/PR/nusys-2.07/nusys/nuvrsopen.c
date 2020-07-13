/*======================================================================*/
/*		NuSYS							*/
/*		nuvrsopen.c						*/
/*									*/
/*		Copyright (C) 1999, NINTENDO Co,Ltd.			*/
/*									*/
/*======================================================================*/
/* $Id: nuvrsopen.c,v 1.1 1999/01/23 05:52:01 ohki Exp $	*/
/*======================================================================*/
#include <nusys.h>

/*----------------------------------------------------------------------*/
/*	nuVrsOpen - Initializes the voice recognition system				*/
/*									*/
/*	 Initializes the Voice Recognition System Manager.			*/
/*	IN:	handle	Structure of type NUVrsHandle				*/
/*		contNo	Number of port to which voice recognition system is connected.	*/
/*	RET:	error							*/
/*----------------------------------------------------------------------*/
s32 nuVrsOpen(NUVrsHandle* handle, s32 contNo)
{
    NUVrsMesg	mesg;
    
    mesg.vrsHandle = handle;
    mesg.data[0] = contNo;
    return nuSiSendMesg(NU_VRS_OPEN_MSG, (void*)&mesg);
}
