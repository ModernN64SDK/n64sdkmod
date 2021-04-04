/*======================================================================*/
/*		NuSYS							*/
/*		nucontmgr.c						*/
/*									*/
/*		Copyright (C) 1997, NINTENDO Co,Ltd.			*/
/*									*/
/*----------------------------------------------------------------------*/
/* Ver 1.0	97/10/9		Created by Kensaku Ohki(SLANP)		*/
/*======================================================================*/
/* $Id: nucontmgr.c,v 1.14 1999/07/02 05:57:41 ohki Exp $		*/
/*======================================================================*/
#include <nusys.h>

OSMesgQueue	nuContWaitMesgQ;	/* Queue for Controller read wait */
static OSMesg	nuContWaitMesgBuf;
OSMesgQueue	nuContDataMutexQ;
static OSMesg	nuContDataMutexBuf;	/* Semaphore used in accessing data */
OSContStatus	nuContStatus[NU_CONT_MAXCONTROLLERS];
OSContPad	nuContData[NU_CONT_MAXCONTROLLERS];
u32		nuContNum;
u32		nuContDataLockKey;

NUContReadFunc	nuContReadFunc = NULL;	/* Callback function for */
							/* end of Controller read */
OSPfs		nuContPfs[NU_CONT_MAXCONTROLLERS]; /* pfs structure for Controller Manager */

static s32 contRetrace(NUSiCommonMesg* mesg);
static s32 contRead(NUSiCommonMesg* mesg);
static s32 contReadNW(NUSiCommonMesg* mesg);
static s32 contQuery(NUSiCommonMesg* mesg);

static s32 (*funcList[])(NUSiCommonMesg*) = {
    contRetrace,			/* NU_CONT_RETRACE_MSG		*/
    contRead,			/* NU_CONT_READ_MSG		*/
    contReadNW,		/* NU_CONT_READ_NW_MSG		*/
    contQuery,			/* NU_CONT_QUERY_MSG		*/
    NULL			/* End of list	*/
};

NUCallBackList	nuContCallBack = {NULL,funcList, NU_SI_MAJOR_NO_CONT};
				  
/*----------------------------------------------------------------------*/
/*	nuContMgrInit - Initializes the Controller Manager	*/
/*	Initializes the Controller Manager and registers it with the SI manager. */
/*	For compatibility with previous versions, this function generates and returns a bit */
/*	pattern.								*/
/*									*/
/*	IN:	nothing							*/
/*	RET:	the bit pattern of the connected Controller			*/
/*----------------------------------------------------------------------*/
u8 nuContMgrInit(void)
{
    int	cnt;
    u8	pattern;
    u8	bitmask;
    
    /* Removes the stop flag for a Controller data read.  */
    nuContDataUnLock();
    
    /* The Controller data read wait queue */
    osCreateMesgQueue(&nuContWaitMesgQ, &nuContWaitMesgBuf, 1);
    
    /* Creates a sephamore for Controller data access. */
    osCreateMesgQueue(&nuContDataMutexQ, &nuContDataMutexBuf, 1);
    
    /* Registers the Controller Manager with the SI manager. */
    nuSiCallBackAdd(&nuContCallBack);
    
    /* Gets the status. */
    nuContQueryRead();
    
    /* Checks the number of Controllers connected. */
    /* Only checks, however.	*/
    nuContNum = 0;
    bitmask = 1;
    pattern = 0;
    

    for(cnt = 0; cnt < NU_CONT_MAXCONTROLLERS; 	bitmask <<= 1, cnt++){
	
	/* Error check.  Not used if error occurred.  */
	if(nuContStatus[cnt].errno) continue;
	
	/* Checks whether the flags for both CONT_ABUSOLUTE and CONT_JOYPORT are set. */
	if((nuContStatus[cnt].type & CONT_TYPE_MASK) == CONT_TYPE_NORMAL){
	    nuContNum++;
	    pattern |= bitmask;
	}

    }
    return pattern;
}
/*----------------------------------------------------------------------*/
/*	nuContMgrRemove - Removes the Controller Manager		*/
/* Separates the Controller Manager from the SI manager.			 */
/*	IN:	nothing							*/
/*	RET:	nothing							*/
/*----------------------------------------------------------------------*/
void nuContMgrRemove(void)
{
    nuSiCallBackRemove(&nuContCallBack);
}

/*----------------------------------------------------------------------*/
/*	nuContDataClose - Closes the Controller data			*/
/*	Prevents other threads from accessing nuContData.			 */
/*	IN:	nothing							*/
/*	RET:	nothing							*/
/*----------------------------------------------------------------------*/
void nuContDataClose(void)
{
    osSendMesg(&nuContDataMutexQ, NULL, OS_MESG_BLOCK);

}

