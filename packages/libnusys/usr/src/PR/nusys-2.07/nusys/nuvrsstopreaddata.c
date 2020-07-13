/*======================================================================*/
/*		NuSYS							*/
/*		nuvrsstopreaddata.c					*/
/*									*/
/*		Copyright (C) 1999, NINTENDO Co,Ltd.			*/
/*									*/
/*======================================================================*/
/* $Id: nuvrsstopreaddata.c,v 1.1 1999/01/23 05:52:15 ohki Exp $	*/
/*======================================================================*/
#include <nusys.h>

/*----------------------------------------------------------------------*/
/*	nuVrsStopReadData - Forcibly stops voice recognition	*/
/*									*/
/*	Forcibly stops voice recognition processing.		*/
/*									*/
/*	IN:	handle		Structure of type NUVrsHandle			*/
/*	RET:	error							*/
/*----------------------------------------------------------------------*/
s32 nuVrsStopReadData(NUVrsHandle* handle)
{
    return nuSiSendMesg(NU_VRS_STOP_READ_DATA_MSG, (void*)handle);
}
