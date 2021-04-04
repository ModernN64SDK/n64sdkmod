/*======================================================================*/
/*		NuSYS							*/
/*		nusimgr.c						*/
/*									*/
/*		Copyright (C) 1997, NINTENDO Co,Ltd.			*/
/*									*/
/*----------------------------------------------------------------------*/
/* Ver 1.1	97/12/1		Created by Kensaku Ohki(SLANP)		*/
/*======================================================================*/
/* $Id: nusimgr.c,v 1.10 1999/01/25 05:47:17 ohki Exp $		*/
/*======================================================================*/
#include <nusys.h>

OSMesgQueue	nuSiMesgQ;		/* SI event  message queue */
static OSMesg	nuSiMesgBuf[8];		/* SI event  message buffer */
static OSThread siMgrThread;
static u64	siMgrStack[NU_SI_STACK_SIZE/sizeof(u64)];
OSMesgQueue	nuSiMgrMesgQ;		/* SI Manager queue */
NUCallBackList*	nuSiCallBackList = NULL;/* Callback function list */

static void nuSiMgrThread(void* arg);
    
/*----------------------------------------------------------------------*/
/*	nuSiMgrInit - Initializes the SI Manager				*/
/*	IN:	nothing							*/
/*	RET:	Controller bit pattern			*/
/*----------------------------------------------------------------------*/
u8 nuSiMgrInit(void)
{
    u8 			pattern;
    OSContStatus	status[NU_CONT_MAXCONTROLLERS];
    
    /* Create an SI message queue and register events. */
    osCreateMesgQueue(&nuSiMesgQ, nuSiMesgBuf, 8);
    osSetEventMesg(OS_EVENT_SI, &nuSiMesgQ, (OSMesg)0);

    /* Controller initialization 				*/
    /* This actually ought to be done by the Controller Manager,		*/
    /* but this function is superior so it is more straightforward to call it.	*/
    /* This poses problems for the OS, though.				*/
    osContInit(&nuSiMesgQ, &pattern, status);
    
    /* Start the Controller thread. */
    osCreateThread(&siMgrThread, NU_SI_THREAD_ID, nuSiMgrThread,
		   (void*)NULL,
		   (siMgrStack + NU_SI_STACK_SIZE/sizeof(u64)), NU_SI_THREAD_PRI);
    osStartThread(&siMgrThread);
    
    return pattern;
}
/*----------------------------------------------------------------------*/
/*	nuSiSendMesg - Sends a message to the SI Manager (asynchronous) */
/*									*/
/*	Sends the message specified by mesg to the SI Manager and waits for a */
/*	message to be returned in rtnMesg.					*/
/*									*/
/*									*/
/*	IN:	mesg		Message to send to the SI Manager		*/
/*		dataPtr		Pointer to the data sent to			*/
/*				the callback function or returned by it. 										*/
/*									*/
/*									*/
/*	RTN:	error							*/
/*----------------------------------------------------------------------*/
s32 nuSiSendMesg(NUScMsg mesg, void* dataPtr)
{
    OSMesg		rtnMesgBuf;
    OSMesgQueue		rtnMesgQ;
    NUSiCommonMesg	siCommonMesg;

    siCommonMesg.mesg	  = mesg;
    siCommonMesg.dataPtr  = dataPtr;
    siCommonMesg.rtnMesgQ = &rtnMesgQ;
    
    osCreateMesgQueue(&rtnMesgQ, &rtnMesgBuf, 1);
    
    osSendMesg(&nuSiMgrMesgQ, (OSMesg*)&siCommonMesg, OS_MESG_BLOCK);
    osRecvMesg(&rtnMesgQ, NULL, OS_MESG_BLOCK);
    return siCommonMesg.error;
}

/*----------------------------------------------------------------------*/
/*	nuSiMgrStop - Stops the SI Manager				*/
/*									*/
/*	Stops the SI manager to allow the application to access an		*/ 
/*	SI device directly. 						*/
/*									*/
/*	After this function is called, nuSiMesgQ can safely be used from the
	application thread.						*/
