/*======================================================================*/
/*		NuSYS										*/
/*		nuauprenmi.c								*/
/*												*/
/*		Copyright (C) 1997, NINTENDO Co,Ltd.				*/
/*												*/
/*======================================================================*/
/* $Id: nuauprenmiproc.c,v 1.3 1999/03/05 00:03:47 ohki Exp $		*/
/*======================================================================*/
#include <nusys.h>
#include <nualsgi.h>

/*----------------------------------------------------------------------*/
/*	nuAuPreNMI - PRE NMI message processing					*/
/* 	Processing function for when PRE NMI message is received.		*/
/*	This function only activates a flag for fade out.			*/
/*	Customizing should be done individually.					*/
/*	IN:	mesg_type		message type					*/
/*		frameCounter	frame counter					*/
/*	RET:	None										*/
/*----------------------------------------------------------------------*/
void nuAuPreNMIProc(NUScMsg mesg_type, u32 frameCounter)
{
    switch(mesg_type){
    case NU_SC_PRENMI_MSG:	/* PRENMI message */
	/* sequence fade out */
	nuAuSeqPlayerFadeOut(0, 28);
	nuAuSeqPlayerFadeOut(1, 28);
	break;
    case NU_SC_RETRACE_MSG:	/* retrace message */
	if(frameCounter == 28){
	    nuAuTaskStop = NU_AU_TASK_STOP;
	}
	break;
    }
}
