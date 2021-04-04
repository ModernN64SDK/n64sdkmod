/*======================================================================*/
/*		NuSYS							*/
/*		nueeprommgr.c						*/
/*									*/
/*		Copyright (C) 1997, NINTENDO Co,Ltd.			*/
/*									*/
/*----------------------------------------------------------------------*/
/* Ver 1.1	97/12/4		Created by Kensaku Ohki(SLANP)		*/
/*======================================================================*/
/* $Id: nueeprommgr.c,v 1.4 1999/01/23 05:46:05 ohki Exp $		*/
/*======================================================================*/
#include <nusys.h>

static s32 eepromCheck(NUSiCommonMesg* mesg);
static s32 eepromRead(NUSiCommonMesg* mesg);
static s32 eepromWrite(NUSiCommonMesg* mesg);

static s32 (*funcList[])(NUSiCommonMesg*) = {
    NULL,			/* NU_EEPROM_RETRACE_MSG	*/
    eepromCheck,		/* NU_EEPROM_CHECK_MSG		*/
    eepromRead,		/* NU_EEPROM_READ_MSG		*/
    eepromWrite,		/* NU_EEPROM_WRITE_MSG		*/
    NULL			/* End of list				*/     
};

NUCallBackList	nuEepromCallBack = {NULL, funcList, NU_SI_MAJOR_NO_EEPROM};

/*----------------------------------------------------------------------*/
/*	nuEepromMgrInit - Initializes EEPROMManager			*/
/*									*/
/*	Registers EEPROMManager with SI Manager.			*/
/*									*/
/*	IN:	nothing							*/
/*	RET:   nothing							*/
/*----------------------------------------------------------------------*/
void nuEepromMgrInit(void)
{
    nuSiCallBackAdd(&nuEepromCallBack);
}

/*----------------------------------------------------------------------*/
/*	nuEepromMgrInit - Removes EEPROMManager			*/
/*	Separates EEPROMManager from SI Manager.			*/
/*	IN:	nothing							*/
/*	RET:   nothing							*/
/*----------------------------------------------------------------------*/
void nuEepromMgrRemove(void)
{
    nuSiCallBackRemove(&nuEepromCallBack);
}

/*----------------------------------------------------------------------*/
/*	nuEepromRead - Reads data from EEPROM			*/
/* Reads in data from EEPROM.						*/
/* Synchronous -- does not return until data read is finished.  */
/* But the maximum read from EEPROM is approx. 1 second . . .		*/
/*	IN:	address		EEPROM address to read data from			*/
/*		buffer		Pointer to buffer to store data	*/
/*		len		Data length					*/
/*	RET:   	0 	Normal termination					*/
/*		-1	Parameter is strange or EEPROM Manager has not been registered	*/
/*		8	Interface abnormality			*/
/*----------------------------------------------------------------------*/
s32 nuEepromRead(u8 address, u8* buffer,u16 len)
{
    NUEepromMesg	eepromMesg;
    eepromMesg.address	= address;
    eepromMesg.dataPtr	= (void*)buffer;
    eepromMesg.dataLen	= len;
    return nuSiSendMesg(NU_EEPROM_READ_MSG, (void*)&eepromMesg);
}

/*----------------------------------------------------------------------*/
/*	nuEepromWrite - Writes data to EEPROM			*/
/* Writes data to EEPROM.					*/
/* Synchronous -- does not return until write is finished.  			*/
/* But the maximum write to EEPROM is approx. 1 second . . .		*/
/*	IN:	address	EEPROM address where data are stored		*/
/*		buffer	Pointer with data		*/
/*		len	Data length					*/
/*	RET:   	0 	Normal termination					*/
/*		-1	Parameter strange or EEPROMManager has not been registered */
/*		8	Interface abnormality				*/
/*----------------------------------------------------------------------*/
s32 nuEepromWrite(u8 address, u8* buffer,u16 len)
{
    NUEepromMesg	eepromMesg;
    eepromMesg.address	= address;
    eepromMesg.dataPtr	= (void*)buffer;
    eepromMesg.dataLen	= len;
    return nuSiSendMesg(NU_EEPROM_WRITE_MSG, (void*)&eepromMesg);
}

/*----------------------------------------------------------------------*/
/*	nuEepromCheck - Detects EEPROM					*/
/* Checks whether EEPROM is present.					*/
/*	IN:	nothing							*/
/*	RET:   	EEPROM_TYPE_4K	4K EEPROM				*/
/* 		EEPROM_TYPE_16K	16K EEPROM				*/
/*		0		EEPROMnothing				*/
/*		-1		EEPROMManager not registered. */
/*----------------------------------------------------------------------*/
s32 nuEepromCheck(void)
{
    return  nuSiSendMesg(NU_EEPROM_CHECK_MSG, (void*)NULL);
}
    
/*----------------------------------------------------------------------*/
/*	eepromCheck - EEPROMcheck 					*/
/*	IN:	mesg	message pointer				*/
/*	RET:	error							*/
/*----------------------------------------------------------------------*/
static s32 eepromCheck(NUSiCommonMesg* mesg)
{
    return  osEepromProbe(&nuSiMesgQ);
}

/*----------------------------------------------------------------------*/
/*	eepromRead - Reads from EEPROM			*/
/*	Reads data from EEPROM.				*/
/*	IN:	mesg	message pointer				*/
/*	RET:	error							*/
/*----------------------------------------------------------------------*/
static s32 eepromRead(NUSiCommonMesg* mesg)    
{
    NUEepromMesg* eepromMesg;
    eepromMesg = (NUEepromMesg*)mesg->dataPtr;
    return osEepromLongRead(&nuSiMesgQ, eepromMesg->address,
			     eepromMesg->dataPtr, eepromMesg->dataLen);
}

/*----------------------------------------------------------------------*/
/*	eepromWrite - Writes to EEPROM				*/
/*	Writes data to EEPROM.				*/
/*	IN:	mesg	message pointer				*/
/*	RET:	error							*/
/*----------------------------------------------------------------------*/
static s32 eepromWrite(NUSiCommonMesg* mesg)
{
    NUEepromMesg* eepromMesg;
    eepromMesg = (NUEepromMesg*)mesg->dataPtr;    
    return  osEepromLongWrite(&nuSiMesgQ, eepromMesg->address,
			       eepromMesg->dataPtr, eepromMesg->dataLen);
}
