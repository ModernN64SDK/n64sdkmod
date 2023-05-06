/*
   gfxinit.c
   
   Display list for graphics initialization. 

   Copyright (C) 1997-1999, NINTENDO Co,Ltd.			
*/

#include <nusys.h>
#include "graphic.h"

/*
  The Viewport structure
  Conversion for range (-1,-1,-1)-(1,1,1).  2-bit fractional portion. 
 */
static Vp vp = {
    SCREEN_WD*2, SCREEN_HT*2, G_MAXZ/2, 0,	/* Magnification */
    SCREEN_WD*2, SCREEN_HT*2, G_MAXZ/2, 0,	/* Movement */
};

Lights1 sun_light = gdSPDefLights1(  80,  80,  80, /* no ambient light */
				    200, 200, 200, /* white light */
				      1,   1,  -1);
/*
   Initialize the RDP. 
*/
Gfx setup_rdpstate[] = {
  gsDPSetRenderMode(G_RM_OPA_SURF, G_RM_OPA_SURF2),
  gsDPSetColorDither(G_CD_BAYER),
  gsDPSetFogColor(255, 0, 0, 255),
  gsDPSetScissor(G_SC_NON_INTERLACE, 0,0, SCREEN_WD,SCREEN_HT),
  gsDPSetCombineMode(G_CC_SHADE, G_CC_SHADE),
  gsSPEndDisplayList(),
};

/*
    Initialize the RSP. 
*/
Gfx setup_rspstate[] = {
  gsSPViewport(&vp),
  gsSPClipRatio(FRUSTRATIO_3),
  gsSPClearGeometryMode(0xFFFFFFFF),
  gsSPSetGeometryMode(G_ZBUFFER | G_SHADE | G_SHADING_SMOOTH | G_CULL_BACK),
  gsSPTexture(0, 0, 0, 0, G_OFF),
  gsSPSetLights1(sun_light),
  gsSPFogPosition(996, 1000),
  gsSPEndDisplayList(),
};
