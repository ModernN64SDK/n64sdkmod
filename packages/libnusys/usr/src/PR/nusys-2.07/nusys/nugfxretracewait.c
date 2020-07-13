/*======================================================================*/
/*		NuSYS										*/
/*		nugfxretracewait.c							*/
/*												*/
/*		Copyright (C) 1997, NINTENDO Co,Ltd.				*/
/*												*/
/*----------------------------------------------------------------------*/    
/* Ver 1.0	97/10/9		Created by Kensaku Ohki(SLANP)		*/
/*======================================================================*/  
#include <nusys.h>


/*----------------------------------------------------------------------*/
/*	nuGfxRetraceWait - Wait for the retrace 					*/
/*	IN:	retrace_num	The retrace number 					*/
/*	RET:	None 										*/
/*----------------------------------------------------------------------*/
void nuGfxRetraceWait(u32 retrace_num)
{
    NUScClient	client;
    OSMesg	mesgBuf;
    OSMesgQueue mesgQ;

    osCreateMesgQueue(&mesgQ, &mesgBuf, 1);

    /* Register the retrace client to the scheduler  */
    nuScAddClient(&client, &mesgQ , NU_SC_RETRACE_MSG);  
    while(retrace_num){
	osRecvMesg( &mesgQ, NULL, OS_MESG_BLOCK );
	retrace_num--;
    }
    nuScRemoveClient(&client );
}
