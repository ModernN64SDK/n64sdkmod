/*======================================================================*/
/*		NuSYS										*/
/*		nugfxswapcfbfuncset.c							*/
/*												*/
/*		Copyright (C) 1997, NINTENDO Co,Ltd.				*/
/*												*/
/*----------------------------------------------------------------------*/    
/* Ver 1.0	97/10/9		Created by Kensaku Ohki(SLANP)		*/
/*======================================================================*/
#include <nusys.h>

/*----------------------------------------------------------------------*/
/*	Register the call-back function for swithcing frame buffers 	*/
/*	IN:	The pointer for the main function 					*/
/*	RET:	None 										*/
/*----------------------------------------------------------------------*/
void nuGfxSwapCfbFuncSet(NUGfxSwapCfbFunc func)
{
    OSIntMask mask;
    
    mask = osSetIntMask(OS_IM_NONE);
    nuGfxSwapCfbFunc = func;
    osSetIntMask(mask);
}

