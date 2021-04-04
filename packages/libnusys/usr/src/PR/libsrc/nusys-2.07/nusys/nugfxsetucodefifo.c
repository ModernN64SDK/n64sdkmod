/*======================================================================*/
/*		NuSYS							*/
/*		nugfxsetucodefifoset.c					*/
/*									*/
/*		Copyright (C) 1999, NINTENDO Co,Ltd.			*/
/*									*/
/*======================================================================*/
/* $Id: nugfxsetucodefifo.c,v 1.1 1999/07/16 13:25:46 ohki Exp $		*/
/*======================================================================*/
#include <nusys.h>

/*----------------------------------------------------------------------*/
/*	nuGfxSetUcodeFifo - Settings for FIFO buffer			*/
/*	IN:	fifoBufPtr	Pointer to fifo buffer(16byte boundaries)*/
/*		size		fifo buffer size(multiple of 8)		*/
/*	RET:	none							*/
/*----------------------------------------------------------------------*/
void nuGfxSetUcodeFifo(void* fifoBufPtr, s32 size)
{
#ifdef NU_DEBUG
    if((u32)fifoBufPtr & 0x0000000f){
	osSyncPrintf("nuGfxUcodeFifoSet: fifo buffer is NOT 16byte boundaries\n");
    }
    if(size % 0x10){
	osSyncPrintf("nuGfxUcodeFifoSet: fifo size is NOT multiple 8 \n");
    }
#endif
    
    nuGfxUcodeFifoPtr = (u64*)fifoBufPtr;
    nuGfxUcodeFifoSize = size;


}    
