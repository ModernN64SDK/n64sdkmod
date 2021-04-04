/*======================================================================*/
/*		NuSYS							*/
/*		nuvrsmgr.c						*/
/*									*/
/*		Copyright (C) 1999, NINTENDO Co,Ltd.			*/
/*									*/
/*======================================================================*/
/* $Id: nuvrsmgr.c,v 1.1 1999/01/23 05:51:50 ohki Exp $	*/
/*======================================================================*/
#include <nusys.h>

static s32 vrsOpen(NUSiCommonMesg* mesg);
static s32 vrsClearDictionary(NUSiCommonMesg* mesg);
static s32 vrsSetWord(NUSiCommonMesg* mesg);
static s32 vrsMaskDictionary(NUSiCommonMesg* mesg);
static s32 vrsStartReadData(NUSiCommonMesg* mesg);
static s32 vrsGetReadData(NUSiCommonMesg* mesg);
static s32 vrsStopReadData(NUSiCommonMesg* mesg);
static s32 vrsControlGain(NUSiCommonMesg* mesg);

static s32 (*funcList[])(NUSiCommonMesg*) = {
    NULL,			/* NU_VRS_RETRACE_MSG		*/
    vrsOpen,			/* NU_VRS_OPEN_MSG		*/
    vrsClearDictionary,		/* NU_VRS_CLEAR_DICTIONARY_MSG	*/
    vrsSetWord,			/* NU_VRS_SET_WORD_MSG		*/
    vrsMaskDictionary,		/* NU_VRS_MASK_DICTIONARY_MSG	*/
    vrsStartReadData,		/* NU_VRS_START_READ_DATA_MSG	*/
    vrsGetReadData,		/* NU_VRS_GET_READ_DATA_MSG	*/
    vrsStopReadData,		/* NU_VRS_STOP_READ_DATA_MSG	*/
    vrsControlGain,		/* NU_VRS_CONTROL_GAIN_MSG	*/
    NULL			/* End of list			*/     
};

NUCallBackList	nuVrsCallBack = {NULL, funcList, NU_SI_MAJOR_NO_VRS};

/*----------------------------------------------------------------------*/
/*	nuVrsMgrInit - Initializes the voice recognition system		*/
/*									*/
/*	Initializes the Voice Recognition System Manager.			*/
/*	IN:	handle	Structure of type NUVrsHandle				*/
/*	RET:	nothing							*/
/*----------------------------------------------------------------------*/
void nuVrsMgrInit(void)
{
    /*  Register the Voice Recognition System Manager with the SI Manager. */
    nuSiCallBackAdd(&nuVrsCallBack);
}

/*----------------------------------------------------------------------*/
/*	nuVrsMgrRemove -  Removes the Voice Recognition System Manager		*/
/*	Separates the voice recognition Manager from the SI Manager.	*/
/*	IN:	nothing							*/
/*	RET:	nothing							*/
/*----------------------------------------------------------------------*/
void nuVrsMgrRemove(void)
{
    nuSiCallBackRemove(&nuVrsCallBack);
}

/*----------------------------------------------------------------------*/
/*	vrsInit - Initializes the vocabulary registration dictionary.					*/
/*									*/
/*	IN:	mesg	  message pointer				*/
/*	RET:	error							*/
/*----------------------------------------------------------------------*/
static s32 vrsOpen(NUSiCommonMesg* mesg)
{
    NUVrsMesg* vrsMesg;

    vrsMesg = (NUVrsMesg*)mesg->dataPtr;
    return osVoiceInit(&nuSiMesgQ, vrsMesg->vrsHandle, vrsMesg->data[0]);
}

/*----------------------------------------------------------------------*/
/*	vrsClearDictionary - vocabulary registration dictionaryinitialization			*/
/*									*/
/*	IN:	mesg	  message pointer				*/
/*	RET:	error							*/
/*----------------------------------------------------------------------*/
static s32 vrsClearDictionary(NUSiCommonMesg* mesg)
{
    NUVrsMesg* vrsMesg;
    vrsMesg = (NUVrsMesg*)mesg->dataPtr;
    
    return osVoiceClearDictionary(vrsMesg->vrsHandle, vrsMesg->data[0]);
}

/*----------------------------------------------------------------------*/
/*	vrsSetWord - Registers terms in the voice recognition system dictionary		*/
/*									*/
/*	IN:	mesg	  message pointer				*/
/*	RET:	error							*/
/*----------------------------------------------------------------------*/
static s32 vrsSetWord(NUSiCommonMesg* mesg)
{
    NUVrsMesg* vrsMesg;
    vrsMesg = (NUVrsMesg*)mesg->dataPtr;
    return osVoiceSetWord(vrsMesg->vrsHandle, (u8*)vrsMesg->data[0]);
}

/*----------------------------------------------------------------------*/
/*	vrsMaskDictionary - Toggles on/off recognition of terms in the dictionary */
/*									*/
/*	IN:	mesg	  message pointer				*/
/*	RET:	error							*/
/*----------------------------------------------------------------------*/
static s32 vrsMaskDictionary(NUSiCommonMesg* mesg)
{
    NUVrsMesg* vrsMesg;
    vrsMesg = (NUVrsMesg*)mesg->dataPtr;
    return osVoiceMaskDictionary(vrsMesg->vrsHandle,
				 (u8*)vrsMesg->data[0], (int)vrsMesg->data[1]);
}

/*----------------------------------------------------------------------*/
/*	vrsStartReadData - Starts voice recognition		*/
/*									*/
/*	IN:	mesg	  message pointer				*/
/*	RET:	error							*/
/*----------------------------------------------------------------------*/
static s32 vrsStartReadData(NUSiCommonMesg* mesg)
{
    return osVoiceStartReadData((NUVrsHandle*)mesg->dataPtr);
}

/*----------------------------------------------------------------------*/
/*	vrsGetReadData - Obtains the results of voice recognition */
/*									*/
/*	IN:	mesg	  message pointer				*/
/*	RET:	error							*/
/*----------------------------------------------------------------------*/
static s32 vrsGetReadData(NUSiCommonMesg* mesg)
{
    NUVrsMesg* vrsMesg;
    vrsMesg = (NUVrsMesg*)mesg->dataPtr;
    return osVoiceGetReadData(vrsMesg->vrsHandle,
			      (OSVoiceData*)vrsMesg->data[0]);
}

/*----------------------------------------------------------------------*/
/*	vrsStopReadData - Forcibly stops voice recognition processing	*/
/*									*/
/*	IN:	mesg	  message pointer				*/
/*	RET:	error							*/
/*----------------------------------------------------------------------*/
static s32 vrsStopReadData(NUSiCommonMesg* mesg)
{

    return osVoiceStopReadData((OSVoiceHandle*)mesg->dataPtr);
}

/*----------------------------------------------------------------------*/
/*	vrsControlGain - Adjusts the input gain of the voice recognition system */
/*									*/
/*	IN:	mesg	  message pointer				*/
/*	RET:	error							*/
/*----------------------------------------------------------------------*/
static s32 vrsControlGain(NUSiCommonMesg* mesg)
{
    NUVrsMesg* vrsMesg;
    vrsMesg = (NUVrsMesg*)mesg->dataPtr;
    return osVoiceControlGain(vrsMesg->vrsHandle,
			      vrsMesg->data[0], vrsMesg->data[1]);
}



