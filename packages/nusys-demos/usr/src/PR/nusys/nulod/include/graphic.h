/*
   graphic.h

   Graphics-related definitions 

   Copyright (C) 1997-1999, NINTENDO Co,Ltd.			
*/

#ifndef _GRAPHIC_H_
#define _GRAPHIC_H_

/* Screen size */
#define SCREEN_HT        240
#define SCREEN_WD        320

/* Maximum length of the display list for a single task. */
#define GFX_GLIST_LEN     2048

/* Set up color combiner (used in LOD model) */
#define G_CC_MULPRIMSHADE PRIMITIVE,0,SHADE,0,PRIMITIVE,0,SHADE,0

/*-------------------------- define structure ------------------------------ */
/* Projection matrix structure */
typedef struct {
  Mtx     projection;
  Mtx     viewing;
  Mtx     modeling;
  Mtx     scaling;
  Mtx     translate;
} Dynamic;

/*-------------------------------- parameter---------------------------------*/
extern Dynamic gfx_dynamic[];
extern Gfx* glistp;
extern Gfx gfx_glist[][GFX_GLIST_LEN];
extern u32 gfx_gtask_no;

extern Gfx* lodlistp;
extern Gfx lod_glist[][16];
extern u32 lod_gtask_no;
/*-------------------------------- function ---------------------------------*/
extern void gfxRCPInit(void);
extern void gfxClearCfb(void);
/*------------------------------- other extern define -----------------------*/
extern Gfx setup_rdpstate[];
extern Gfx setup_rspstate[];

#endif /* _GRAPHIC_H_ */



