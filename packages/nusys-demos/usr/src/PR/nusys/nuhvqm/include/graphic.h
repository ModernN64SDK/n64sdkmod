/*
   graphic.h

   Graphics-related definitions
*/

#ifndef _GRAPHIC_H_
#define _GRAPHIC_H_

/* Screen size */
#define SCREEN_HT        240
#define SCREEN_WD        320

/* Maximum value for length of display list for one task */
#define GFX_GLIST_LEN     2048

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
extern u32 gfx_gtask_no;
/*-------------------------------- function ---------------------------------*/
extern void gfxRCPInit(void);
extern void gfxClearCfb(void);
#if HVQM_CFB_FORMAT == 2
extern void gfxClearCfb32(void);
#endif /* HVQM_CFB_FORMAT */
/*------------------------------- other extern define -----------------------*/
extern Gfx setup_rdpstate[];
extern Gfx setup_rspstate[];

#endif /* _GRAPHIC_H_ */



