/* 
 *  N64-HVQ2 library  Sample program
 * 
 *  FILE : hvqbuf.c
 * 
 *  Copyright (C) 1998,1999 NINTENDO Co.,Ltd.
 * 
 */

/* 1999-02-19 */

#include <ultra64.h>
#include "system.h"

/*
 * Buffer into which compressed image data is read.
 * (Note) Please locate at a 16byte-aligned  
 *        address with the spec file.
 */
u8 HVQbuf[HVQ_DATASIZE_MAX];

/* end */
