/*======================================================================*/
/*		NuSYS										*/
/*		nucontpakcodeset.c							*/
/*												*/
/*		Copyright (C) 1997, NINTENDO Co,Ltd.				*/
/*												*/
/*----------------------------------------------------------------------*/    
/* Ver 1.0	97/10/9		Created by Kensaku Ohki(SLANP)		*/
/*======================================================================*/
#include <nusys.h>

/*----------------------------------------------------------------------*/
/*	nuContPakCodeSet - Set the code of the controller pack. 		*/
/* 	Set the company and game codes of the 					*/
/*	controller pack to the controller manager. 				*/
/*	IN:	companyCode[2]	The company code 					*/
/*		gameCode[4]	The game code 						*/
/*	RET:	None 										*/
/*----------------------------------------------------------------------*/
void nuContPakCodeSet(u8* companyCode, u8* gameCode)
{
    bcopy(companyCode, (u8*)&nuContPakCompanyCode, 2);
    bcopy(gameCode, (u8*)&nuContPakGameCode, 4);
}
