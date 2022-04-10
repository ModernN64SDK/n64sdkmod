/*======================================================================*/
/*		NuSYS							*/
/*		nugfxthread.c						*/
/*									*/
/*		Copyright (C) 1997, NINTENDO Co,Ltd.			*/
/*									*/
/*----------------------------------------------------------------------*/    
/* $Id: nugfxthread.c,v 1.3 1999/06/10 04:44:47 ohki Exp $		*/
/*======================================================================*/
#include <nusys.h>

NUGfxFunc	nuGfxFunc = NULL;
NUGfxPreNMIFunc	nuGfxPreNMIFunc = NULL;
OSMesgQueue	nuGfxMesgQ;
OSThread	nuGfxThread;			/* graphic thread */
static OSMesg	nuGfxMesgBuf[NU_GFX_MESGS];

static u64	GfxStack[NU_GFX_STACK_SIZE/8];	/* gfx thread stack  */


/*---------------------------------------------------------------------------*/
/*	gfxThread - The graphic thread for the applicaton		     */
/*	IN:	*arg	The argument of osCreateThread. Nothing in particular*/
/*	RET:	None							     */
/*---------------------------------------------------------------------------*/
static void gfxThread(void *arg)
{
    NUScClient	gfx_client;
    NUScMsg*	mesg_type;

    osCreateMesgQueue(&nuGfxMesgQ, nuGfxMesgBuf, NU_GFX_MESGS);

    /* Register as the RETRACE client to the scheduler */
    nuScAddClient(&gfx_client, &nuGfxMesgQ, NU_SC_RETRACE_MSG | NU_SC_PRENMI_MSG);

    while(1){
	(void)osRecvMesg(&nuGfxMesgQ, (OSMesg*)&mesg_type, OS_MESG_BLOCK);
	switch(*mesg_type){
	case NU_SC_RETRACE_MSG:	/* The retrace message */

	    /* Call the graphic function of the application side by the retrace message */
	    /* nuGfxpending is the interruption over-flag. */
	    if(nuGfxFunc != NULL){
		(*nuGfxFunc)(nuGfxTaskSpool);
	    }
	    break;
	    
	case NU_SC_PRENMI_MSG: /* The PRENMI message */

	    /* Call the call-back function in PRE NMI functions */
	    if(nuGfxPreNMIFunc != NULL){
		(*nuGfxPreNMIFunc)();
	    }
	    break;
	default:
	    break;
	}
    }
}

/*----------------------------------------------------------------------*/
/*	nuGfxThreadStart - Activate the graphic thread			*/
/*	IN:	None							*/
/*	RET:	None							*/
/*----------------------------------------------------------------------*/
void nuGfxThreadStart(void)
{
    /* Activate the graphic thread */
    osCreateThread(&nuGfxThread, NU_GFX_THREAD_ID, gfxThread, (void*)NULL,
		   (GfxStack + NU_GFX_STACK_SIZE/8), NU_GFX_THREAD_PRI);
    osStartThread(&nuGfxThread);
}
