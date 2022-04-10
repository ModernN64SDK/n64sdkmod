/*======================================================================*/
/*		NuSystem						*/
/*		nuaudma.c						*/
/*									*/
/*		Copyright (C) 1998, NINTENDO Co,Ltd.			*/
/*									*/
/*	98/12/09	Created	 by K.Ohki(SLANP)			*/
/*======================================================================*/
/* $Id: nuaudma.c,v 1.7 1999/06/10 05:05:59 ohki Exp $		*/
/*======================================================================*/
#include <nusys.h>
#include <nualsgi.h>

OSMesgQueue	nuAuDmaMesgQ;
OSMesg*		nuAuDmaMesgBuf;
OSIoMesg*	nuAuDmaIOMesgBuf;

NUDMAState	nuAuDmaState;
NUDMABuffer*	nuAuDmaBuf;
s16		nuAuDmaBufNum = NU_AU_DMA_BUFFER_NUM;
s16		nuAuDmaBufSize = NU_AU_DMA_BUFFER_SIZE;


static s32	nuAuDmaNext;


/*----------------------------------------------------------------------*/
/*	DMA CallBack 							*/
/*									*/
/*   DMA callback routine for the synthesizer driver			*/
/*   If the DMA buffers contains a required value, this function returns a pointer to the buffer.	*/
/*   If not, it reads the data into a buffer by DMA transfer. */
/*   This mechanism reduces the number of DMA calls. */
/*   Unused portions are cleared with 2 frames.				*/
/* IN	addr	ROM address with data				*/
/* 	len	Data size						*/
/*----------------------------------------------------------------------*/
s32 nuAuDmaCallBack(s32 addr, s32 len, void *state)
{
    
    void*	freeBuffer;
    int		delta;
    NUDMABuffer* dmaPtr;
    NUDMABuffer* lastDmaPtr;
    s32		addrEnd,buffEnd;
    OSIoMesg*	dmaIoMesgBufPtr;
    
#ifdef	NU_DEBUG
    nuAuDebDmaCallbackFrame++;
    if(len > nuAuDmaBufSize){
	nuAuDebStatus |= NU_AU_DEBUG_DMABUFSIZE;
	if(nuAuDebFlag & NU_AU_DEBUG_NORMAL){
	    osSyncPrintf("nuAuDmaCallBack: DMA Buffer size is too small.\n");
	}
    }
#endif	/* NU_DEBUG */
    
    lastDmaPtr = NULL;
    dmaPtr = nuAuDmaState.firstUsed;
    addrEnd = addr+len;

    /* Check whether desired data are in the buffer. */    
    while(dmaPtr) {

	buffEnd = dmaPtr->startAddr + nuAuDmaBufSize;

	/*--------------------------------------*/
	/* DMA buffers are arranged in ascending order of startAddr, */
	/*   so a small value means the	*/
	/*   desired data are not present.	*/
	/*--------------------------------------*/
	if(dmaPtr->startAddr > addr) {	/* No data in buffer */
	    break;                   
	} else if(addrEnd <= buffEnd){  /* Data present in buffer */
	    
	    /* Reset the current counter. */	    
	    dmaPtr->frameCnt = NU_AU_BUFFER_USE_FRAME;
	    
	    /* Calculate addresses where required data are actually located.*/
	    freeBuffer = dmaPtr->ptr + addr - dmaPtr->startAddr;
	    
	    /* Return the physical address of the buffer. */
	    return (s32) osVirtualToPhysical(freeBuffer);
	}

	lastDmaPtr = dmaPtr;
	dmaPtr = (NUDMABuffer*)dmaPtr->node.next;
    }
    
    /*------------------------------------------------------------------*/
    /* No desired data were contained in the DMA buffers, so transfer data	*/
    /* by DMA.						*/
    /*------------------------------------------------------------------*/
    
    /* Select 1 free buffer from the DMA buffers. 	*/
    dmaPtr = nuAuDmaState.firstFree;

    
    /* If there are no free buffers, NULL is selected.	*/
    /* For the time being, return a pointer to the buffer being used,	*/
    /* since this is better than stopping.		*/
    if(dmaPtr == NULL){
#ifdef NU_DEBUG
	if(nuAuDebFlag & NU_AU_DEBUG_NORMAL){
	    osSyncPrintf("auDmaCallBack: no dma buffer!!\n");
	}
	nuAuDebStatus |= NU_AU_DEBUG_NODMABUF;
#endif /* NU_DEBUG */
	return(int)OS_K0_TO_PHYSICAL(nuAuDmaState.firstUsed->ptr);
    }
    
    nuAuDmaState.firstFree = (NUDMABuffer*)dmaPtr->node.next;
    alUnlink((ALLink*)dmaPtr);
    
    /*--------------------------------------------------*/
    /* Insert the obtained DMA buffers in the linked list being used so that*/
    /*   the starting addresses are arranged in ascending order.		*/
    /*--------------------------------------------------*/	   
    if(lastDmaPtr) {
	
	/* Usually insert in linked list. */
	alLink((ALLink*)dmaPtr,(ALLink*)lastDmaPtr);
	
    } else if(nuAuDmaState.firstUsed) {
	
	/* Insert at start of linked list. */
	lastDmaPtr = nuAuDmaState.firstUsed;
	nuAuDmaState.firstUsed = dmaPtr;
	dmaPtr->node.next = (ALLink*)lastDmaPtr;
	dmaPtr->node.prev = 0;
	lastDmaPtr->node.prev = (ALLink*)dmaPtr;
	
    } else {
	
	/* If there is no DMA buffer being used */
	nuAuDmaState.firstUsed = dmaPtr;
	dmaPtr->node.next = 0;
	dmaPtr->node.prev = 0;
	
    }

#ifdef USE_EPI
    dmaIoMesgBufPtr = &nuAuDmaIOMesgBuf[nuAuDmaNext++];
    freeBuffer = dmaPtr->ptr;
    delta = addr & 0x1;
    addr -= delta;            
    dmaPtr->startAddr = addr;  /* Set address locations for DMA to 2-byte intervals.*/
    dmaPtr->frameCnt = NU_AU_BUFFER_USE_FRAME;

#ifdef	NU_DEBUG
    nuAuDebDmaFrame++;
    if(nuAuDebFlag & NU_AU_DEBUG_DISABLEDMA){
	return (s32)osVirtualToPhysical(dmaPtr->ptr);		/* dummy */
    }
#endif	/* NU_DEBUG	*/
    
    dmaIoMesgBufPtr->hdr.pri      = OS_MESG_PRI_NORMAL;
    dmaIoMesgBufPtr->hdr.retQueue = &nuAuDmaMesgQ;
    dmaIoMesgBufPtr->dramAddr     = freeBuffer;
    dmaIoMesgBufPtr->devAddr      = (u32)addr;
    dmaIoMesgBufPtr->size         = nuAuDmaBufSize;
    osEPiStartDma(nuPiCartHandle, dmaIoMesgBufPtr, OS_READ);
    
#else
    
    freeBuffer = dmaPtr->ptr;
    delta = addr & 0x1;
    addr -= delta;            /* Set address locations for DMA to 2-byte intervals. */
    dmaPtr->startAddr = addr;
    dmaPtr->frameCnt = NU_AU_BUFFER_USE_FRAME;
    
#ifdef	NU_DEBUG
    nuAuDebDmaFrame++;
    if(nuAuDebFlag & NU_AU_DEBUG_DISABLEDMA){
	return (s32)osVirtualToPhysical(dmaPtr->ptr);		/* dummy */
    }
#endif	/* NU_DEBUG	*/

    /* DMA transfer */
    osPiStartDma(&nuAuDmaIOMesgBuf[nuAuDmaNext++], OS_MESG_PRI_NORMAL, OS_READ,
		 (u32)addr, freeBuffer, nuAuDmaBufSize, &nuAuDmaMesgQ);
#endif /* USE_EPI */
    return (s32) osVirtualToPhysical(freeBuffer) + delta;
}


