/*======================================================================*/
/*		NuSYS										*/
/*		nudebconwindowset.c							*/
/*												*/
/*		Copyright (C) 1997, NINTENDO Co,Ltd.				*/
/*												*/
/*----------------------------------------------------------------------*/    
/* Ver 1.0	97/10/9		Created by Kensaku Ohki(SLANP)		*/
/*======================================================================*/
#include <nusys.h>

/*----------------------------------------------------------------------------*/
/*	nuDebConWindowSet - Set the console window  					*/
/* 	Set the size and position of the console window 				*/
/*	IN:	wndNo	The window number 							*/
/*			NU_DEB_CON_WINDOW0							*/
/*			NU_DEB_CON_WINDOW1							*/
/*			NU_DEB_CON_WINDOW2							*/
/*			NU_DEB_CON_WINDOW3							*/
/*		winX		The X-coordinate to start the window display 		*/
/*		winY		The Y-coordinate to start the window display 		*/
/*		width		The number of digits of the character display (1-40)	*/
/*		height	The number of lines of the character display (1-30)	*/
/*	RET:	None 											*/
/*----------------------------------------------------------------------------*/
void nuDebConWindowSet(u32 wndNo, u32 winX , u32 winY, u32 width, u32 height)
{
    nuDebConWindowPos(wndNo, winX, winY);
    nuDebConWindowSize(wndNo, width, height);
}

