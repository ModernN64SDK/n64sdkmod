/*======================================================================*/
/*		NuSYS										*/
/*		nudebcontextpos.c								*/
/*												*/
/*		Copyright (C) 1997, NINTENDO Co,Ltd.				*/
/*												*/
/*----------------------------------------------------------------------*/    
/* Ver 1.0	97/10/9		Created by Kensaku Ohki(SLANP)		*/
/*======================================================================*/
#include <nusys.h>

/*----------------------------------------------------------------------*/
/*	nuDebConTextPos - Set the character display position 			*/
/* 	Set the character display position in the console window 		*/
/*	IN:	wndNo	The window number 						*/
/*		posX	The X-coordinate on the window 				*/
/*		posY	The Y-coordinate on the window 				*/
/*	RET:	None										*/
/*----------------------------------------------------------------------*/
void nuDebConTextPos(u32 wndNo, u32 posX, u32 posY)
{
    NUDebConWindow* conWin;
    u16		indexY;
    
    conWin = &nuDebConWin[wndNo];

    /* Reset the X-coordinate  */
    if(posX  < conWin->winW) {
	conWin->posX = posX;
    } else {
	conWin->posX = conWin->winW - 1;
    }
    
    /* Reset the Y-coordinate  */
    if(posY < conWin->winH){
	conWin->posY = posY;
    } else {
	conWin->posY = conWin->winH - 1;
    }

    /* Recalculate the character store index  */
    indexY = (conWin->posY + conWin->scroll) % conWin->winH;
    conWin->index = indexY * conWin->winW + conWin->posX;
}
