/*
   main.h

   Copyright (C) 1997-1999, NINTENDO Co,Ltd.			
*/

#ifndef MAIN_H
#define MAIN_H

#define STATIC_SEGMENT	1	/* Segment number for LOD model */
#define GFX_DL_BUF_SIZE	0x3000	/* Buffer size for display list */

#ifdef _LANGUAGE_C

#include <nusys.h>

/* Declare external variables */
extern NUContData	contdata[1]; /* Data to be read for one controller */
extern u8 contPattern;		     /* Pattern for the connected controllers */

/* lodmodel.c */
//extern Gfx lodmodel_dl[];	/* Display list of LOD model */
extern Gfx near__dl[];
extern Gfx middle__dl[];
extern Gfx far__dl[];

/* gfx_dl_buf.c */
extern Gfx gfx_dl_buf[];	/* Buffer for display list */


/* segment */
extern u8 _lodmodelSegmentRomStart[];
extern u8 _lodmodelSegmentRomEnd[];

#endif /* _LANGUAGE_C */
#endif /* MAIN_H */




