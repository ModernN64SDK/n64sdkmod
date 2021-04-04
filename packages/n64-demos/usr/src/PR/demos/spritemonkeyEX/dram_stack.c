/*
  dram_stack.c
    Copyright (C) 1999, NINTENDO Co,Ltd., MONEGI CORPORATION.
    Jan. 28, 1999.
*/

#include <ultra64.h>

/*
 * The "dram_stack" field of the RCP task structure is set to this address.
 * It is placed in its own .c, and thus its own .o, since the linker aligns
 * individual relocatables to data cache line size (16 byte) boundaries.
 *
 * This avoids the problem where the dram_stack data is accidentally
 * scribbled over during a writeback for data sharing the same line.
 */
u64 dram_stack[SP_DRAM_STACK_SIZE64];	/* used for matrix stack */
