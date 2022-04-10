/*======================================================================*/
/*		NuSYS							*/
/*		nupireadrom.c						*/
/*									*/
/*		Copyright (C) 1997, NINTENDO Co,Ltd.			*/
/*									*/
/*----------------------------------------------------------------------*/
/* Ver 1.0	97/10/9		Created by Kensaku Ohki(SLANP)		*/
/*----------------------------------------------------------------------*/
/* $Id: nupireadrom.c,v 1.10 1999/06/10 04:43:25 ohki Exp $		*/
/*======================================================================*/
#include <nusys.h>


/*----------------------------------------------------------------------*/
/*	nuPiReadRom  -  Transfers data from ROM by DMA			*/
/*	Transfers data from ROM by DMA.					*/
/*	The message queue is a local variable, allowing it to be shared between threads.   */
/*	DMA transfers are performed in 16 Kbyte units in consideration of PI transfers.	*/
/*										*/
/*									*/
/*	Changed for EPI						*/
/*									*/
/*	IN:	Pointer to the start of the heap buffer			*/
/*	RET:	nothing							*/
/*----------------------------------------------------------------------*/
void nuPiReadRom(u32 rom_addr, void* buf_ptr, u32 size)
{
    OSIoMesg	dmaIoMesgBuf;
    OSMesgQueue dmaMesgQ;
    OSMesg	dmaMesgBuf;
    u32		readSize;

    
#ifdef	USE_EPI

    /* Create message queue */
    osCreateMesgQueue(&dmaMesgQ, &dmaMesgBuf, 1);
    dmaIoMesgBuf.hdr.pri      = OS_MESG_PRI_NORMAL;
    dmaIoMesgBuf.hdr.retQueue = &dmaMesgQ;
    
    /* Disable the CPU cache. */
    osInvalDCache((void*)buf_ptr, (s32)size);

    while(size){
	if(size > NU_PI_CART_BLOCK_READ_SIZE){
	    readSize = NU_PI_CART_BLOCK_READ_SIZE;
	} else {
	    readSize = size;
	}
	dmaIoMesgBuf.dramAddr     = buf_ptr;
	dmaIoMesgBuf.devAddr      = (u32)rom_addr;
	dmaIoMesgBuf.size         = readSize;

	osEPiStartDma(nuPiCartHandle, &dmaIoMesgBuf, OS_READ);

	/* Wait for end. */
	(void)osRecvMesg(&dmaMesgQ, &dmaMesgBuf, OS_MESG_BLOCK);
	
	rom_addr += readSize;
	buf_ptr = (void*)((u8*)buf_ptr + readSize);
	size -= readSize;
    }
#else

     /* Disable the CPU cache. */
    osInvalDCache((void*)buf_ptr, (s32)size);

    /* Create message queue. */
    osCreateMesgQueue(&dmaMesgQ, &dmaMesgBuf, 1);
    
    while(size){
	if(size > NU_PI_CART_BLOCK_READ_SIZE){
	    readSize = NU_PI_CART_BLOCK_READ_SIZE;
	} else {
	    readSize = size;
	}
	    
	/* DMA read */
	osPiStartDma(&dmaIoMesgBuf, OS_MESG_PRI_NORMAL, OS_READ, rom_addr,
		     buf_ptr, readSize, &dmaMesgQ);

	/* Wait for end. */
	(void)osRecvMesg(&dmaMesgQ, NULL, OS_MESG_BLOCK);
	
	rom_addr += readSize;
	buf_ptr = (void*)((u8*)buf_ptr + readSize);
	size -= readSize;
    }

#endif /* USE_EPI */
}
