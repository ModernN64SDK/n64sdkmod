/*======================================================================*/
/*		NuSYS										*/
/*		nudebconwindowpos.c							*/
/*												*/
/*		Copyright (C) 1997, NINTENDO Co,Ltd.				*/
/*												*/
/*----------------------------------------------------------------------*/    
/* Ver 1.0	97/10/9		Created by Kensaku Ohki(SLANP)		*/
/*======================================================================*/
#include <nusys.h>

/*----------------------------------------------------------------------*/
/*	nuDebConWindowPos - Set the position of the console window 		*/
/* 	Set the display position of the console window 				*/
/*	IN:	wndNo	The window number 						*/
/*			NU_DEB_CON_WINDOW0						*/
/*			NU_DEB_CON_WINDOW1						*/
/*			NU_DEB_CON_WINDOW2						*/
/*			NU_DEB_CON_WINDOW3						*/
/*		winX	The X-coordinate on the upper-left window 		*/
/*		winY	The Y-coordinate on the upper-left window 		*/
/*	RET:	None 										*/
/*----------------------------------------------------------------------*/
void nuDebConWindowPos(u32 wndNo, u32 winX , u32 winY)
{
    /* The upper-left coordinate on the console window  */
    nuDebConWin[wndNo].winX = winX;
    nuDebConWin[wndNo].winY = winY;
}
