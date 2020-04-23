
/**************************************************************************
 *                                                                        *
 *               Copyright (C) 1995, Silicon Graphics, Inc.               *
 *                                                                        *
 *  These coded instructions, statements, and computer programs  contain  *
 *  unpublished  proprietary  information of Silicon Graphics, Inc., and  *
 *  are protected by Federal copyright  law.  They  may not be disclosed  *
 *  to  third  parties  or copied or duplicated in any form, in whole or  *
 *  in part, without the prior written consent of Silicon Graphics, Inc.  *
 *                                                                        *
 *************************************************************************/

/*
 * File:	static.c
 * Creator:	hsa@sgi.com
 * Create Date:	Sun Jun  4 18:49:42 PDT 1995
 *
 *
 * This file holds tiny display list segments that are 'static' data.
 *
 */

#include <ultra64.h>
#include "teapot.h"

/*
 * Remember, viewport structures have 2 bits of fraction in them.
 */
static Vp vp = {
	SCREEN_WD*2, SCREEN_HT*2, G_MAXZ/2, 0,	/* scale */
	SCREEN_WD*2, SCREEN_HT*2, G_MAXZ/2, 0,	/* translate */
};

Gfx rspinit_dl[] = {
    gsSPViewport(&vp),
    gsSPClearGeometryMode(G_SHADE | G_SHADING_SMOOTH | G_CULL_BOTH |
			  G_FOG | G_LIGHTING | G_TEXTURE_GEN |
			  G_TEXTURE_GEN_LINEAR | G_LOD ),
    gsSPTexture(0, 0, 0, 0, G_OFF),
    gsSPSetGeometryMode(G_SHADE | G_SHADING_SMOOTH),
    gsSPEndDisplayList(),
};

Gfx rdpinit_dl[] = {
    gsDPSetScissor(G_SC_NON_INTERLACE, 0, 0, SCREEN_WD, SCREEN_HT),
    gsDPSetCombineMode(G_CC_SHADE, G_CC_SHADE),
    gsDPSetTextureLOD(G_TL_TILE),
    gsDPSetTextureLUT(G_TT_NONE),
    gsDPSetTextureDetail(G_TD_CLAMP),
    gsDPSetTexturePersp(G_TP_PERSP),
    gsDPSetTextureFilter(G_TF_BILERP),
    gsDPSetTextureConvert(G_TC_FILT),
    gsDPSetCombineKey(G_CK_NONE),
    gsDPSetAlphaCompare(G_AC_NONE),
    gsDPSetRenderMode(G_RM_OPA_SURF, G_RM_OPA_SURF2),
    gsDPSetBlendMask(0xff),
    gsDPSetColorDither(G_CD_ENABLE),
    gsDPSetCycleType(G_CYC_1CYCLE),
    gsDPPipeSync(),
    gsSPEndDisplayList(),
};

#ifdef USE_HIGHLIGHT
/* texture for the specular highlight: */
#include "IA8phong.h"
#endif

#include "teapot_vtx.h"

Lights2 litc2 = gdSPDefLights2(0x5, 0x5, 0x5,			/* ambient color */
			       100, 100, 0,			/* light color */
			       32, 64, 0, 			/* normal */
			       50, 50, 0,			/* light color */
			       -50, 50, 0);			/* normal */
			       /*50/4, 100/4, 400/4);		/* normal */



Gfx teapot_setup_dl[] = {
    gsDPPipeSync(),
    gsDPSetCycleType(G_CYC_1CYCLE),
    gsSPSetGeometryMode(G_ZBUFFER | G_SHADE | G_SHADING_SMOOTH | 
			G_CULL_BACK | G_LIGHTING),
    gsDPSetColorDither(G_CD_BAYER),
    gsSPSetLights2(litc2),
#ifdef USE_HIGHLIGHT
    gsDPSetCycleType(G_CYC_2CYCLE),
    gsSPSetGeometryMode(G_TEXTURE_GEN),
    gsSPTexture(0x07c0, 0x07c0, 0, G_TX_RENDERTILE, G_ON),
    gsDPSetTexturePersp(G_TP_PERSP),
    gsDPSetTextureFilter(G_TF_BILERP),
    gsDPSetPrimColor(0, 0, 255, 255, 255, 255),
    gsDPSetEnvColor(64, 64, 64, 255),
    gsDPSetCombineMode(G_CC_HILITERGB, G_CC_HILITERGB2),
    gsDPSetRenderMode(G_RM_PASS, G_RM_AA_ZB_OPA_SURF2),
    gsDPLoadTextureBlock(IA8phong, G_IM_FMT_IA, G_IM_SIZ_8b, 32, 32, 0,
			 G_TX_WRAP | G_TX_NOMIRROR, G_TX_WRAP | G_TX_NOMIRROR,
			 5, 5, G_TX_NOLOD, G_TX_NOLOD),
    gsDPSetTile(G_IM_FMT_IA, G_IM_SIZ_8b, ((((32) * G_IM_SIZ_8b_LINE_BYTES)+7)>>3),
                        0, G_TX_RENDERTILE+1, 0, G_TX_WRAP | G_TX_NOMIRROR,
                        5, G_TX_NOLOD, G_TX_WRAP | G_TX_NOMIRROR, 5, G_TX_NOLOD),
#else
    gsDPSetCombineMode (G_CC_SHADE, G_CC_SHADE),
    gsDPSetRenderMode(G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2),
#endif

    gsSPEndDisplayList(),
};

