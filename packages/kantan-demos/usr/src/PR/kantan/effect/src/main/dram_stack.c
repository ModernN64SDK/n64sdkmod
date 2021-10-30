/*============================================================================
  dram_stack.c
  ============================================================================*/

#define F3DEX_GBI
#include <ultra64.h>
#include <PR/ramrom.h>
#include <assert.h>

#include "message.h"
#include "def.h"

/* dram stack buffer */
u64 dram_stack[SP_DRAM_STACK_SIZE64];
