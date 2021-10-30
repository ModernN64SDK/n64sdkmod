/*
   graphic.h

   The definition of around graphic

   Copyright (C) 1997-1999, NINTENDO Co,Ltd.	
*/

#ifndef _GRAPHIC_H_
#define _GRAPHIC_H_

/* The screen size */
#define SCREEN_HT        240
#define SCREEN_WD        320

/* The maximum length of the display list of one task */
#define GFX_GLIST_LEN     2048

/*-------------------------- define structure ------------------------------ */
/* The structure of the projection-matrix */
typedef struct {
  Mtx     projection;
  Mtx     modeling;
  Mtx     translate;
} Dynamic;

/*-------------------------------- parameter---------------------------------*/
extern Dynamic gfx_dynamic[];
extern Gfx* glistp;
extern Gfx gfx_glist[][GFX_GLIST_LEN];
extern u32 gfx_gtask_no;
/*-------------------------------- function ---------------------------------*/
extern void gfxRCPInit(void);
extern void gfxClearCfb(void);
/*------------------------------- other extern define -----------------------*/
extern Gfx setup_rdpstate[];
extern Gfx setup_rspstate[];
/*------------------------------ GRAFIC MANEGER DEFINE ----------------------*/
#define	APP_GFX_UCODE_F3DEX2	0		/* F3DEX2 micro code  */
#define	APP_GFX_UCODE_S2DEX2	1		/* S2DEX2 micro code  */

#endif /* _GRAPHIC_H_ */



