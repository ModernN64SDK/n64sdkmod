/*======================================================================*/
/*	Mirror House							*/
/*	om_rom_data_read.c						*/
/*									*/
/*	Copyright (C) 1998 MONEGI CORPORATION				*/
/*									*/
/*======================================================================*/

#include "pack.h"
#include <PR/sp.h>

#include "nusys.h"
#include "om_segment.h"


/* ===========================================================================
	data section
 =========================================================================== */
static char *om_segment_addr[] = {_select_bgSegmentStart, _select_bgSegmentEnd,
                                  _openingSegmentStart,   _openingSegmentEnd,
                                  _logoimgsSegmentStart,  _logoimgsSegmentEnd,
                                  _opimgsSegmentStart,    _opimgsSegmentEnd
};

static char *om_rom_addr[] = {_select_bgSegmentRomStart, _select_bgSegmentRomEnd,
                              _openingSegmentRomStart,   _openingSegmentRomEnd,
                              _logoimgsSegmentRomStart,  _logoimgsSegmentRomEnd,
                              _opimgsSegmentRomStart,    _opimgsSegmentRomEnd
};

static char *om_data_addr[] = {_select_bgSegmentDataStart, _select_bgSegmentDataEnd,
                               _openingSegmentDataStart,   _openingSegmentDataEnd,
                               _logoimgsSegmentDataStart,  _logoimgsSegmentDataEnd,
                               _opimgsSegmentDataStart,    _opimgsSegmentDataEnd
};

static char *om_text_addr[] = {NULL, NULL,
                               NULL, NULL,
                               NULL, NULL,
                               NULL, NULL,
                               NULL, NULL
};
static char *om_bss_addr[] = {NULL, NULL,
                              NULL, NULL,
                              NULL, NULL,
                              NULL, NULL,
                              NULL, NULL
};

/* ===========================================================================
	Function Prototype Declarations 
 =========================================================================== */
void om_pi_dma_data_read(u32 seg_no);
void om_pi_dma_prog_read(u32 seg_no);

void om_pi_dma_data_read(u32 seg_no)
{
    u32 size;

#ifdef OM_DEBUG
    osSyncPrintf("Rom Read Start! %x %x\n", om_rom_addr[seg_no], om_segment_addr[seg_no]);
#endif
    size = om_rom_addr[seg_no+1] - om_rom_addr[seg_no];

#ifdef OM_DEBUG
    osSyncPrintf("Dma Status %d\n", osPiGetStatus());
#endif
    nuPiReadRom((u32)om_rom_addr[seg_no], (void*)om_segment_addr[seg_no], size);
}

void om_pi_dma_prog_read(u32 seg_no)
{
    OSIoMesg        dmaIOMessageBuf;
    OSMesgQueue     dmaMessageQ;
    OSMesg          dmaMessageBuf;
    u32             text_size, count;

    text_size = om_text_addr[seg_no+1] - om_text_addr[seg_no];
    count = om_rom_addr[seg_no+1] - om_rom_addr[seg_no];
    osInvalICache(om_text_addr[seg_no], text_size);
    osCreateMesgQueue(&dmaMessageQ, &dmaMessageBuf, 1);
    osPiStartDma(&dmaIOMessageBuf, OS_MESG_PRI_NORMAL, OS_READ,
                 (u32)om_rom_addr[seg_no], om_segment_addr[seg_no],
                 (u32)count, &dmaMessageQ);
    (void)osRecvMesg(&dmaMessageQ, NULL, OS_MESG_BLOCK);
}