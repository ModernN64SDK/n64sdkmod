/*======================================================================*/
/*		NuSYS										*/
/*		nucondatalock.c								*/
/*												*/
/*		Copyright (C) 1997, NINTENDO Co,Ltd.				*/
/*												*/
/*----------------------------------------------------------------------*/    
/* Ver 1.0	97/10/9		Created by Kensaku Ohki(SLANP)		*/
/*======================================================================*/
#include <nusys.h>

/*----------------------------------------------------------------------*/
/*	nuContDataLock - Lock controller data 					*/
/*	IN:	None 										*/
/*	RET:	None 										*/
/*----------------------------------------------------------------------*/
void nuContDataLock(void)
{
    OSIntMask	mask;

    mask = osSetIntMask(OS_IM_NONE);
    nuContDataLockKey = NU_CONT_DATA_LOCK;
    osSetIntMask(mask);

}
/*----------------------------------------------------------------------*/
/*	nuContDataUnLock - Unlock controller data 				*/
/*	IN:	None 										*/
/*	RET:	None 										*/
/*----------------------------------------------------------------------*/
void nuContDataUnLock(void)
{
    OSIntMask	mask;
    
    mask = osSetIntMask(OS_IM_NONE);
    nuContDataLockKey = NU_CONT_DATA_UNLOCK;
    osSetIntMask(mask);
}    
