/*======================================================================*/
/*		NuSYS										*/
/*		nudebconwindowsize.c							*/
/*												*/
/*		Copyright (C) 1997, NINTENDO Co,Ltd.				*/
/*												*/
/*----------------------------------------------------------------------*/    
/* Ver 1.0	97/10/9		Created by Kensaku Ohki(SLANP)		*/
/*======================================================================*/
#include <nusys.h>

/*----------------------------------------------------------------------------------*/
/*	nuDebConWindowsize - Set the console window  						*/
/* 	Set the window size of the console 								*/
/*	IN:	wndNo	The window number 								*/
/*			NU_DEB_CON_WINDOW0								*/
/*			NU_DEB_CON_WINDOW1								*/
/*			NU_DEB_CON_WINDOW2								*/
/*			NU_DEB_CON_WINDOW3								*/
/*		width		The number of digits of the character representation (1-40)	*/
/*		height	The number of lines of the character representation (1-30)	*/
/*	RET:	None 												*/
/*----------------------------------------------------------------------------------*/
void nuDebConWindowSize(u32 wndNo,  u32 width, u32 height)
{
    NUDebConWindow* conWin;

    conWin = &nuDebConWin[wndNo];
    
    /* Check the number of digits of the character representation  */
    if(width > NU_DEB_CON_ROW_MAX){
	width = NU_DEB_CON_ROW_MAX;
    }
    
    /* Check the number of lines of the character representation  */
    if(height > NU_DEB_CON_COLUMN_MAX){
	height = NU_DEB_CON_COLUMN_MAX;
    }
    
    /* Check if the number of digits of the character representation is 0. */
    if(!conWin->winW){
	conWin->winW = 1;
    }
    
    /* Check if the number of lines of the character representation is 0. */
    if(!conWin->winH){
	conWin->winH = 1;
    }
    conWin->winW = width;
    conWin->winH = height;

    /* Initialize the character representation position  */
    conWin->posX = 0;
    conWin->posY = 0;
    
}
