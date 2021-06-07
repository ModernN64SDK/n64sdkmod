/*============================================================================
  gfxinit.c
============================================================================*/

#define F3DEX_GBI
#include <ultra64.h>
#include <PR/ramrom.h>
#include <assert.h>

#include "def.h"
/*
 * Remember, viewport structures have 2 bits of fraction in them.
 */
static Vp vp = {
    SCREEN_WD*2, SCREEN_HT*2, G_MAXZ/2, 0,	/* scale */
    SCREEN_WD*2, SCREEN_HT*2, G_MAXZ/2, 0,	/* translate */
};

#if 0
Lights1 sun_light = gdSPDefLights1( 80,  80,  80,  /* no ambient light */
				   200, 200, 200,  /* white light */
				     1,   1,  -1
				   );
#else
Lights1 sun_light = gdSPDefLights1( 80,  80,  80,
				   200, 200, 200,
				   100, 100, 10
				  );
#endif
/*
 * This display list initializes the RDP the first time. After this, only
 * the contents of the texture memory is undefined.
 * Note that some of the RDP state is initialized everytime the gfx
 * microcode is loaded.  See gspFast3D (3P) for details
 */
Gfx rdpstateinit_dl[] = {

    /* set all of the attribute registers to zero */
    gsDPSetEnvColor(0,0,0,0),
    gsDPSetPrimColor(0,0,0,0,0,0),
    gsDPSetBlendColor(0,0,0,0),
    gsDPSetFogColor(0,0,0,0),
    gsDPSetFillColor(0),
    gsDPSetPrimDepth(0,0),
    gsDPSetConvert(0,0,0,0,0,0),
    gsDPSetKeyR(0,0,0),
    gsDPSetKeyGB(0,0,0,0,0,0),

    /* set combine mode */
    gsDPSetCombineMode(G_CC_SHADE, G_CC_SHADE),

    /* initialize the scissor box */
    gsDPSetScissor(G_SC_NON_INTERLACE, 0, 0, SCREEN_WD, SCREEN_HT),

    /* initialize all the texture tile descriptors to zero */
    gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
    gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0),
    gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0),
    gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0),
    gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0),
    gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0),
    gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0),
    gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0),

    gsDPSetTileSize(0, 0, 0, (1 - 1) << G_TEXTURE_IMAGE_FRAC, (1 - 1) <<
		G_TEXTURE_IMAGE_FRAC),
    gsDPSetTileSize(1, 0, 0, (1 - 1) << G_TEXTURE_IMAGE_FRAC, (1 - 1) <<
		G_TEXTURE_IMAGE_FRAC),
    gsDPSetTileSize(2, 0, 0, (1 - 1) << G_TEXTURE_IMAGE_FRAC, (1 - 1) <<
		G_TEXTURE_IMAGE_FRAC),
    gsDPSetTileSize(3, 0, 0, (1 - 1) << G_TEXTURE_IMAGE_FRAC, (1 - 1) <<
		G_TEXTURE_IMAGE_FRAC),
    gsDPSetTileSize(4, 0, 0, (1 - 1) << G_TEXTURE_IMAGE_FRAC, (1 - 1) <<
		G_TEXTURE_IMAGE_FRAC),
    gsDPSetTileSize(5, 0, 0, (1 - 1) << G_TEXTURE_IMAGE_FRAC, (1 - 1) <<
		G_TEXTURE_IMAGE_FRAC),
    gsDPSetTileSize(6, 0, 0, (1 - 1) << G_TEXTURE_IMAGE_FRAC, (1 - 1) <<
		G_TEXTURE_IMAGE_FRAC),
    gsDPSetTileSize(7, 0, 0, (1 - 1) << G_TEXTURE_IMAGE_FRAC, (1 - 1) <<
		G_TEXTURE_IMAGE_FRAC),


    gsDPPipeSync(),
    gsSPEndDisplayList(),
};



Gfx setup_rdpstate[] = {

    /*
     * This RDP initialization sequence will setup the following modes
     *
     * 1 cycle mode, point sampled, non-zbuffer, smooth shaded opaque polygons
     */

    /* bl */
  gsDPSetRenderMode(G_RM_OPA_SURF, G_RM_OPA_SURF2),
  gsDPSetColorDither(G_CD_BAYER),
  gsDPSetFogColor(255, 0, 0, 255),
  gsDPSetScissor(G_SC_NON_INTERLACE, 0,0, 319,239),
  gsDPPipeSync(),
  gsSPEndDisplayList(),
};

/* intialize the RSP state: */
Gfx setup_rspstate[] = {
  gsSPViewport(&vp),
  gsSPClearGeometryMode((G_ZBUFFER | G_SHADE | G_SHADING_SMOOTH | 
			 G_CULL_BOTH | G_FOG | G_LIGHTING)),
  gsSPTexture(0, 0, 0, 0, G_OFF),
  gsSPSetLights1(sun_light),
/*  gsSPFogPosition(996, 1000),*/
  gsSPEndDisplayList(),
};

Vtx bullet_vtx[] = {
  {   1,    0,   20,   0,   0,   0, 0xff, 0xff, 0x00, 0xff},
  {  -1,    0,   20,   0,   0,   0, 0xff, 0xff, 0x00, 0xff},
  {  -1,    0,  -20,   0,   0,   0, 0xff, 0xff, 0x00, 0xff},
  {   1,    0,  -20,   0,   0,   0, 0xff, 0xff, 0x00, 0xff},
};

Gfx bullet_dl[] = {
  gsDPPipeSync(),
  gsDPSetCycleType(G_CYC_1CYCLE),
  gsDPSetRenderMode(G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2),
  gsSPClearGeometryMode((G_SHADE|G_SHADING_SMOOTH|G_LIGHTING|G_TEXTURE_GEN|G_TEXTURE_GEN_LINEAR|G_CULL_BOTH|G_FOG)),
  gsSPSetGeometryMode( G_ZBUFFER | G_SHADE | G_SHADING_SMOOTH),
  gsDPSetCombineMode(G_CC_PRIMITIVE, G_CC_PRIMITIVE),
  gsSPVertex(&bullet_vtx[0], 4 ,0),
  gsSP1Triangle(0, 1, 3, 0),
  gsSP1Triangle(1, 2, 3, 0),
  gsSPEndDisplayList(),
};
