
/*---------------------------------------------------------------------*
        Copyright (C) 1998 Nintendo.
        
        $RCSfile: heap.c,v $
        $Revision: 1.1 $
        $Date: 1999/07/05 02:17:00 $
 *---------------------------------------------------------------------*/

#include "hglobals.h"
#include "hdd.h"
#include "hvideo.h"
#include "hvideo_str.h"

u8   heap_rwbuf[READWRITE_BYTESIZE];
Gfx  heap_sp_dlb[2][DL_BUFSIZE];
Gfx  heap_str_dlb[2][STR_DL_BUFSIZE];
u8 * heap_dma[JSTR_DMA_NUM + 1][JSTR_DMA_SIZE];
