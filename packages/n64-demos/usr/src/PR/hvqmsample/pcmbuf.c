/* 
 *  N64-HVQM2 library  Sample program
 * 
 *  FILE : pcmbuf.c
 * 
 *  Copyright (C) 1998,1999 NINTENDO Co.,Ltd.
 * 
 */

/* 1999-04-12 */

#include <ultra64.h>
#include "system.h"

/* PCM data buffer
 * 
 * (Note) pcmbuf[i] must have 8byte alignment.
 * Please set to an 8byte aligned address with the spec file.
 * (PCMBUF_SIZE is aligned with system.h)
 */
s16 pcmbuf[NUM_PCMBUFs][PCMBUF_SIZE];

/* end */
