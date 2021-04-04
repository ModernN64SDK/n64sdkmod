/*======================================================================*/
/*		NuSYS										*/
/*		nugfxswapcfb.c								*/
/*												*/
/*		Copyright (C) 1997, NINTENDO Co,Ltd.				*/
/*												*/
/*----------------------------------------------------------------------*/    
/* Ver 1.0	97/10/9		Created by Kensaku Ohki(SLANP)		*/
/*======================================================================*/  
/* $Id: nugfxswapcfb.c,v 1.2 1997/11/28 01:21:10 ohki Exp $			*/
/*======================================================================*/
#include <nusys.h>


/*----------------------------------------------------------------------*/
/*	nuGfxSwapCfb - Switch frame buffers 					*/
/*	IN:	None 										*/
/*	RET:	None 										*/
/*----------------------------------------------------------------------*/
void nuGfxSwapCfb(void* gfxTask)
{
    NUScTask*	gfxTaskPtr;
    gfxTaskPtr = (NUScTask*)gfxTask;
    
    osViSwapBuffer(gfxTaskPtr->framebuffer);
}
