/*======================================================================*/
/*		NuSYS							*/
/*		nucontrmbmgr.c						*/
/*									*/
/*		Copyright (C) 1997, NINTENDO Co,Ltd.			*/
/*									*/
/*----------------------------------------------------------------------*/    
/* Version sion sion sion  1.0	97/10/9		Created by Kensaku Ohki(SLANP)		*/
/*======================================================================*/
/* $Id: nucontrmbmgr.c,v 1.14 1999/01/23 05:44:22 ohki Exp $	*/
/*======================================================================*/
#include <nusys.h>

NUContRmbCtl	nuContRmbCtl[NU_CONT_MAXCONTROLLERS];
u32		nuContRmbSearchTime = NU_CONT_RMB_AUTO_SEARCHTIME;


static s32 contRmbRetrace(NUSiCommonMesg* mesg);
static s32 contRmbCheckMesg(NUSiCommonMesg* mesg);
static s32 contRmbStartMesg(NUSiCommonMesg* mesg);
static s32 contRmbStopMesg(NUSiCommonMesg* mesg);
static s32 contRmbForceStopMesg(NUSiCommonMesg* mesg);
static s32 contRmbForceStopEndMesg(NUSiCommonMesg* mesg);

static s32 (*funcList[])(NUSiCommonMesg*) = {
    contRmbRetrace,		/* NU_CONT_RMB_RETRACE_MSG	*/
    contRmbCheckMesg,	/* NU_CONT_RMB_CHECK_MSG	*/
    contRmbStartMesg,    	/* NU_CONT_RMB_START_MSG	*/
    contRmbStopMesg,		/* NU_CONT_RMB_STOP_MSG		*/
    contRmbForceStopMesg,    /* NU_CONT_RMB_FORCESTOP_MSG	*/
    contRmbForceStopEndMesg,	/* NU_CONT_RMB_FORCESTOPEND_MSG	*/
    NULL			/* End of list			*/    
};

NUCallBackList	nuContRmbCallBack = {NULL, funcList, NU_SI_MAJOR_NO_RMB};

/*----------------------------------------------------------------------*/
/*	nuContRmbMgrInit - Initializes the Rumble Pak Manager		*/
/*	IN:	nothing							*/
/*	RET:	nothing							*/
/*----------------------------------------------------------------------*/
void nuContRmbMgrInit(void)
{
    u32 cnt;

    /* Initializes the Rumble Pak Control structure. */
    for(cnt = 0; cnt <NU_CONT_MAXCONTROLLERS; cnt++){
	nuContRmbCtl[cnt].state = NU_CONT_RMB_STATE_STOPPED;
	nuContRmbCtl[cnt].mode	= NU_CONT_RMB_MODE_DISABLE;
	nuContRmbCtl[cnt].counter = cnt;
    }
    nuSiCallBackAdd(&nuContRmbCallBack);
}
/*----------------------------------------------------------------------*/
/*	nuContRmbMgrRemove - Removes the Rumble Pak Manager	*/
/*	IN:	nothing							*/
/*	RET:	nothing							*/
/*----------------------------------------------------------------------*/
void nuContRmbMgrRemove(void)
{
    nuSiCallBackRemove(&nuContRmbCallBack);
}

