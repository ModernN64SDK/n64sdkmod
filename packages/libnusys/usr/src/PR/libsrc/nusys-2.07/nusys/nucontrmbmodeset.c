/*======================================================================*/
/*		NuSYS							*/
/*		nucontrmbenble.c					*/
/*									*/
/*		Copyright (C) 1997, NINTENDO Co,Ltd.			*/
/*									*/
/*----------------------------------------------------------------------*/    
/* Ver 1.0	97/10/9		Created by Kensaku Ohki(SLANP)		*/
/*======================================================================*/
#include <nusys.h>

/*----------------------------------------------------------------------*/
/*	nuContRmModeSet - Set the mode of the oscillating pack 		*/
/* 	Set the operation of the oscillating pack 			*/
/*	IN:	contNo	The controller number 				*/
/*		mode	The operation mode of the oscillating pack	*/
/*			NU_CONT_RMB_MODE_DISABLE	Disabled 	*/
/*			NU_CONT_RMB_MODE_ENABLE		Enabled	 	*/
/*			NU_CONT_RMB_MODE_AUTORUN	The auto recognition*/
/*	RET:	FALSE if the oscillating pack is not put in.  		*/
/*----------------------------------------------------------------------*/
void nuContRmbModeSet(u32 contNo, u8 mode)
{
    u32		forceStop;
    OSIntMask	mask;
    
    mask = osSetIntMask(OS_IM_NONE);
    
    /* Fetch the flag of the forced stop mode  */
    forceStop = nuContRmbCtl[contNo].mode & NU_CONT_RMB_MODE_PAUSE;
    
    /* Set the operation of the oscillating pack 	*/
    switch(mode){
    case NU_CONT_RMB_MODE_DISABLE:	/* Disabled  */
	nuContRmbCtl[contNo].mode = mode | forceStop;
	break;
    case NU_CONT_RMB_MODE_ENABLE:	/* Enabled  */
	/* Not permitted if the nuContRmbCheck function has not checked.  */
	if(nuContRmbCtl[contNo].type == NU_CONT_PAK_TYPE_RUMBLE){
	    nuContRmbCtl[contNo].mode = mode | forceStop;
	}
	break;
    case NU_CONT_RMB_MODE_AUTORUN:	/* The auto recognition  */
	/* In the case of detecting the oscillating pack automatically, examine */
	/* if it has been already detected and initialize the automatic detection. */
	nuContRmbCtl[contNo].mode = mode | forceStop;
	if(nuContRmbCtl[contNo].type == NU_CONT_PAK_TYPE_RUMBLE){
	    nuContRmbCtl[contNo].autorun = NU_CONT_RMB_AUTO_FIND;
	} else {
	    nuContRmbCtl[contNo].autorun = NU_CONT_RMB_AUTO_SEARCH;
	}
    }
     osSetIntMask(mask);    
}
