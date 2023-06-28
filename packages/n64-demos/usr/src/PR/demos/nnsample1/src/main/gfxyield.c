/*============================================================================

		NINTENDO64 TECHNICAL SUPPORT CENTER 
		
		    NINTENDO64 SAMPLE PROGRAM 1

		Copyright (C) 1997, NINTENDO Co,Ltd.

============================================================================*/
#include <ultra64.h>
#include <PR/ramrom.h>
#include <assert.h>

#include "def.h"

u64 gfxYieldBuf[OS_YIELD_DATA_SIZE/sizeof(u64)];
