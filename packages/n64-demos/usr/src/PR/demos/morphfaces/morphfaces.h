/*
 * Header file for 'morphfaces' program
 *
 * Included by: morphfaces.c; makeverts.c; morph.c; static.c;
 *              cfb.c; zbuf.c; spec
 *
 * This file #include's <ultra64.h> as soon as it gets past the
 * stuff which has to be used by the spec file
 *
 */

#ifndef __MORPHFACES_H__
#define __MORPHFACES_H__

/*
 * Configuration stuff...
 */

#if 0 // This breaks SOMETHING, need to find out what
/*
 * Set this to do 32 bit frame buffer
 */
#define FB32BIT
#endif

/*
 * Set this to do textures
 */
#define DOTEX

/*
 * Set this to do mip-mapping (DOTEX must already be #defined!)
 */
/* #define DOMM */

/*
 * Set this to turn on Z-buffered rendering, which should be irrelevant for
 * this demo.
 */
#define DOZBUF

/*
 * Now, do the #define's required by the 'spec' file first.  The spec file
 * does not agree well with C code, which comes later in this file.
 */

/*
 * Define stack size for R4300 threads
 * This definition is in a header file because it is required by both
 * morphfaces.c (for declaring the stacks), and by the spec file for
 * indicating the top of the stack.
 *
 */

/* Stacksize in bytes */
#define STACKSIZE 0x2000

/*
 * Define RSP segment numbers
 * These definitions are required both by the 'spec' file, and by the
 * code in 'morphfaces.c' which sets up the RSP segment table.
 */

#define STATIC_SEG  1
#define DYNAMIC_SEG 2

/*
 * Now do the stuff only required by .c files, and not by the spec file
 */
#ifdef _LANGUAGE_C

#include <ultra64.h>

/*
 * Define screensize.
 * This definition is in a header file because it is required by the
 * cfb.c and zbuf.c files, as well as the morphfaces.c and static.c files.
 * Note that changing these variables is not sufficient to change the
 * screen size -- the video modes must be changed as well in morphfaces.c,
 * and probably some of the init stuff in static.c.
 */

#define SCREEN_HT 240
#define SCREEN_WD 320

/*
 * Define dynamic data structure
 * This definition is required both by 'morphfaces.c' and by 'dynamic.c'
 * The R4300 addressible version (and the one that reserves physical memory)
 * is defined in morphfaces.c.  The RSP addressible version is defined in
 * dynamic.c
 */

#define VCNT 600

#define GLIST_SIZE 2048
typedef struct {
  Mtx projection;
  Mtx modeling_translate;
  Mtx modeling_rotate1;
  Gfx glist[GLIST_SIZE];
  Vtx tvface1[VCNT];   /* Transformed verticies for face #1 */
  Vtx tvface2[VCNT];   /* Transformed verticies for face #2 */
} dynamic_stuff;

extern u64 dram_stack[];

#endif  /* _LANGUAGE_C */

#endif  /* __MORPHFACES_H__ */