/*----------------------------------------------------------------------*/
/*	contRmbControl - Rumble Pak Control 				*/
/*	IN:	rmtCtl	Pointer to the Rumble Pak Control structure 	*/
/*		contNo	Controller number 				*/
/*	RET:	error							*/
/*----------------------------------------------------------------------*/
static s32 contRmbControl(NUContRmbCtl* rmbCtl, u32 contNo)
{
    s32	rtn = 0;
    u32	integer;

    /* Control the Rumble Pak status.		*/
    switch(rmbCtl->state){
    case NU_CONT_RMB_STATE_STOPPED:	/* Motor stopped. */
	break;
    case NU_CONT_RMB_STATE_STOPPING:
	/* To stop the motor, osMotorStop must be executed for 3 frames; */
	/* this processing is implemented.				   */
	if(rmbCtl->counter > 0){
	    rtn = osMotorStop(&nuContPfs[contNo]);
	} else {
	    rmbCtl->state = NU_CONT_RMB_STATE_STOPPED;
	}
	rmbCtl->counter--;
	break;
    case NU_CONT_RMB_STATE_RUN:	/* Run the motor to match the frequency. */
	if(rmbCtl->frame >0){
	    rmbCtl->counter += rmbCtl->freq;
	    integer = rmbCtl->counter >> 8;
	    rmbCtl->counter &= 0x00ff;
	    if( integer > 0){
		rtn = osMotorStart(&nuContPfs[contNo]);
	    } else {
		rtn = osMotorStop(&nuContPfs[contNo]);
	    }
	} else {
	    rtn = osMotorStop(&nuContPfs[contNo]);
	    rmbCtl->state = NU_CONT_RMB_STATE_STOPPING;
	    rmbCtl->counter = 2;
	}
	rmbCtl->frame--;
	break;
    case NU_CONT_RMB_STATE_FORCESTOP:	/* Force a stop. */
	rtn = osMotorInit(&nuSiMesgQ, &nuContPfs[contNo], contNo);
	if(!rtn){
	    osMotorStop(&nuContPfs[contNo]);
	}
	rmbCtl->state = NU_CONT_RMB_STATE_STOPPING;
	rmbCtl->counter = 2;
	break;
    default:
	break;
    }
    return rtn;
}

/*----------------------------------------------------------------------*/
/*	contRmbRetrace - Controls the Rumble Pak by retrace		*/
/*	IN:	mesg	message pointer					*/
/*	RET:	NU_SI_CALLBACK_CONTINUE					*/
/*----------------------------------------------------------------------*/
static s32 contRmbRetrace(NUSiCommonMesg* mesg)
{
    u32		cnt;
    s32		rtn;
    u32 	counter;
    NUContRmbCtl*  rmbCtl;
    
    counter = nuScRetraceCounter  % nuContRmbSearchTime;
    for(cnt = 0; cnt < NU_CONT_MAXCONTROLLERS; cnt++){
	rmbCtl = &nuContRmbCtl[cnt];
	
	switch(rmbCtl->mode){
	case NU_CONT_RMB_MODE_DISABLE:	/* Disable Rumble Pak use. */
	    break;
	    
	case NU_CONT_RMB_MODE_ENABLE:	/* Enable Rumble Pak use. */
	    rtn = contRmbControl(rmbCtl, cnt);	/* Rumble Pak Control */

	    /* Disable when an error occurs. */
	    if(rtn){
		rmbCtl->mode = NU_CONT_RMB_MODE_DISABLE;
	    }
	    break;
	    
	case NU_CONT_RMB_MODE_AUTORUN:  /* Automatically identify the Rumble Pak. */

	    if(rmbCtl->autorun == NU_CONT_RMB_AUTO_SEARCH){
		/* In search mode, check whether there is a	*/
		/* Rumble Pak if the counter value is reached. */
		counter =  rmbCtl->counter % nuContRmbSearchTime;
		if(!counter){
		    rtn = osMotorInit(&nuSiMesgQ, &nuContPfs[cnt], cnt);
		    if(!rtn) {
			rmbCtl->autorun = NU_CONT_RMB_AUTO_FIND;
			rmbCtl->type = NU_CONT_PAK_TYPE_RUMBLE;
		    }
		}
		rmbCtl->counter++;
	    } else {
		/* When a Rumble Pak is present, perform same processing as for enabled state. */
		/* But if an error occurs, shift to Search mode.*/
		    rtn = contRmbControl(rmbCtl, cnt);
		if(rtn){
		    rmbCtl->counter = cnt;
		    rmbCtl->autorun = NU_CONT_RMB_AUTO_SEARCH;
		    rmbCtl->type = NU_CONT_PAK_TYPE_NONE;
		}
	    }
	    break;
	case (NU_CONT_RMB_MODE_ENABLE | NU_CONT_RMB_MODE_PAUSE):
	case (NU_CONT_RMB_MODE_AUTORUN | NU_CONT_RMB_MODE_PAUSE):
	    if(rmbCtl->type == NU_CONT_PAK_TYPE_RUMBLE){
		rtn = contRmbControl(rmbCtl, cnt);
	    }
	    break;
	default:
	    break;
	    
	}
    }
    return NU_SI_CALLBACK_CONTINUE;
}

