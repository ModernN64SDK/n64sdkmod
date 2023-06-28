
#include <ultra64.h>

#include "src/main/textlib.h"

#ifdef __MWERKS__
#pragma align(8)
#endif

u16 bitmap_buf1[SCREEN_SIZE_VY*SCREEN_SIZE_X] __attribute__((aligned (64)));
u16 bitmap_buf2[SCREEN_SIZE_VY*SCREEN_SIZE_X] __attribute__((aligned (64)));
u16 *bitmap_buf;
u8 backup_buffer[256*0x100];





