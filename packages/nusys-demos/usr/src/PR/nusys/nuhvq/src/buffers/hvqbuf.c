/*
 *  N64-HVQ library  Sample program
 * 
 *  FILE : hvqbuf.c
 *
 *  (C) 1998 HUDSON SOFT
 *
 *  Author:	Itagaki Fumihiko, HUDSON, Oct. 1998
 */

#include <ultra64.h>
#include "localdef.h"
/*
 * Buffer in which compressed image data is loaded
 * (Note) Please locate at a 16byte-aligned  
 *        address with the spec file.
 */
u8 HVQbuf[HVQ_DATASIZE_MAX];
/* end */
