/*
  dma.c
    Copyright (C) 1999, NINTENDO Co,Ltd., MONEGI CORPORATION.
    Jan. 28, 1999.
*/

#include <ultra64.h>
#include "Dma.h"

static OSMesgQueue  dmaMessageQ;
static OSMesg       dmaMessageBuf;
static OSIoMesg     dmaIOMessageBuf;

void init_dma( void )
{
    osCreateMesgQueue( &dmaMessageQ, &dmaMessageBuf, 1 );
}

void read_rom( u32 *seg_start, u32 *rom_start, u32 *rom_end  )
{
    osPiStartDma( &dmaIOMessageBuf, OS_MESG_PRI_NORMAL, OS_READ,
		  (u32)rom_start, seg_start,
		  (u32)rom_end - (u32)rom_start,
		  &dmaMessageQ);

    (void) osRecvMesg(&dmaMessageQ, NULL, OS_MESG_BLOCK);
}


