/*======================================================================*/
/*		NuSYS							*/
/*		nucontpakmgr.c						*/
/*									*/
/*		Copyright (C) 1997, NINTENDO Co,Ltd.			*/
/*									*/
/*----------------------------------------------------------------------*/    
/* Version  1.0	97/10/9		Created by Kensaku Ohki(SLANP)		*/
/*======================================================================*/
/* $Id: nucontpakmgr.c,v 1.20 1999/01/25 06:06:38 ohki Exp $	*/
/*======================================================================*/
#include <nusys.h>

u16		nuContPakCompanyCode = 0;	/* Controller Pak company code */
u32		nuContPakGameCode = 0;	/* Controller Pak game code */

static s32 contPakOpen(NUSiCommonMesg* mesg);
static s32 contPakFree(NUSiCommonMesg* mesg);
static s32 contPakFileReadWrite(NUSiCommonMesg* mesg);
static s32 contPakFileOpen(NUSiCommonMesg* mesg);
static s32 contPakFileDelete(NUSiCommonMesg* mesg);
static s32 contPakFileState(NUSiCommonMesg* mesg);
static s32 contPakFileNum(NUSiCommonMesg* mesg);
static s32 contPakRepairId(NUSiCommonMesg* mesg);

static s32 (*funcList[])(NUSiCommonMesg*) = {
    NULL,			/* NU_CONT_PAK_REPEAT_MSG	*/
    contPakOpen,		/* NU_CONT_PAK_OPEN_MSG 	*/
    contPakFree,		/* NU_CONT_PAK_FREE_MSG		*/
    contPakFileOpen,    	/* NU_CONT_PAK_FOPEN_MSG	*/
    contPakFileReadWrite,	/* NU_CONT_PAK_READWRITE_MSG	*/
    contPakFileDelete,		/* NU_CONT_PAK_FILEDELETE_MSG	*/
    contPakFileState,		/* NU_CONT_PAK_FILESTATE_MSG	*/
    contPakFileNum,		/* NU_CONT_PAK_FILENUM_MSG	*/
    contPakRepairId,		/* NU_CONT_PAK_REPAIRID_MSG	*/
    NULL			/* End of list			*/    
};

NUCallBackList	nuContPakCallBack = {NULL, funcList, NU_SI_MAJOR_NO_PAK};

/*----------------------------------------------------------------------*/
/*	nuContPakMgrInit - Initializes the Controller Pak Manager		*/
/*	IN:	nothing							*/
/*	RET:	nothing							*/
/*----------------------------------------------------------------------*/
void nuContPakMgrInit(void)
{
    /* Registers the Controller Pak Manager with the SI Manager.	*/
    nuSiCallBackAdd(&nuContPakCallBack);
}
/*----------------------------------------------------------------------*/
/*	nuContPakMgrRemove - Removes the Controller Pak Manager		*/
/* Separates Controller Pak Manager from SI Manager.			*/
/*	IN:	nothing							*/
/*	RET:	nothing							*/
/*----------------------------------------------------------------------*/
void nuContPakMgrRemove(void)
{
    nuSiCallBackRemove(&nuContPakCallBack);
}

/*----------------------------------------------------------------------*/
/*	contPakOpen - Opens the Controller Pak				*/
/* 	Checks the device inserted in the Controller connector, 		*/
/*	and initializes it so that it can be used.				*/
/*	IN:	*mesg	message pointer					*/
/*	RET:	error							*/
/*----------------------------------------------------------------------*/
static s32 contPakOpen(NUSiCommonMesg* mesg)
{
    s32			rtn;
    NUContPakFile*	file;

    
    file = (NUContPakFile*)mesg->dataPtr;
    file->type = NU_CONT_PAK_TYPE_NONE;

    
    /*	Initializes the file handle.					*/
    /* 	Time required for PfsInitPak					*/
    rtn = osPfsInitPak(&nuSiMesgQ, file->pfs, file->pfs->channel);
    
    if(rtn == 0) {
	/* Contains the Controller Pak if no error. */
	file->type = NU_CONT_PAK_TYPE_PAK;
    }
    return rtn;
}

/*----------------------------------------------------------------------*/
/*	contPakFree - Obtains the amount of free area in the Controller Pak */
/*	IN:	*mesg	message pointer					*/
/*	RET:	error							*/
/*----------------------------------------------------------------------*/
static s32 contPakFree(NUSiCommonMesg* mesg)
{
    NUContPakFreeMesg* free;
    
    free = (NUContPakFreeMesg*)mesg->dataPtr;
    return osPfsFreeBlocks(free->file->pfs, &free->size);
}

