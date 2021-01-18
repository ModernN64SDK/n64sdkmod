/*
 * Header file for 'morphdemo' program
 *
 * Included by: morphdemo.c; makeverts.c; morph.c; static.c;
 *              cfb.c; zbuf.c; spec
 *
 * This file #include's <ultra64.h> as soon as it gets past the
 * stuff which has to be used by the spec file
 *
 */

#ifndef __MORPHDEMO_H__
#define __MORPHDEMO_H__

/*
 * Configuration stuff...
 */

/*
 * Set this to do lighting
 */
#define DO_LIGHTING

/*
 * Set this to do textures
 */
#define DOTEX

/*
 * Set this to do mip-mapping (DOTEX must already be #defined!)
 */
#define DOMM

/*
 * Set this to change sphere size so that sphere radius is equal to distance
 * to corners of cube instead of distance to center of face of cube.
 * Note that you can begin to see the verticies along the silhouette of the
 * sphere when you turn this on.
 */
/* #define BIGSPHERE */

/*
 * Set this to turn on Z-buffered rendering, which is not strictly required
 * for this demo, but gives better quality images because the delta-Z values
 * are used for anti-aliasing on the non-silhouette cube edges.
 * Performance is reduced somewhat by Z-buffered rendering.
 */
#define DOZBUF

/*
 * Now, do the #define's required by the 'spec' file first.  The spec file
 * does not agree well with C code, which comes later in this file.
 */

/*
 * Define stack size for R4300 threads
 * This definition is in a header file because it is required by both
 * morphdemo.c (for declaring the stacks), and by the spec file for
 * indicating the top of the stack.
 *
 */

/* Stacksize in bytes */
#define STACKSIZE 0x2000

/*
 * Define RSP segment numbers
 * These definitions are required both by the 'spec' file, and by the
 * code in 'morphdemo.c' which sets up the RSP segment table.
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
 * cfb.c and zbuf.c files, as well as the morphdemo.c and static.c files.
 * Note that changing these variables is not sufficient to change the
 * screen size -- the video modes must be changed as well in morphdemo.c,
 * and probably some of the init stuff in static.c.
 */

#define SCREEN_HT 240
#define SCREEN_WD 320

/*
 * Define dynamic data structure
 * This definition is required both by 'morphdemo.c' and by 'dynamic.c'
 * The R4300 addressible version (and the one that reserves physical memory)
 * is defined in morphdemo.c.  The RSP addressible version is defined in
 * dynamic.c
 */

/* See makeverts.c for details...
 * Currently NVE can't be > 8 */
#define NVE 8
#define VCNT (NVE*NVE*6)

#define GLIST_SIZE 2048
typedef struct {
  Mtx projection;
  Mtx modeling_translate;
  Mtx modeling_rotate1;
  Mtx modeling_rotate2;
  Mtx viewing;  /* Currently unused           */
  Mtx identity; /* Currently unset and unused */
  Gfx glist[GLIST_SIZE];
  Vtx mvlist[VCNT]; /* Verticies for morph */
} dynamic_stuff;

extern u64 dram_stack[];

#endif  /* _LANGUAGE_C */

#endif  /* __MORPHDEMO_H__ */
