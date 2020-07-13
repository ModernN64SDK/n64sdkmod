/*======================================================================*/
/*		NuSYS										*/
/*		nudebcontextcolor.c							*/
/*												*/
/*		Copyright (C) 1997, NINTENDO Co,Ltd.				*/
/*												*/
/*----------------------------------------------------------------------*/    
/* Ver 1.0	97/10/9		Created by Kensaku Ohki(SLANP)		*/
/*======================================================================*/
#include <nusys.h>

/*----------------------------------------------------------------------*/
/*	nuDebConTextColor - Set the color of the console character 		*/
/* 	Set the color of the console character 					*/
/*	IN:	wndNo	The window number 						*/
/*			NU_DEB_CON_WINDOW0						*/
/*			NU_DEB_CON_WINDOW1						*/
/*			NU_DEB_CON_WINDOW2						*/
/*			NU_DEB_CON_WINDOW3						*/
/*		color	Character colors (0-15)						*/
/*	RET:	None 										*/
/*----------------------------------------------------------------------*/
void nuDebConTextColor(u32 wndNo, u32 color)
{
    /* There are 16 character colors.   */
    color &= 0x0f;
    nuDebConWin[wndNo].textColor = color;
}
