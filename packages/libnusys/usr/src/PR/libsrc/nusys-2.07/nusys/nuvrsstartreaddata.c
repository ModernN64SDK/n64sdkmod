/*======================================================================*/
/*		NuSYS							*/
/*		nuvrsstartreaddata.c					*/
/*									*/
/*		Copyright (C) 1999, NINTENDO Co,Ltd.			*/
/*									*/
/*======================================================================*/
/* $Id: nuvrsstartreaddata.c,v 1.1 1999/01/23 05:53:41 ohki Exp $	*/
/*======================================================================*/
#include <nusys.h>

/*----------------------------------------------------------------------*/
/*	nuVrsStartReadData - Starts voice recognition		*/
/*									*/
/*	Starts voice recognition by the voice recognition system. */
/*									*/
/*	IN:	handle		Structure of type NUVrsHandle			*/
/*	RET:	error							*/
/*----------------------------------------------------------------------*/
s32 nuVrsStartReadData(NUVrsHandle* handle)
{
    return nuSiSendMesg(NU_VRS_START_READ_DATA_MSG, (void*)handle);
}
