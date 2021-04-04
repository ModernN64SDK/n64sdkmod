/*======================================================================*/
/*		NuSYS										*/
/*		nuidlefuncset.c								*/
/*												*/
/*		Copyright (C) 1997, NINTENDO Co,Ltd.				*/
/*												*/
/*----------------------------------------------------------------------*/    
/* Ver 1.0	97/10/9		Created by Kensaku Ohki(SLANP)		*/
/*======================================================================*/
#include <nusys.h>

/*----------------------------------------------------------------------*/
/*	Register the call-back function in the loop of the Idle thread 	*/
/*	IN:	The pointer of the call-back function 				*/
/*	RET:	None 										*/
/*----------------------------------------------------------------------*/
void nuIdleFuncSet(NUIdleFunc func)
{
    OSIntMask mask;
    
    mask = osSetIntMask(OS_IM_NONE);
    nuIdleFunc = func;
    osSetIntMask(mask);
}
