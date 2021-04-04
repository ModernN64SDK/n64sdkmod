/*======================================================================*/
/*		NuSYS										*/
/*		nucontreadfuncset.c							*/
/*												*/
/*		Copyright (C) 1997, NINTENDO Co,Ltd.				*/
/*												*/
/*----------------------------------------------------------------------*/    
/* Ver 1.0	97/10/9		Created by Kensaku Ohki(SLANP)		*/
/*======================================================================*/
#include <nusys.h>

/*----------------------------------------------------------------------*/
/*	Register the call-back function of the controller 			*/
/*	IN:	The pointer of the call-back function 				*/
/*	RET:	None 										*/
/*----------------------------------------------------------------------*/
void nuContReadFuncSet(NUContReadFunc func)
{
    OSIntMask	mask;

    mask = osSetIntMask(OS_IM_NONE);
    nuContReadFunc = func;
    osSetIntMask(mask);
}
