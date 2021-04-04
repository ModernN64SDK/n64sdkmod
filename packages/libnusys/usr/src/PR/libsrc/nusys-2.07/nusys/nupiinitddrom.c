/*======================================================================*/
/*		NuSYS							*/
/*		nupisraminit.c						*/
/*									*/
/*		Copyright (C) 1997, NINTENDO Co,Ltd.			*/
/*									*/
/*----------------------------------------------------------------------*/
/* Ver 1.2	98/07/4		Created by Kensaku Ohki(SLANP)		*/
/*----------------------------------------------------------------------*/
/* $Id: nupiinitddrom.c,v 1.2 1999/01/14 11:34:22 ohki Exp $	*/
/*======================================================================*/
#include <nusys.h>

OSPiHandle*	nuPiDDRomHandle;

/*----------------------------------------------------------------------*/
/*	nuPiDDRomInit  -  Initializes the DDROM handle		*/
/*	Initializes the DDROM handle.				*/
/*	IN:	nothing							*/
/*	RET:	nothing							*/
/*----------------------------------------------------------------------*/
void nuPiInitDDRom(void)
{
    nuPiDDRomHandle = osDriveRomInit();
}
