/*======================================================================*/
/*		NuSYS							*/
/*		nuauprenmiproc.c					*/
/*									*/
/*		Copyright (C) 1997, NINTENDO Co,Ltd.			*/
/*									*/
/*======================================================================*/
/* $Id: nuauprenmiproc.c,v 1.2 1999/02/16 10:02:17 ohki Exp $		*/
/*======================================================================*/
#include <nusys.h>
#include <nualstl.h>

/*----------------------------------------------------------------------*/
/*	nuAuPreNMI - PRE NMI message processing				*/
/* 	Processing function for when PRENMI message is received.	*/
/*	This function only activates a flag for fade out.		*/
/*	Customizing should be done individually				*/
/*	IN:	mesg_type		message type			*/
/*		frameCounter	frame counter				*/
/*	RET:	None							*/
/*----------------------------------------------------------------------*/
void nuAuPreNMIProc(NUScMsg mesg_type, u32 frameCounter)
{
    switch(mesg_type){
    case NU_SC_PRENMI_MSG:	/* PRENMI message */
	/* Sequence fade out */
	nuAuStlSeqPlayerStop(25);
	nuAuStlSndPlayerStop(25);
	break;
    case NU_SC_RETRACE_MSG:	/* retrace message */
	/* Stop the task at 28 frames (approx. 0.47 sec.) from PRENNI */
	if(frameCounter == 28){
	    nuAuTaskStop = NU_AU_TASK_STOP;
	    nuScRemoveClient(&nuAuClient);
	}
	break;
    }
}
