/*======================================================================*/
/*		NuSYS										*/
/*		nudebconcputs.c								*/
/*												*/
/*		Copyright (C) 1997, NINTENDO Co,Ltd.				*/
/*												*/
/*----------------------------------------------------------------------*/    
/* Ver 1.0	97/10/9		Created by Kensaku Ohki(SLANP)		*/
/*======================================================================*/
#include <nusys.h>

/*----------------------------------------------------------------------*/
/*	nuDebConCPuts - Output the character string to the console 		*/
/* 	Output the character string to the console 				*/
/*	IN:	windowNo	The window number to display 				*/
/*		*s		The pointer of the character string 		*/
/*	RET:	None 										*/
/*----------------------------------------------------------------------*/
void nuDebConCPuts(u32 wndNo, const char *s)
{
    
    while(*s){
	nuDebConPutc(wndNo, (u32)*s);
	s++;
    }
}
