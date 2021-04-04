/*======================================================================*/
/*		NuSYS										*/
/*		nudebconputc.c								*/
/*												*/
/*		Copyright (C) 1997, NINTENDO Co,Ltd.				*/
/*												*/
/*----------------------------------------------------------------------*/    
/* Ver 1.0	97/10/9		Created by Kensaku Ohki(SLANP)		*/
/*======================================================================*/
#include <nusys.h>

/*----------------------------------------------------------------------*/
/*	nuDebConPutc - Output characters to the console 			*/
/* 	Output characters to the console 						*/
/*	IN:	windowNo	The window number to display 				*/
/*		c		The character code 					*/
/*	RET:	None 										*/
/*----------------------------------------------------------------------*/
void nuDebConPutc(u32 wndNo, u32  c)
{
    NUDebConWindow* conWin;
    u32		color;
    u32		attr;
    
    conWin = &nuDebConWin[wndNo];
    color = conWin->textColor << 8;
    attr = conWin->attribute << 12;

    /* Check if it is the escape sequence  */
    if( c < 0x20){
	nuDebConEsc(conWin, c);
    } else if(c < 0x80){
	c -=0x20;
	conWin->text[conWin->index] = c | color | attr;
	nuDebConInc(conWin);
    }
}
