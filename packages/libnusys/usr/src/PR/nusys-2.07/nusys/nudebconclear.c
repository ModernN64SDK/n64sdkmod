/*======================================================================*/
/*		NuSYS							*/
/*		nudebconclear.c						*/
/*									*/
/*		Copyright (C) 1997, NINTENDO Co,Ltd.			*/
/*									*/
/*----------------------------------------------------------------------*/    
/* Ver 1.0	97/10/9		Created by Kensaku Ohki(SLANP)		*/
/*======================================================================*/
/* $Id: nudebconclear.c,v 1.2 1998/04/21 00:00:08 ohki Exp $	*/
/*======================================================================*/
#include <nusys.h>

/*----------------------------------------------------------------------*/
/*	nuDebConClr - Clear the console					*/
/* 	Clear the console display buffer				*/
/*	IN:	windowNo	The window number			*/
/*	RET:	None							*/
/*----------------------------------------------------------------------*/
void nuDebConClear(u32 wndNo)
{
    NUDebConWindow* conWin;
    conWin = &nuDebConWin[wndNo];
    
    bzero(conWin->text, NU_DEB_CON_TEXT_SIZE*sizeof(u16));
    nuDebConTextPos(wndNo, 0, 0);
}
