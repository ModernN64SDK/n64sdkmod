/*======================================================================*/
/*		NuSYS							*/
/*		nucontrmbstop.c						*/
/*									*/
/*		Copyright (C) 1997, NINTENDO Co,Ltd.			*/
/*									*/
/*======================================================================*/
/* $Id: nucontrmbstop.c,v 1.2 1999/01/21 07:17:12 ohki Exp $	*/
/*======================================================================*/
#include <nusys.h>

/*----------------------------------------------------------------------*/
/*	nuContRmbStop - Stops the Rumble Pak from vibrating				*/
/*	IN:	contNo	Controller number 				*/
/*	RET:	nothing							*/
/*----------------------------------------------------------------------*/
void nuContRmbStop(u32 contNo)
{    
    NUContRmbMesg rmbMesg;
    
    rmbMesg.contNo = contNo;
    nuSiSendMesg(NU_CONT_RMB_STOP_MSG, (void*)&rmbMesg);
}
