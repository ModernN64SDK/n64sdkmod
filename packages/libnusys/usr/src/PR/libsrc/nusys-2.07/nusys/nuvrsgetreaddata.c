/*======================================================================*/
/*		NuSYS							*/
/*		nuvrsgetreaddata.c					*/
/*									*/
/*		Copyright (C) 1999, NINTENDO Co,Ltd.			*/
/*									*/
/*======================================================================*/
/* $Id: nuvrsgetreaddata.c,v 1.1 1999/01/23 05:51:24 ohki Exp $	*/
/*======================================================================*/
#include <nusys.h>

/*----------------------------------------------------------------------*/
/*	nuVrsGetReadData - Obtains results of voice recognition by the voice 	recognition system						*/
/*									*/
/*	Initiates processing by the voice recognition system. 			*/
/*									*/
/*	IN:	handle		Structure of type NUVrsHandle			*/
/*	RET:	Error							*/
/*----------------------------------------------------------------------*/
s32 nuVrsGetReadData(NUVrsHandle* handle, NUVrsData* data)
{
    NUVrsMesg mesg;
    mesg.vrsHandle = handle;
    mesg.data[0] = (s32)data;
    return nuSiSendMesg(NU_VRS_GET_READ_DATA_MSG, (void*)&mesg);
}
