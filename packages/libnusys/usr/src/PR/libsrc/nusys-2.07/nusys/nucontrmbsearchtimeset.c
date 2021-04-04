/*======================================================================*/
/*		NuSYS										*/
/*		nucontrmbsearchtimeset.c						*/
/*												*/
/*		Copyright (C) 1997, NINTENDO Co,Ltd.				*/
/*												*/
/*----------------------------------------------------------------------*/    
/* Ver 1.0	97/10/9		Created by Kensaku Ohki(SLANP)		*/
/*======================================================================*/
#include <nusys.h>

/*----------------------------------------------------------------------------*/
/*	nuContRmbSearchTimeSet	Set the check time of the oscillating pack 	*/
/*	Set the time interval to check the oscillating pack by the auto 		*/
/*             recognition mode.                 					*/
/*	IN:	frame	The time interval to check (The frame) 				*/
/*	RET:	None 											*/
/*----------------------------------------------------------------------------*/
void nuContRmbSearchTimeSet(u32 frame)
{
    OSIntMask	mask;
    mask = osSetIntMask(OS_IM_NONE);
    nuContRmbSearchTime = frame;
    osSetIntMask(mask);
}