/*									*/
/*									*/
/*	IN:	nothing							*/
/*	RTN:	nothing							*/
/*----------------------------------------------------------------------*/
void nuSiMgrStop(void)
{
    nuSiSendMesg(NU_SI_STOP_MGR_MSG, (void*)NULL);
}
/*----------------------------------------------------------------------*/
/*	nuSiMgrRestart - Sets a stopped SI Manager to executable status. 	*/
/*									*/
/*	Sets a stopped SI Manager to executable status.			*/
/*									*/
/*	IN:	nothing							*/
/*	RTN:	nothing							*/
/*----------------------------------------------------------------------*/
void nuSiMgrRestart(void)
{
    osStartThread(&siMgrThread);
}

/*----------------------------------------------------------------------*/
/*	nuSiMgrThread - SI Manager thread				*/
/* 	SI Manager.  If a message is received, */
/*	the callback functions are executed in order				*/
/*	IN:	*arg	 Argument used in generating thread. None.	*/
/*	RET:	nothing							*/
/*----------------------------------------------------------------------*/
static void nuSiMgrThread(void* arg)
{
    NUScClient		siClient;
    OSMesg		siMgrMesgBuf[NU_SI_MESG_MAX];
    NUSiCommonMesg*	siMesg;    
    NUCallBackList**	siCallBackListPtr;
    s32			ret;
    u16			majorNo;
    u16			minorNo;
    
    /* Create an SI message queue. */
    osCreateMesgQueue(&nuSiMgrMesgQ,siMgrMesgBuf, NU_SI_MESG_MAX);

    /* Register a RETRACE client with the Scheduler. */
    nuScAddClient(&siClient, &nuSiMgrMesgQ, NU_SC_RETRACE_MSG);

    /* SI Manager loop */
    while(1){
	(void)osRecvMesg(&nuSiMgrMesgQ,(OSMesg*)&siMesg, OS_MESG_BLOCK);
	
	siCallBackListPtr = &nuSiCallBackList;

	switch(siMesg->mesg){
	case NU_SC_RETRACE_MSG:
	    /* Processing with a retrace message */
	    
	    /* Call functions registered with the callback function linked list. */
	    while(*siCallBackListPtr){
		
		if((*siCallBackListPtr)->func[0]){
		    ret = (*((*siCallBackListPtr)->func[0]))(siMesg);
		}
		
		/* Exit loop when return value is 1. */
		if(ret) break;
		siCallBackListPtr = &(*siCallBackListPtr)->next;
	    }
	    break;
	    
	case NU_SI_STOP_MGR_MSG:	/* Stop the SI Manager. */
	    osSendMesg(siMesg->rtnMesgQ, NULL, OS_MESG_BLOCK);
	    nuScResetClientMesgType(&siClient, 0);
	    osStopThread(NULL);
	    nuScResetClientMesgType(&siClient, NU_SC_RETRACE_MSG);
	    break;
	    
	default:
	    /* Processing with a non-retrace message.*/
	    
	    majorNo = siMesg->mesg & NU_SI_MSG_MAJOR_NO_MASK;
	    minorNo = siMesg->mesg & NU_SI_MSG_MINOR_NO_MASK;
	    
	    /* Call functions registered with the callback function linked list. */
	    while(*siCallBackListPtr){
		if((*siCallBackListPtr)->majorNo == majorNo){
		    if((*siCallBackListPtr)->funcNum > minorNo){
			siMesg->error = (*((*siCallBackListPtr)->func[minorNo]))(siMesg);			
		    } else {
#ifdef	NU_DEBUG
			osSyncPrintf("nuSiMgr: majorNo %#x minorNo %#x is over callback func num(%d).\n",
				     majorNo, minorNo,
				     (*siCallBackListPtr)->funcNum);
			siMesg->error = -1;
#endif	/* NU_DEBUG */
			
		    }
		    /* Return an 'finished' message if rtnMesgQ is not NULL. */
		    /* Set,rtnMesgQ to NULL for asynchronous functions.	*/
		    if(siMesg->rtnMesgQ != NULL){
			osSendMesg(siMesg->rtnMesgQ, NULL, OS_MESG_BLOCK);
		    }
		    break;
		}
		siCallBackListPtr = &(*siCallBackListPtr)->next;
	    }
#ifdef	NU_DEBUG
	    if(*siCallBackListPtr == NULL){
		osSyncPrintf("nuSiMgr: no si callback function mesg %#X\n", siMesg->mesg);
	    }
#endif	/* NU_DEBUG */
	    break;
	}
    }
}
