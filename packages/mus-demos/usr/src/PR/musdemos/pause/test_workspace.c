
/*************************************************************

  test_workspace.c : Nintendo 64 Music Tools Library Sample
  (c) Copyright 1998, Software Creations (Holdings) Ltd.

  Version 3.14

**************************************************************/

/* include current header file */
#include "test_rom.h"

/* screen buffers */
unsigned short cfb_A[SCREEN_XSIZE*SCREEN_YSIZE];
unsigned short cfb_B[SCREEN_XSIZE*SCREEN_YSIZE];

/* graphics task yield buffer */
u64   yield_buffer[OS_YIELD_DATA_SIZE/sizeof(u64)];

/* graphics task fifo buffer */
u64   fifo_buffer[FIFO_DATA_SIZE/sizeof(u64)];

/* end of file */

