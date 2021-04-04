/*======================================================================*/
/*		NuSYS										*/
/*		nugfxtaskendfuncset.c							*/
/*												*/
/*		Copyright (C) 1997, NINTENDO Co,Ltd.				*/
/*												*/
/*----------------------------------------------------------------------*/    
/* Ver 1.0	97/10/9		Created by Kensaku Ohki(SLANP)		*/
/*======================================================================*/
#include <nusys.h>

/*----------------------------------------------------------------------*/
/*	The call-back function of the end message of the graphic task 	*/
/*	IN:	The pointer for the call-back function 				*/
/*	RET:	None 										*/
/*----------------------------------------------------------------------*/
void nuGfxTaskEndFuncSet(NUGfxTaskEndFunc func)
{
    OSIntMask mask;
    
    mask = osSetIntMask(OS_IM_NONE);
    nuGfxTaskEndFunc = func;
    osSetIntMask(mask);
}
