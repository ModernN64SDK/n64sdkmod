/*======================================================================*/
/*		NuSYS							*/
/*		nupireadromoverlay.c					*/
/*									*/
/*		Copyright (C) 1999, NINTENDO Co,Ltd.			*/
/*									*/
/*----------------------------------------------------------------------*/
/* $Id: nupireadromoverlay.c,v 1.1 1999/06/10 04:43:46 ohki Exp $       */
/*======================================================================*/
#include <nusys.h>


/*----------------------------------------------------------------------*/
/*	nuPiReadRomOverlay  - DMA transfers overlay area from ROM       */
/*									*/
/*	DMA transfers the segment area, including the code area and	*/
/*	the data area, from ROM.  Used mainly for overlay.		*/
/*									*/
/*	IN:	segment		Segment structure       		*/
/*	RET:	None							*/
/*----------------------------------------------------------------------*/
void nuPiReadRomOverlay(NUPiOverlaySegment* segment)
{
    OSIoMesg	dmaIoMesgBuf;
    OSMesgQueue dmaMesgQ;
    OSMesg	dmaMesgBuf;
    u32		readSize;
    u32		rom_addr;
    u32		size;    
    s32		flag = 1;
    void*	buf_ptr;

    
#ifdef	USE_EPI

    /* Create message queue */
    osCreateMesgQueue(&dmaMesgQ, &dmaMesgBuf, 1);
    dmaIoMesgBuf.hdr.pri      = OS_MESG_PRI_NORMAL;
    dmaIoMesgBuf.hdr.retQueue = &dmaMesgQ;
    
    /* Invalidate CPU's instruction cache and data cache */
    osInvalDCache((void*)segment->dataStart, (u32)segment->dataEnd - (u32)segment->dataStart);
    osInvalICache((void*)segment->textStart, (u32)segment->textEnd - (u32)segment->textStart);
    
    rom_addr = (u32)segment->romStart;
    size = (u32)segment->romEnd - (u32)segment->romStart;
    buf_ptr = (void*)segment->ramStart;
    
    while(size){
	if(size > NU_PI_CART_BLOCK_READ_SIZE){
	    readSize = NU_PI_CART_BLOCK_READ_SIZE;
	} else {
	    readSize = size;
	}
	dmaIoMesgBuf.dramAddr     = buf_ptr;
	dmaIoMesgBuf.devAddr      = rom_addr;
	dmaIoMesgBuf.size         = readSize;

	osEPiStartDma(nuPiCartHandle, &dmaIoMesgBuf, OS_READ);

	/* Clear BSS section during first DMA transfer */
	if(flag){
	    bzero(segment->bssStart, (u32)segment->bssEnd - (u32)segment->bssStart);
	    flag = 0;
	}

	/* Wait for end */
	(void)osRecvMesg(&dmaMesgQ, &dmaMesgBuf, OS_MESG_BLOCK);
	
	rom_addr += readSize;
	buf_ptr = (void*)((u8*)buf_ptr + readSize);
	size -= readSize;
    }
#else
    /* Create message queue */
    osCreateMesgQueue(&dmaMesgQ, &dmaMesgBuf, 1);
    
    /* Invalidate CPU's instruction cache and data cache */
    osInvalDCache((void*)segment->dataStart, (u32)segment->dataEnd - (u32)segment->dataStart);
    osInvalICache((void*)segment->textStart, (u32)segment->textEnd - (u32)segment->textStart);
    
    rom_addr = (u32)segment->romStart;
    size = (u32)segment->romEnd - (u32)segment->romStart;
    buf_ptr = (void*)segment->ramStart;
    
    while(size){
	if(size > NU_PI_CART_BLOCK_READ_SIZE){
	    readSize = NU_PI_CART_BLOCK_READ_SIZE;
	} else {
	    readSize = size;
	}

	/* Read DMA */
	osPiStartDma(&dmaIoMesgBuf, OS_MESG_PRI_NORMAL, OS_READ, rom_addr,
		     buf_ptr, readSize, &dmaMesgQ);


	/* Clear BSS section during first DMA transfer */
	if(flag){
	    bzero(segment->bssStart, (u32)segment->bssEnd - (u32)segment->bssStart);
	    flag = 0;
	}

	/* Wait for end */
	(void)osRecvMesg(&dmaMesgQ, &dmaMesgBuf, OS_MESG_BLOCK);
	
	rom_addr += readSize;
	buf_ptr = (void*)((u8*)buf_ptr + readSize);
	size -= readSize;
    }

    
#endif /* USE_EPI */
}