/*----------------------------------------------------------------------*/
/*	contPakFileReadWrite - Obtains the amount of free area in the Controller Pak	*/
/*	IN:	*mesg	message pointer				*/
/*	RET:	error							*/
/*----------------------------------------------------------------------*/
static s32 contPakFileReadWrite(NUSiCommonMesg* mesg)    
{
    NUContPakFileRWMesg* fileRW;
    NUContPakFile*	file;
    
    fileRW = (NUContPakFileRWMesg*)mesg->dataPtr;
    file = fileRW->file;
    return osPfsReadWriteFile(file->pfs, file->file_no, fileRW->mode,
			      fileRW->offset, fileRW->size, fileRW->buf);
}

/*----------------------------------------------------------------------*/
/*	contPakFileOpen - Opens the Controller Pak file		*/
/*	IN:	*mesg	message pointer				*/
/*	RET:	error						*/
/*----------------------------------------------------------------------*/
static s32 contPakFileOpen(NUSiCommonMesg* mesg)    
{
    s32			rtn;
    NUContPakFile*	file;
    NUContPakFileOpenMesg* openMesg;
    
    openMesg = (NUContPakFileOpenMesg*)mesg->dataPtr;
    file = openMesg->file;
    rtn = osPfsFindFile(file->pfs, nuContPakCompanyCode, nuContPakGameCode,
		  openMesg->noteName, openMesg->extName, &file->file_no);

    /* If there is no file and the mode is NU_CONT_PAK_MODE_CRATE */
    if((rtn == PFS_ERR_INVALID)
       && (openMesg->mode == NU_CONT_PAK_MODE_CREATE)){
	rtn = osPfsAllocateFile(file->pfs,
				nuContPakCompanyCode, nuContPakGameCode,
				openMesg->noteName, openMesg->extName,
				openMesg->size, &file->file_no);
    }
    return rtn;
}

/*----------------------------------------------------------------------*/
/*	contPakFileDelte - Deletes the file				*/
/*	IN:	*mesg	message pointer				*/
/*	RET:	error							*/
/*----------------------------------------------------------------------*/
static s32 contPakFileDelete(NUSiCommonMesg* mesg)   
{
    
    NUContPakFile*	file;
    NUContPakFileOpenMesg* deleteMesg;
    
    deleteMesg = (NUContPakFileOpenMesg*)mesg->dataPtr;
    file = (NUContPakFile*)deleteMesg->file;
    
    return osPfsDeleteFile(file->pfs,
			   nuContPakCompanyCode, nuContPakGameCode,
			   deleteMesg->noteName, deleteMesg->extName);
}

/*----------------------------------------------------------------------*/
/*	contPakFileState - Obtains file information			*/
/*	IN:	*mesg	message pointer				*/
/*	RET:	error							*/
/*----------------------------------------------------------------------*/
static s32 contPakFileState(NUSiCommonMesg* mesg)    
{
    
    NUContPakFileStateMesg* stateMesg;
    NUContPakFile*	file;

    stateMesg = (NUContPakFileStateMesg*)mesg->dataPtr;
    file = stateMesg->file;
    return osPfsFileState(file->pfs, file->file_no, stateMesg->state);
}

/*----------------------------------------------------------------------*/
/*	contPakFileNum - Obtains the number of Pak notes				*/
/*	IN:	*mesg	message pointer				*/
/*	RET:	error							*/
/*----------------------------------------------------------------------*/
static s32 contPakFileNum(NUSiCommonMesg* mesg)    
{
    NUContPakFileNumMesg* numMesg;
    NUContPakFile*	file;

    numMesg = (NUContPakFileNumMesg*)mesg->dataPtr;
    file = numMesg->file;
    return osPfsNumFiles(file->pfs, numMesg->max_files, numMesg->used_files);
}


/*----------------------------------------------------------------------*/
/*	contPakRepairId - Repairs the Pak file system		*/
/*	IN:	*mesg	message pointer				*/
/*	RET:	error						*/
/*----------------------------------------------------------------------*/
static s32 contPakRepairId(NUSiCommonMesg* mesg)    
{
    NUContPakFile*	file;

    file = (NUContPakFile*)mesg->dataPtr;
    return osPfsRepairId(file->pfs);
}

