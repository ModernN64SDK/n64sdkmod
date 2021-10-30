/*============================================================================
  NuSYSTEM sample program [SNAKE TAIL HACK]
  
  gfxinit.c
  
  Copyright (C) 1997, NINTENDO Co,Ltd.
  ============================================================================*/

#include <ultra64.h>
#include "graphic.h"

/* The viewport structure  */
static Vp vp = {
    SCREEN_WD*2, SCREEN_HT*2, G_MAXZ/2, 0, /* scale */
    SCREEN_WD*2, SCREEN_HT*2, G_MAXZ/2, 0, /* translate */
};

Lights1 sun_light = gdSPDefLights1(  80,  80,  80, /* no ambient light */
				   200, 200, 200, /* white light */
				   0,   -1,   1);

/* Setting RDP  */
Gfx setup_rdpstate[] = {
    gsDPSetRenderMode(G_RM_OPA_SURF, G_RM_OPA_SURF2),
    gsDPSetColorDither(G_CD_BAYER),
    gsDPSetFogColor(255, 0, 0, 255),
    gsDPSetScissor(G_SC_NON_INTERLACE, 0,0, 319,239),
    gsDPPipeSync(),
    gsSPEndDisplayList(),
};

/* Setting RSP  */
Gfx setup_rspstate[] = {
    gsSPViewport(&vp),
    gsSPClearGeometryMode((G_ZBUFFER | G_SHADE | G_SHADING_SMOOTH | 
			   G_CULL_BOTH | G_FOG | G_LIGHTING)),
    gsSPTexture(0, 0, 0, 0, G_OFF),
    gsSPSetLights1(sun_light),
    gsSPFogPosition(996, 1000),
    gsSPEndDisplayList(),
};
