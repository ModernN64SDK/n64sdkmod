/*======================================================================*/
/*		NuSYS										*/
/*		nuprenmifuncset.c								*/
/*												*/
/*		Copyright (C) 1997, NINTENDO Co,Ltd.				*/
/*												*/
/*----------------------------------------------------------------------*/    
/* Ver 1.0	97/10/9		Created by Kensaku Ohki(SLANP)		*/
/*======================================================================*/
#include <nusys.h>

/*----------------------------------------------------------------------------------*/
/*	Register the PRE NMI call-back function 							*/
/*	IN:	The pointer for the PRE NMI-process function 					*/
/*	RET:	None 												*/
/*----------------------------------------------------------------------------------*/
/*	Note: 												*/
/*	This call-back function is directly called from the event handler 		*/
/*	thread.  Therefore, provide the process by the call-back function 		*/
/*	as least as possible.  										*/
/*	Also, the other threads don't operate during executing the call-back function	*/
/*----------------------------------------------------------------------------------*/
void nuPreNMIFuncSet(NUScPreNMIFunc func)
{
    OSIntMask mask;

    mask = osSetIntMask(OS_IM_NONE);
    nuScPreNMIFunc =(NUScPreNMIFunc) func;
    osSetIntMask(mask);
}
