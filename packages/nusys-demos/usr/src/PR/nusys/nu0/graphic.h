/*
   graphic.h

   The definition of graphic and around

   Copyright (C) 1997-1999, NINTENDO Co,Ltd.	
*/

#ifndef _GRAPHIC_H_
#define _GRAPHIC_H_

/* The screen size  */
#define SCREEN_HT        240
#define SCREEN_WD        320

/* The maximum length of the display list of one task  */
#define GFX_GLIST_LEN     2048

/*-------------------------- define structure ------------------------------ */
/* The projection-matrix structure  */
typedef struct {
  Mtx     projection;
  Mtx     modeling;
} Dynamic;

/*-------------------------------- parameter---------------------------------*/
extern Dynamic gfx_dynamic;
extern Gfx* glistp;
extern Gfx gfx_glist[GFX_GLIST_LEN];
/*-------------------------------- function ---------------------------------*/
extern void gfxRCPInit(void);
extern void gfxClearCfb(void);
/*------------------------------- other extern define -----------------------*/
extern Gfx setup_rdpstate[];
extern Gfx setup_rspstate[];

#endif /* _GRAPHIC_H_ */



