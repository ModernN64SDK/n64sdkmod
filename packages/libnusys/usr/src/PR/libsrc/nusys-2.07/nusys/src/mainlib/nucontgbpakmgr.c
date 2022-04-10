/*======================================================================*/
/*		NuSYS							*/
/*		nucontgbpakmgr.c					*/
/*									*/
/*		Copyright (C) 1997, NINTENDO Co,Ltd.			*/
/*									*/
/*----------------------------------------------------------------------*/    
/* Ver 1.0	97/10/9		Created by Kensaku Ohki(SLANP)		*/
/*======================================================================*/
/* $Id: nucontgbpakmgr.c,v 1.7 1999/01/23 05:42:10 ohki Exp $	*/
/*======================================================================*/

#include <nusys.h>

static s32 contGBPakOpen(NUSiCommonMesg* mesg);
static s32 contGBPakGetState(NUSiCommonMesg* mesg);
static s32 contGBPakPower(NUSiCommonMesg* mesg);
static s32 contGBPakReadID(NUSiCommonMesg* mesg);
static s32 contGBPakReadWrite(NUSiCommonMesg* mesg);
static s32 contGBPakCheckConnector(NUSiCommonMesg* mesg);

static s32 (*funcList[])(NUSiCommonMesg*) = {
    NULL,			/* NU_CONT_GBPAK_RETRACE_MSG	*/
    contGBPakOpen,		/* NU_CONT_GBPAK_OPEN_MSG	*/
    contGBPakGetState,		/* NU_CONT_GBPAK_STATUS_MSG	*/
    contGBPakPower,		/* NU_CONT_GBPAK_POWER_MSG	*/
    contGBPakReadID,		/* NU_CONT_GBPAK_READID_MSG	*/
    contGBPakReadWrite,		/* NU_CONT_GBPAK_READWRITE_MSG	*/
    contGBPakCheckConnector,	/* NU_CONT_GBPAK_CHECKCONNECTOR_MSG	*/
    NULL			/* End of list */        
};
NUCallBackList	nuContGBPakCallBack = {NULL, funcList, NU_SI_MAJOR_NO_GBPAK};

/*----------------------------------------------------------------------*/
/*	nuContGBPakMgrInit - Initializes the 64GB Pak Manager		*/
/*	IN:	nothing				*/
/*	RET:	nothing				*/
/*----------------------------------------------------------------------*/
void nuContGBPakMgrInit(void)
{

    /* Registers the Controller Pak Manager with the SI manager */
    nuSiCallBackAdd(&nuContGBPakCallBack);
}
/*----------------------------------------------------------------------*/
/*	nuContGBPakMgrRemove - Removes the 64GB Pak Manager	*/
/* 	Separates the 64GB Pak Manager from the SI manager.		*/
/*	IN:	nothing							*/
/*	RET:	nothing							*/
/*----------------------------------------------------------------------*/
void nuContGBPakMgrRemove(void)
{
    nuSiCallBackRemove(&nuContGBPakCallBack);

}

/*----------------------------------------------------------------------*/
/*	contGBPakOpen - Checks the 64GB Pak	*/
/*	IN:	mesg	message pointer					*/
/*	RET:	error							*/
/*----------------------------------------------------------------------*/
static s32 contGBPakOpen(NUSiCommonMesg* mesg)    
{
    NUContPakFile*		handle;
    NUContGBPakMesg*	gbpakMesg;

    gbpakMesg = (NUContGBPakMesg*)mesg->dataPtr;
    handle = gbpakMesg->handle;
    return osGbpakInit(&nuSiMesgQ, handle->pfs, handle->pfs->channel);
}

/*----------------------------------------------------------------------*/
/*	contGBPakGetState - Obtains the status of the 64GB Pak			*/
/*	IN:	mesg	message pointer 					*/
/*	RET:	error							*/
/*----------------------------------------------------------------------*/
static s32 contGBPakGetState(NUSiCommonMesg* mesg)
{
    NUContPakFile*		handle;
    NUContGBPakMesg*	gbpakMesg;
    
    gbpakMesg = (NUContGBPakMesg*)mesg->dataPtr;
    handle = gbpakMesg->handle;
    return osGbpakGetStatus(handle->pfs, (u8*)gbpakMesg->data[0]);
}


/*----------------------------------------------------------------------*/
/*	contGBPakPower - Controls 64GB Pak power				*/
/*	IN:	mesg	message pointer				*/
/*	RET:	error							*/
/*----------------------------------------------------------------------*/
static s32 contGBPakPower(NUSiCommonMesg* mesg)
{
    NUContPakFile*	handle;
    NUContGBPakMesg*	gbpakMesg;
    
    gbpakMesg = (NUContGBPakMesg*)mesg->dataPtr;
    handle = gbpakMesg->handle;
    return osGbpakPower(handle->pfs, gbpakMesg->data[0]);
}

/*----------------------------------------------------------------------*/
/*	contGBPakReadID - Reads the ID of the GB cartridge		*/
/*	IN:	mesg	message pointer				*/
/*	RET:	error							*/
/*----------------------------------------------------------------------*/
static s32 contGBPakReadID(NUSiCommonMesg* mesg)
{
    NUContPakFile*		handle;
    NUContGBPakMesg*	gbpakMesg;
    
    gbpakMesg = (NUContGBPakMesg*)mesg->dataPtr;
    handle = gbpakMesg->handle;
    return osGbpakReadId(handle->pfs, (OSGbpakId*)gbpakMesg->data[0],
			 (u8*)gbpakMesg->data[1]);
}
/*----------------------------------------------------------------------*/
/*	contGBPakReadWrite - Reads from and writes to the GB cartridge	*/
/*	IN:	mesg	message pointer				*/
/*	RET:	error							*/
/*----------------------------------------------------------------------*/
static s32 contGBPakReadWrite(NUSiCommonMesg* mesg)
{
    NUContPakFile*	handle;
    NUContGBPakMesg*	gbpakMesg;
    
    gbpakMesg = (NUContGBPakMesg*)mesg->dataPtr;
    handle = gbpakMesg->handle;
    return osGbpakReadWrite(handle->pfs
			    ,(u16)gbpakMesg->data[0]
			    ,(u16)gbpakMesg->data[1]
			    ,(u8*)gbpakMesg->data[2]
			    ,(u16)gbpakMesg->data[3]);
}

/*----------------------------------------------------------------------*/
/*	contGBPakCheckConnector - Detects a poor GB cartridge connection	*/
/*	IN:	mesg	message pointer				*/
/*	RET:	error							*/
/*----------------------------------------------------------------------*/
static s32 contGBPakCheckConnector(NUSiCommonMesg* mesg)
{
    NUContPakFile*	handle;
    NUContGBPakMesg*	gbpakMesg;
    
    gbpakMesg = (NUContGBPakMesg*)mesg->dataPtr;
    handle = gbpakMesg->handle;
    return osGbpakCheckConnector(handle->pfs, (u8*)gbpakMesg->data[0]);
}

