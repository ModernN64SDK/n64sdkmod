/*======================================================================*/
/*		NuSYS							*/
/*		nucontpakrepairid.c					*/
/*									*/
/*		Copyright (C) 1997, NINTENDO Co,Ltd.			*/
/*									*/
/*----------------------------------------------------------------------*/    
/* Ver 1.0	97/10/9		Created by Kensaku Ohki(SLANP)		*/
/*======================================================================*/
/* $Id: nucontpakrepairid.c,v 1.6 1999/01/21 07:20:46 ohki Exp $	*/
/*======================================================================*/
#include <nusys.h>

/*----------------------------------------------------------------------*/
/*	nuContPakRepairId - Repair the file system of the controller pack*/
/*	Use it if PFS_ERR_ID_FATAL returns in osPfsInitPak.		*/
/*	IN:	*file	The controller pack structure			*/
/*	RET:	Error							*/
/*----------------------------------------------------------------------*/
s32 nuContPakRepairId(NUContPakFile *file)
{
        file->error = nuSiSendMesg(NU_CONT_PAK_REPAIRID_MSG, (void*)file);

	return file->error;
}
