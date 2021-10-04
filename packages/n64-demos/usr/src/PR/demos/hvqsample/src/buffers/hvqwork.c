/* 
 *  N64-HVQ2 library  Sample program
 * 
 *  FILE : hvqwork.c
 * 
 *  Copyright (C) 1998,1999 NINTENDO Co.,Ltd.
 * 
 */

/* 1999-02-12 */

#include <ultra64.h>
#include <hvq2dec.h>
#include "system.h"


/*
 * Work area needed by the HVQ2 library
 *
 * (Note) MAXWIDTH and MAXHEIGHT are set with Makefile
 */
#if 0
/*
 * If the compressed data for decoding is only in 4:1:1 format:
 */
	u16 hvqwork[(MAXWIDTH/8)*(MAXHEIGHT/8)*6];
#else
/*
 * If there is (also) data in 4:2:2 format for decoding:
 */
	u16 hvqwork[(MAXWIDTH/8)*(MAXHEIGHT/4)*4];
#endif


#if USE_RSP

/*
 * Data area for the HVQ2 microcode
 */
HVQ2Info hvq_spfifo[HVQ_SPFIFO_SIZE];

/*
 * Buffer for RSP task yield
 */
u64 hvq_yieldbuf[HVQ2_YIELD_DATA_SIZE/8];

#endif

/* end */
