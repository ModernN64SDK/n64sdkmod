/*======================================================================*/
/*		NuSYS										*/
/*		nudebtaskperfbar.c							*/
/*												*/
/*		Copyright (C) 1997, NINTENDO Co,Ltd.				*/
/*												*/
/*----------------------------------------------------------------------*/    
/* Ver 1.0	97/10/9		Created by Kensaku Ohki(SLANP)		*/
/*======================================================================*/
#include <nusys.h>
#ifdef NU_DEBUG

#define LOAD_WIDTH	300
#define	LOAD_HEIGHT	100

static u32	auLoad[300];
static u32	gfxRspLoad[300];
static u32	gfxRdpLoad[300];
static u32	idx = 0;

/*----------------------------------------------------------------------*/
/*	Output the RSP and RDP loads 							*/
/*	IN:	None 										*/
/*	RET:	None 										*/
/*----------------------------------------------------------------------*/
void nuDebTaskPerfLoad(void)
{

    u32		cnt;

    auLoad[idx] = 0;
    gfxRspLoad[idx] = 0;
    gfxRdpLoad[idx] = 0;
    for(cnt = 0; cnt < nuDebTaskPerfPtr->auTaskCnt; cnt++){
	auLoad[idx] += nuDebTaskPerfPtr->auTaskTime[cnt].rspEnd
	    - nuDebTaskPerfPtr->auTaskTime[cnt].rspStart;
    }
    for(cnt = 0; cnt < nuDebTaskPerfPtr->gfxTaskCnt; cnt++){
	gfxRspLoad[idx] += nuDebTaskPerfPtr->gfxTaskTime[cnt].rspEnd
	    - nuDebTaskPerfPtr->gfxTaskTime[cnt].rspStart;
	gfxRdpLoad[idx] += nuDebTaskPerfPtr->gfxTaskTime[cnt].rdpEnd
	    - nuDebTaskPerfPtr->gfxTaskTime[cnt].rspStart;
    }
    osSyncPrintf("%d, %d, %d, %d\n", idx, auLoad[idx], gfxRspLoad[idx], gfxRdpLoad[idx]);
    idx++;
    if(idx >= 300) idx = 0;
    
}
#endif /* NU_DEBUG */