Gfx teapot_geom_dl[] = {

#include "teapot_tri.h"

    gsSPEndDisplayList(),
};

static Vtx shad_vtx[] =  {
	{ -23, 0, -32, 0, (63 << 6), (44 << 6), 0, 0, 0, 0xff },
	{ 23, 0, -32, 0, (63 << 6), (0 << 6), 0, 0, 0, 0xff },
	{ -23, 0, 32, 0, (0 << 6), (44 << 6), 0, 0, 0, 0xff },
	{ 23, 0, 32, 0, (0 << 6), (0 << 6), 0, 0, 0, 0xff },
};

#include "teapot_shad64.h"

Gfx shadow_setup_dl[] = {

#define INVERT_TEX_PRIM         1, TEXEL0, PRIMITIVE, 0, 0, 0, 0, TEXEL0

    gsDPPipeSync(),
    gsDPSetCycleType(G_CYC_1CYCLE),
    gsSPClearGeometryMode(G_CULL_BOTH),
    gsSPSetGeometryMode(G_ZBUFFER | G_SHADE),
    gsDPSetPrimColor(0, 0, 0x30, 0x30, 0x30, 0xff),
    gsDPSetCombineMode(INVERT_TEX_PRIM, INVERT_TEX_PRIM),
    gsDPSetRenderMode(G_RM_ZB_XLU_SURF, G_RM_ZB_XLU_SURF2),

    gsSPTexture(0x8000, 0x8000, 0, G_TX_RENDERTILE, G_ON),
    gsDPSetColorDither(G_CD_DISABLE),
    gsDPSetTexturePersp(G_TP_PERSP),
    gsDPSetTextureLOD(G_TL_TILE),
    gsDPSetTextureFilter(G_TF_BILERP),

    gsDPLoadTextureBlock_4b(teapot_shadow, G_IM_FMT_I, 64, 45, 0,
		    G_TX_WRAP | G_TX_NOMIRROR, G_TX_WRAP | G_TX_NOMIRROR,
		    G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD),
    gsDPPipeSync(),

    gsSPEndDisplayList(),
};

Gfx shadow_geom_dl[] = {

    gsSPVertex(&shad_vtx, 4, 0),
    gsSP1Triangle(0, 2, 1, 0),
    gsSP1Triangle(1, 2, 3, 0),

    gsDPPipeSync(),
    gsDPSetColorDither(G_CD_ENABLE),

    gsSPEndDisplayList(),
};

#ifdef USE_MIPMAP
#   include "../Texture/RGBA16brickMM.h"
#else
#   include "../Texture/RGBA16brick.h"
#endif

Gfx floor_setup_dl[] = {
    gsDPPipeSync (),
    gsSPClearGeometryMode(G_SHADE | G_SHADING_SMOOTH | G_CULL_BOTH |
			  G_FOG | G_LIGHTING | G_TEXTURE_GEN |
			  G_TEXTURE_GEN_LINEAR | G_LOD ),
    gsSPSetGeometryMode(G_ZBUFFER),
    gsDPSetTexturePersp(G_TP_PERSP),
    gsDPSetTextureFilter(G_TF_BILERP),

#ifdef USE_MIPMAP
    gsDPSetRenderMode(G_RM_PASS, G_RM_AA_ZB_OPA_SURF2),
    gsSPTexture(0x8000, 0x8000, 5, G_TX_RENDERTILE, G_ON),
    gsDPSetCombineMode(G_CC_TRILERP, G_CC_DECALRGB2),
    gsDPSetTextureDetail (G_TD_CLAMP),

    /* set up all the tiles */
    gsSPDisplayList(RGBA16brickMM_dl),

    gsDPSetTextureLOD(G_TL_LOD),
    gsDPPipelineMode(G_PM_1PRIMITIVE),
    gsDPSetCycleType(G_CYC_2CYCLE),
#else
    gsDPSetRenderMode(G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2),
    gsSPTexture(0x8000, 0x8000, 0, G_TX_RENDERTILE, G_ON),
    gsDPSetCombineMode(G_CC_DECALRGB, G_CC_DECALRGB),
    gsDPLoadTextureBlock(RGBA16brick, G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 32, 0,
			 G_TX_WRAP | G_TX_NOMIRROR, G_TX_WRAP | G_TX_NOMIRROR,
			 5, 5, G_TX_NOLOD, G_TX_NOLOD),
#endif
    gsSPEndDisplayList(),
};

