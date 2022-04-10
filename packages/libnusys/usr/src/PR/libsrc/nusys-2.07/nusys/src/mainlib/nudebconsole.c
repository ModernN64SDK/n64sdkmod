/*======================================================================*/
/*		NuSYS							*/
/*		nuDebConsole.c						*/
/*									*/
/*		Copyright (C) 1997, NINTENDO Co,Ltd.			*/
/*									*/
/*----------------------------------------------------------------------*/    
/* Ver 1.0	97/10/9		Created by Kensaku Ohki(SLANP)		*/
/* Ver 1.2	97/07/1		Modify  by Kensaku Ohki(SLANP)		*/
/*======================================================================*/
/* $Id: nudebconsole.c,v 1.11 1998/07/07 12:38:53 ohki Exp $	*/
/*======================================================================*/
#include <nusys.h>


/*----------------------------------------------------------------------*/
/*	nuDebConInc - The increment of the output position		*/
/*	IN:	None							*/
/*	RET:	None							*/
/*----------------------------------------------------------------------*/
void nuDebConInc(NUDebConWindow* conWin)
{

    conWin->posX++;
    if(conWin->posX < conWin->winW){
	conWin->index++;
    } else {
	/* Line feed if it is the line end */
	nuDebConRtn(conWin);
    }
}

/*----------------------------------------------------------------------*/
/*	nuDebConInc - Line feed of the output position			*/
/*	IN:	None							*/
/*	RET:	None							*/
/*----------------------------------------------------------------------*/
void nuDebConRtn(NUDebConWindow* conWin)
{
    u32	cnt;
    
    conWin->posX = 0;
    if(conWin->posY < (conWin->winH - 1)){
	conWin->posY++;
	conWin->index = conWin->posY * conWin->winW;
    } else {

	/* In the case of scrolling  */
	if(conWin->scrollFlag){
	    conWin->index = conWin->scroll * conWin->winW;
	    conWin->scroll++;
	    conWin->scroll %= conWin->winH;
	    /* Clear the lowest line */
	    for(cnt = 0; cnt < conWin->winW; cnt++){
		conWin->text[conWin->index + cnt] = 0;
	    }
	} else {
	    /* If not scrolling */
	    conWin->posY = 0;
	    conWin->index = 0;
	}
    }
}

/*----------------------------------------------------------------------*/
/*	nuDebConEsc - The escape sequence process			*/
/*	IN:	None							*/
/*	RET:	None							*/
/*----------------------------------------------------------------------*/
void nuDebConEsc(NUDebConWindow* conWin, char esc)
{
    u32	cnt;
    
    switch(esc){
    case '\b' :	/* Back Space */
	if(conWin->posX > 0){
	    conWin->posX--;
	    conWin->index--;
	}
	break;
	
    case '\t' :	/* Horizontal Tabulation */
	for(cnt = 0; cnt < 4; cnt++){
	    nuDebConInc(conWin);
	}
	break;
	
    case '\n' :	/* New Line */
	nuDebConRtn(conWin);
	break;
	
    case '\v' :	/* Vertical Tabulation */
	nuDebConRtn(conWin);
	nuDebConRtn(conWin);
	break;
	
    case '\f' :	/* Form Feed */
	conWin->posX = 0;
	conWin->posY = 0;
	conWin->index = 0;
	break;
	
    case '\r' :	/* Carrige Return */
	conWin->index -= conWin->posX;
	conWin->posX = 0;
	break;
    }
}