/*----------------------------------------------------------------------*/
/*	contRmbCheckMesg - Rumble Pak check 				*/
/*	IN:	mesg	message pointer				*/
/*	RET:	error							*/
/*----------------------------------------------------------------------*/
static s32 contRmbCheckMesg(NUSiCommonMesg* mesg)
{
    NUContRmbMesg* rmbMesg;
    
    rmbMesg = (NUContRmbMesg*)mesg->dataPtr;
    return osMotorInit(&nuSiMesgQ, &nuContPfs[rmbMesg->contNo], rmbMesg->contNo);

}

/*----------------------------------------------------------------------*/
/*	contRmbForceStopMesg - Forces Rumble Pak to stop			*/
/*	IN:	mesg	message pointer				*/
/*	RET:	error							*/
/*----------------------------------------------------------------------*/
static s32 contRmbForceStopMesg(NUSiCommonMesg* mesg)    
{
    u32 cnt;
    for(cnt = 0; cnt < NU_CONT_MAXCONTROLLERS; cnt++){
	nuContRmbCtl[cnt].state = NU_CONT_RMB_STATE_FORCESTOP;
	nuContRmbCtl[cnt].mode |= NU_CONT_RMB_MODE_PAUSE;
    }
    return 0;
}
/*----------------------------------------------------------------------*/
/*	contRmbForceStopEndMesg - Cancels forced stop of the Rumble Pak		*/
/*	IN:	mesg	message pointer				*/
/*	RET:	error							*/
/*----------------------------------------------------------------------*/
static s32 contRmbForceStopEndMesg(NUSiCommonMesg* mesg)    
{
    u32 cnt;
    for(cnt = 0; cnt < NU_CONT_MAXCONTROLLERS; cnt++){
	nuContRmbCtl[cnt].mode &= 0x7f;
    }
    return 0;
}

/*----------------------------------------------------------------------*/
/*	contRmbStartMesg - Starts the Rumble Pak				*/
/*	IN:	mesg	message pointer					*/
/*	RET:	error							*/
/*----------------------------------------------------------------------*/
static s32 contRmbStartMesg(NUSiCommonMesg* mesg)    
{
    NUContRmbMesg*	rmbMesg;
    NUContRmbCtl*	rmbCtlData;

    rmbMesg = (NUContRmbMesg*)mesg->dataPtr;
    rmbCtlData = (NUContRmbCtl*)rmbMesg->data;
    
    nuContRmbCtl[rmbMesg->contNo].state   = rmbCtlData->state;
    nuContRmbCtl[rmbMesg->contNo].frame   = rmbCtlData->frame;
    nuContRmbCtl[rmbMesg->contNo].freq    = rmbCtlData->freq;
    nuContRmbCtl[rmbMesg->contNo].counter = 0;
    
    return 0;
}

/*----------------------------------------------------------------------*/
/*	contRmbStopMesg - Stops the Rumble Pak				*/
/*	IN:	mesg	message pointer				*/
/*	RET:	error							*/
/*----------------------------------------------------------------------*/
static s32 contRmbStopMesg(NUSiCommonMesg* mesg)
{
    NUContRmbMesg*	rmbMesg;

    rmbMesg = (NUContRmbMesg*)mesg->dataPtr;
    nuContRmbCtl[rmbMesg->contNo].frame	= 0;
    
    return 0;   
}