/*----------------------------------------------------------------------				*/
/*	nuContDataOpen - Opens Controller data					*/
/*	Enables nuContData to be accessed by other threads.				*/
/*	IN:	nothing								*/
/*	RET:	nothing								*/
/*----------------------------------------------------------------------				*/
void nuContDataOpen(void)
{
    osRecvMesg(&nuContDataMutexQ, NULL, OS_MESG_BLOCK);
}
	
/*----------------------------------------------------------------------*/
/*	contReadData - Reads Controller data (internal function)			*/
/*	IN:	lockflag		A flag that checks whether the data are locked.	*/
/*				0, no check performed (forced-read mode)		*/
/*				1, perform check					*/
/*	RET:	error								*/
/*----------------------------------------------------------------------*/
static s32 contReadData(OSContPad *pad,u32 lockflag)
{

    s32 rtn;
    
    /* Start Controller read.  */
    rtn = osContStartReadData(&nuSiMesgQ);
    if(rtn) return rtn;
    
    /* Wait for end of Controller read (approx. 2ms) */
    osRecvMesg(&nuSiMesgQ, NULL, OS_MESG_BLOCK);

    /* Check whether data are locked. */
    /* Used in cases where the thread is switched by above-mentioned osRecvMesg, */
    /* and the data are locked by that thread.*/
    if(lockflag & nuContDataLockKey) return rtn;
    
    /* Get Controller data. */
    nuContDataClose();
    osContGetReadData(pad);
    nuContDataOpen();
    
    return rtn;
}

/*----------------------------------------------------------------------*/
/*	contQuery - Obtains the Controller status			*/
/*	IN:	*mesg	Pointer to a structure of the same type as NUContQueryMesg */
/*	RET:	nothing							*/
/*----------------------------------------------------------------------*/
static s32 contQuery(NUSiCommonMesg* mesg)
{
    s32 rtn;
    
    /* Start obtaining Controller status */
    rtn = osContStartQuery(&nuSiMesgQ);
    if(rtn) return rtn;
    
    /* Wait for status to be obtained. */
    osRecvMesg(&nuSiMesgQ, NULL, OS_MESG_BLOCK);
    
    /* Read the obtained data. */
    osContGetQuery(nuContStatus);
    
    return rtn;
}

/*----------------------------------------------------------------------*/
/*	contRetrace - Retrace processing					*/
/*									*/
/*	IN:	mesg	Message					*/
/*	RET:	nothing							*/
/*----------------------------------------------------------------------*/
static s32 contRetrace(NUSiCommonMesg* mesg)
{
    /* Do not read the data if locked. */
    if(nuContDataLockKey) {
	return NU_SI_CALLBACK_CONTINUE;
    }
    /* Clear the wait message queue.  */
    osRecvMesg(&nuContWaitMesgQ, NULL, OS_MESG_NOBLOCK);
    
    /* Controller read */
    contReadData(nuContData, 1);
    
    /* Call the callback function after read ends.			*/
    /* Because the priority of this thread is second to that of the event handler,	*/
    /* processing by the callback function should be held to the minimum required.*/
    /* The parameter is *msg_type.						*/
    if(nuContReadFunc != NULL){
	(*nuContReadFunc)(mesg->mesg);
    }
    
    /* Send message indicating end of read. */
    osSendMesg(&nuContWaitMesgQ, NULL, OS_MESG_NOBLOCK);
    
    return NU_SI_CALLBACK_CONTINUE;
}

/*----------------------------------------------------------------------*/
/*	contRead - Controller read processing 				*/
/*									*/
/*	IN:	mesg	message						*/
/*	RET:	nothing							*/
/*----------------------------------------------------------------------*/
static s32 contRead(NUSiCommonMesg* mesg)
{
	return contReadData((OSContPad*)mesg->dataPtr, 0);
}

/*----------------------------------------------------------------------*/
/*	contReadNW - Asynchronous Controller read				*/
/*									*/
/*	IN:	mesg	message					*/
/*	RET:	nothing							*/
/*----------------------------------------------------------------------*/
static s32 contReadNW(NUSiCommonMesg* mesg)
{
    s32 rtn;
    /* Clear the waitmessage queue.		*/
    osRecvMesg(&nuContWaitMesgQ, NULL, OS_MESG_NOBLOCK);
    
    /* Controller read	*/
    rtn = contReadData(nuContData, 0);
    if(rtn) return rtn;
    
    /* Call the callback function after read ends. */
    /* Because the priority of this thread is second to that of the event handler,	*/
    /* processing by the callback function should be held to the minimum required.*/
    if(nuContReadFunc != NULL){
	(*nuContReadFunc)(mesg->mesg);
    }
    
    return rtn;
}

