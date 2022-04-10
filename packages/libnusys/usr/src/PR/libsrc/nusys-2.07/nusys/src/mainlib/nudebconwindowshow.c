/*======================================================================*/
/*		NuSYS										*/
/*		nudebconwindowshow.c							*/
/*												*/
/*		Copyright (C) 1997, NINTENDO Co,Ltd.				*/
/*												*/
/*----------------------------------------------------------------------*/    
/* Ver 1.0	97/10/9		Created by Kensaku Ohki(SLANP)		*/
/*======================================================================*/
#include <nusys.h>

/*----------------------------------------------------------------------*/
/*	nuDebConWindowShow - Display-ON/OFF of the console window 		*/
/*	Display-ON/OFF of the console window 					*/
/*	IN:	wndNo	The window number 						*/
/*			NU_DEB_CON_WINDOW0						*/
/*			NU_DEB_CON_WINDOW1						*/
/*			NU_DEB_CON_WINDOW2						*/
/*			NU_DEB_CON_WINDOW3						*/
/*		flag	Display-ON/OFF 							*/
/*			NU_DEB_CON_WINDOW_OFF						*/
/*			NU_DEB_CON_WINDOW_ON(default)					*/
/*	RET:	None 										*/
/*----------------------------------------------------------------------*/
void nuDebConWindowShow(u32 wndNo, u32 flag)
{
    nuDebConWin[wndNo].windowFlag = flag;
}