/*----------------------------------------------------------------------*/ 
/*  auDmaNew -  Initializes the Sequence Player DMA callback			*/
/*									*/
/*  Callback routine called once for initialization when registered with the Sequence Player. */
/*  The callback routine returned by 'return' is subsequently called. */
/*							*/
/* IN **state	Status pointer					*/
/*	        Setting the pointer here causes the pointer to be passed to state	*/
/*		when DMAcallback is called.					*/
/*----------------------------------------------------------------------*/ 
ALDMAproc nuAuDmaNew(NUDMAState **state)
{
    s32 i;
    
    /* Check whether initialized. If not, initialize. */
    if(!nuAuDmaState.initialized) {

	nuAuDmaMesgBuf = nuAuHeapAlloc(nuAuDmaBufNum * sizeof(OSMesg));
	nuAuDmaIOMesgBuf = nuAuHeapAlloc(nuAuDmaBufNum * sizeof(OSIoMesg));
	
	/* Create a message queue for DMA callbacks. */
	osCreateMesgQueue(&nuAuDmaMesgQ, nuAuDmaMesgBuf, nuAuDmaBufNum);

	/* Allocated a buffer for the DMA buffer pointer array. */
	nuAuDmaBuf = nuAuHeapAlloc(nuAuDmaBufNum * sizeof(NUDMABuffer));
	
	nuAuDmaState.firstFree = nuAuDmaBuf;	/* Register the start of the free DMABuf list. */
	nuAuDmaState.firstUsed = NULL;            /* No DMA buffers being used. */
	nuAuDmaBuf[0].node.prev = NULL;
	nuAuDmaBuf[0].node.next = NULL;
	
	/* Create links to pointers. */
	for (i = 0; i < nuAuDmaBufNum - 1; i++){
	    alLink((ALLink*)&nuAuDmaBuf[i+1], (ALLink*)&nuAuDmaBuf[i]);
	    nuAuDmaBuf[i].ptr = nuAuHeapAlloc(nuAuDmaBufSize);
	}
	nuAuDmaBuf[i].ptr = nuAuHeapAlloc(nuAuDmaBufSize);
	/* The initialized flag settings */
	nuAuDmaState.initialized++;
    }
    nuAuDmaNext = 0;
    *state = &nuAuDmaState;  /* Assign the nuDmaState structure pointer to state. */
    
    return nuAuDmaCallBack;
}


