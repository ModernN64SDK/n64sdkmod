/*
  Dma.h
    Copyright (C) 1999, NINTENDO Co,Ltd., MONEGI CORPORATION.
    Jan. 28, 1999.
*/

#ifndef _Dma_h_
#define _Dma_h_

#ifdef __cplusplus
extern "C" {
#endif

extern void init_dma( void );
extern void read_rom( u32 *seg_start, u32 *rom_start, u32 *rom_end  );

#ifdef __cplusplus
}
#endif

#endif

