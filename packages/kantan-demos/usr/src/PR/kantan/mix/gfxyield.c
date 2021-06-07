/*============================================================================
  gfxyield.c
============================================================================*/
#define F3DEX_GBI
#include <ultra64.h>
#include <PR/ramrom.h>
#include <assert.h>

#include "def.h"

u64 gfxYieldBuf[OS_YIELD_DATA_SIZE/sizeof(u64)];