/*----------------------------------------------------------------------*/ 
/*  auCleanDMABuffers - DMA buffer list area			*/
/*									*/
/*  Search the DMA buffers and add buffers with 2 or more unused frames		*/
/*  to the linked list of free buffers.			*/
/*									*/
/*	IN 	nothing							*/
/* 	RTN	nothing							*/
/*----------------------------------------------------------------------*/ 
void nuAuCleanDMABuffers(void)
{
    
    NUDMABuffer *dmaPtr,*nextPtr;

#ifdef	NU_DEBUG
    u32	DmaUseCnt = 0;
#endif	/* NU_DEBUG	*/
    
#ifdef NU_DEBUG
    while(nuAuDmaNext){
	if(osRecvMesg(&nuAuDmaMesgQ, NULL, OS_MESG_NOBLOCK)){
	    if(nuAuDebFlag & NU_AU_DEBUG_NORMAL){
		osSyncPrintf("nuAuCleanDMABuffers: Audio DMA not completed!!\n");
	    }
	    nuAuDebStatus |= NU_AU_DEBUG_DMANOTCOMPLETE;
	}
	nuAuDmaNext--;    
    }
#else
    nuAuDmaNext = 0;    
#endif	/* NU_DEBUG */
    
    dmaPtr =nuAuDmaState.firstUsed;
    
    while(dmaPtr) {
	
	nextPtr = (NUDMABuffer*)dmaPtr->node.next;
	
	/*------------------------------------------------------*/
	/*  Adds buffers that have handled at least NU_AU_BUFFER_USE_FRAME number of */
	/*  frames to the linked list of free buffers.		*/
	/*  To increase NU_AU_BUFFER_USE_FRAME , the number of buffers must be increased.*/
	/*  Decreasing them increases the frequency of DMA use.  Ouch! */
	/*------------------------------------------------------*/	      
  	if(!dmaPtr->frameCnt--){
	    if(nuAuDmaState.firstUsed == dmaPtr){
		nuAuDmaState.firstUsed = (NUDMABuffer*)dmaPtr->node.next;
	    }
	    alUnlink((ALLink*)dmaPtr);
	    if(nuAuDmaState.firstFree){
		alLink((ALLink*)dmaPtr,(ALLink*)nuAuDmaState.firstFree);
	    }else{
		nuAuDmaState.firstFree = dmaPtr;
		dmaPtr->node.next = 0;
		dmaPtr->node.prev = 0;
	    }
	}
#ifdef	NU_DEBUG
	else {
	    DmaUseCnt++;
	}
#endif	/* NU_DEBUG	*/
	
	dmaPtr = nextPtr;
    }

    
#ifdef	NU_DEBUG

    /* Update the value for maximum number of DMA buffers used. */
    if(nuAuDebDmaBufMaxUse < DmaUseCnt){
	nuAuDebDmaBufMaxUse = DmaUseCnt;
    }
    if(nuAuDebDmaCallbackMax < nuAuDebDmaCallbackFrame){
	nuAuDebDmaCallbackMax = nuAuDebDmaCallbackFrame;
    }
    if(nuAuDebDmaMax < nuAuDebDmaFrame){
	nuAuDebDmaMax = nuAuDebDmaFrame;
    }
    nuAuDebDmaCount += nuAuDebDmaFrame;
    nuAuDebDmaCallbackCount += nuAuDebDmaCallbackFrame;
    nuAuDebDmaFrame  = 0;
    nuAuDebDmaCallbackFrame  = 0;
    
#endif	/* NU_DEBUG	*/

    nuAuFrameCounter++;
}

