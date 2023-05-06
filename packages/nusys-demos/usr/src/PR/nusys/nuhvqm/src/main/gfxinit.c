/*
   gfxinit.c
   
   Display list for initializing graphics
*/

#include <nusys.h>
#include "graphic.h"

/*
  Viewport structure
  Conversion from (-1,-1,-1)-(1,1,1).  2bit decimal fraction.
 */
static Vp vp = {
    SCREEN_WD*2, SCREEN_HT*2, G_MAXZ/2, 0,	/* Scale factor */
    SCREEN_WD*2, SCREEN_HT*2, G_MAXZ/2, 0,	/* Movement */
};

/*
  Initialize RDP
*/
Gfx setup_rdpstate[] = {
  gsDPSetRenderMode(G_RM_OPA_SURF, G_RM_OPA_SURF2),
  gsDPSetCombineMode(G_CC_SHADE, G_CC_SHADE),
  gsDPSetColorDither(G_CD_BAYER),
  gsSPEndDisplayList(),
};

/*
  Initialize RSP
*/
Gfx setup_rspstate[] = {
  gsSPViewport(&vp),
  gsSPClearGeometryMode(0xFFFFFFFF),
  gsSPSetGeometryMode(G_ZBUFFER | G_SHADE | G_SHADING_SMOOTH | G_CULL_BACK),
  gsSPTexture(0, 0, 0, 0, G_OFF),
  gsSPEndDisplayList(),
};
