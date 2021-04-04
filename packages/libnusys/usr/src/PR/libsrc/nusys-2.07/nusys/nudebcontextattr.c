/*======================================================================*/
/*		NuSYS							*/
/*		nudebcontextattr.c					*/
/*									*/
/*		Copyright (C) 1997, NINTENDO Co,Ltd.			*/
/*									*/
/*----------------------------------------------------------------------*/    
/* Ver 1.0	97/10/9		Created by Kensaku Ohki(SLANP)		*/
/*======================================================================*/
#include <nusys.h>

/*----------------------------------------------------------------------*/
/*	nuDebConTextAttr - The attribute setting of console characters 	*/
/* 	Set the attribute of console characters 			*/
/*	IN:	wndNo	The window number 				*/
/*			NU_DEB_CON_WINDOW0				*/
/*			NU_DEB_CON_WINDOW1				*/
/*			NU_DEB_CON_WINDOW2				*/
/*			NU_DEB_CON_WINDOW3				*/
/*		attr	The character attribute 			*/
/*			NU_DEB_CON_ATTR_BLINK				*/
/*			NU_DEB_CON_ATTR_NORMAL				*/
/*			NU_DEB_CON_ATTR_REVERSE				*/
/*	RET:	None 							*/
/*----------------------------------------------------------------------*/
void nuDebConTextAttr(u32 wndNo, u32 attr)
{
    attr &= 0x0f;
    nuDebConWin[wndNo].attribute = attr;
}
