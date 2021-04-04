/*
 *  N64-HVQ library Sample program
 * 
 *  FILE : hvqwork.c
 *
 *  NUSYSTEM-HVQ library Sample program
 */

#include <ultra64.h>
#include <hvq2dec.h>
#include "localdef.h"

/*
 * Work area needed by the HVQ library
 *
 * (Note) MAXWIDTH and MAXHEIGHT are set with Makefile 
 */
#if 0
/*
 *  If the compressed data for decoding is only in 4:1:1 format:
 */
      s16 hvqwork[(MAXWIDTH/8)*(MAXHEIGHT/8)*6];
#else
/*
 *  If there is also data in 4:2:2 format for decoding:
 */
       s16 hvqwork[(MAXWIDTH/8)*(MAXHEIGHT/4)*4];
#endif


/*
 * Data area for the HVQ microcode 
 */
HVQ2Info hvq_spfifo[HVQ_SPFIFO_SIZE];
