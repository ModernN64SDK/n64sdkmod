/*============================================================================

		NINTENDO64 TECHNICAL SUPPORT CENTER 
		
		    NINTENDO64 SAMPLE PROGRAM 1

		Copyright (C) 1997, NINTENDO Co,Ltd.

============================================================================*/
#include <ultra64.h>
#include <PR/ramrom.h>
#include <assert.h>

#include "message.h"
#include "def.h"

/*  for CodeWarrior compiler */
#ifdef __MWERKS__
#pragma align(16)
#endif

#if __GNUC__ /* { */

/* for GNU compiler */
u64 dram_stack[SP_DRAM_STACK_SIZE64] __attribute__((aligned (16))); /* used for matrix stack */

#else /* }{ */

/* for SGI compiler */
u64 dram_stack[SP_DRAM_STACK_SIZE64];

#endif /* } */
