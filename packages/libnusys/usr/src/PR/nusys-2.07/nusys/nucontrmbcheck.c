/*======================================================================*/
/*		NuSYS							*/
/*		nucontrmbcheck.c					*/
/*									*/
/*		Copyright (C) 1997, NINTENDO Co,Ltd.			*/
/*									*/
/*----------------------------------------------------------------------*/    
/* Ver 1.0	97/10/9		Created by Kensaku Ohki(SLANP)		*/
/*======================================================================*/
/* $Id: nucontrmbcheck.c,v 1.6 1999/05/07 08:18:59 ohki Exp $		*/
/*======================================================================*/
#include <nusys.h>

/*----------------------------------------------------------------------*/
/*	nuContRmbcheck - Check the oscillating pack 			*/
/* 	Check if the oscillating pack is inserted. 			*/
/*	The controller pack is not detected; thus, if you examine which */
/*	pack is inserted, use nuContPakOpen. 				*/
/*	IN:	contNo	The number of the controller 			*/
/*	RET:	The return value of osMotorInit 			*/
/*----------------------------------------------------------------------*/
s32 nuContRmbCheck(u32 contNo)
{
    NUContRmbMesg	checkMesg;
    s32			rtn;
    OSIntMask	mask;
    

    checkMesg.contNo   = contNo;
    rtn = nuSiSendMesg(NU_CONT_RMB_CHECK_MSG, (void*)&checkMesg);
    
    mask = osSetIntMask(OS_IM_NONE); 
    /* Check errors; if there are no errors, it is the oscillating pack. */
    if(!rtn){
	nuContRmbCtl[contNo].type = NU_CONT_PAK_TYPE_RUMBLE;
    } else {
	nuContRmbCtl[contNo].type = NU_CONT_PAK_TYPE_NONE;
    }
    osSetIntMask(mask);    
    return rtn;
}
