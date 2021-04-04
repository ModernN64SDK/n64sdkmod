/*======================================================================*/
/*		NuSYS										*/
/*		nugfxprenmifuncset.c							*/
/*												*/
/*		Copyright (C) 1997, NINTENDO Co,Ltd.				*/
/*												*/
/*----------------------------------------------------------------------*/    
/* Ver 1.0	97/10/9		Created by Kensaku Ohki(SLANP)		*/
/*======================================================================*/
#include <nusys.h>

/*----------------------------------------------------------------------------*/
/*	Register the PRENMI call-back function in the loop of the Gfx thread	*/
/*	IN:	The pointer of the call-back function 					*/
/*	RET:	None 											*/
/*----------------------------------------------------------------------------*/
void nuGfxPreNMIFuncSet(NUGfxPreNMIFunc func)
{
    OSIntMask mask;

    mask = osSetIntMask(OS_IM_NONE);
    nuGfxPreNMIFunc = func;
    osSetIntMask(mask);
}
