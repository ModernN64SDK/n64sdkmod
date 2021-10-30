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
/* Length of the display list for buffer clear  */
#define GFX_CLEAR_GLIST_LEN     512

/*-------------------------- define structure ------------------------------ */
/* Projection matrix structure */
typedef struct {
  Mtx     projection;
  Mtx     modeling;
  Mtx     translate;
} Dynamic;

/*-------------------------------- parameter---------------------------------*/
extern Dynamic gfx_dynamic[];
extern Gfx* glistp;
extern Gfx gfx_glist[][GFX_GLIST_LEN];
extern Gfx gfx_clear_glist[][GFX_CLEAR_GLIST_LEN];
extern u32 gfx_gtask_no;
/*-------------------------------- function ---------------------------------*/
extern void gfxRCPInit(void);
extern void gfxClearCfb(void);
/* Draw a square */
extern void shadetri(Dynamic* dynamicp);
/*------------------------------- other extern define -----------------------*/
extern Gfx setup_rdpstate[];
extern Gfx setup_rspstate[];

#endif /* _GRAPHIC_H_ */