static Vtx floor_vtx[25] =  {
	{ -64, 0, -64, 0, (0 << 6), (0 << 6), 0, 0, 0, 0xff },
	{ -32, 0, -64, 0, (31 << 6), (0 << 6), 0, 0, 0, 0xff },
	{ 0, 0, -64, 0, (63 << 6), (0 << 6), 0, 0, 0, 0xff },
	{ 32, 0, -64, 0, (95 << 6), (0 << 6), 0, 0, 0, 0xff },
	{ 64, 0, -64, 0, (127 << 6), (0 << 6), 0, 0, 0, 0xff },
	{ -64, 0, -32, 0, (0 << 6), (31 << 6), 0, 0, 0, 0xff },
	{ -32, 0, -32, 0, (31 << 6), (31 << 6), 0, 0, 0, 0xff },
	{ 0, 0, -32, 0, (63 << 6), (31 << 6), 0, 0, 0, 0xff },
	{ 32, 0, -32, 0, (95 << 6), (31 << 6), 0, 0, 0, 0xff },
	{ 64, 0, -32, 0, (127 << 6), (31 << 6), 0, 0, 0, 0xff },
	{ -64, 0, 0, 0, (0 << 6), (63 << 6), 0, 0, 0, 0xff },
	{ -32, 0, 0, 0, (31 << 6), (63 << 6), 0, 0, 0, 0xff },
	{ 0, 0, 0, 0, (63 << 6), (63 << 6), 0, 0, 0, 0xff },
	{ 32, 0, 0, 0, (95 << 6), (63 << 6), 0, 0, 0, 0xff },
	{ 64, 0, 0, 0, (127 << 6), (63 << 6), 0, 0, 0, 0xff },
	{ -64, 0, 32, 0, (0 << 6), (95 << 6), 0, 0, 0, 0xff },
	{ -32, 0, 32, 0, (31 << 6), (95 << 6), 0, 0, 0, 0xff },
	{ 0, 0, 32, 0, (63 << 6), (95 << 6), 0, 0, 0, 0xff },
	{ 32, 0, 32, 0, (95 << 6), (95 << 6), 0, 0, 0, 0xff },
	{ 64, 0, 32, 0, (127 << 6), (95 << 6), 0, 0, 0, 0xff },
	{ -64, 0, 64, 0, (0 << 6), (127 << 6), 0, 0, 0, 0xff },
	{ -32, 0, 64, 0, (31 << 6), (127 << 6), 0, 0, 0, 0xff },
	{ 0, 0, 64, 0, (63 << 6), (127 << 6), 0, 0, 0, 0xff },
	{ 32, 0, 64, 0, (95 << 6), (127 << 6), 0, 0, 0, 0xff },
	{ 64, 0, 64, 0, (127 << 6), (127 << 6), 0, 0, 0, 0xff },
};

Gfx floor_geom_dl[] = {

#define	floorPatch(a)	gsSPVertex(&(floor_vtx[(a)]), 10, 0),	\
    gsSP1Triangle(0, 1, 5, 0),					\
    gsSP1Triangle(1, 5, 6, 0),					\
    gsSP1Triangle(1, 2, 6, 0),					\
    gsSP1Triangle(2, 6, 7, 0),					\
    gsSP1Triangle(2, 3, 7, 0),					\
    gsSP1Triangle(3, 7, 8, 0),					\
    gsSP1Triangle(3, 4, 8, 0),					\
    gsSP1Triangle(4, 8, 9, 0)

    floorPatch(0),
    floorPatch(5),
    floorPatch(10),
    floorPatch(15),

    gsSPEndDisplayList(),
};
