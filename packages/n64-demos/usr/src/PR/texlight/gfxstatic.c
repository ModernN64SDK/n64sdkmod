
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
 * File:	gfxstatic.c
 *
 * This file holds display list segments that are 'static' data.
 *
 */

#include <ultra64.h>
#include "boot.h"
#include "game.h"
#include "texture.h"



/***********************************************************************
 ******************* RENDERING SETUP DISPLAY LISTS *********************
 ***********************************************************************/

/*
 *   PLAIN
 */
Gfx setrend[] = {
    gsDPPipeSync(),

    gsDPSetCycleType(G_CYC_1CYCLE),
    gsSPSetGeometryMode(G_ZBUFFER | 
			G_CULL_BACK),
    gsDPSetRenderMode(G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2),
    gsDPSetCombineMode(G_CC_PRIMITIVE, G_CC_PRIMITIVE),

    gsSPEndDisplayList(),
};

/*
 *   BILERP
 */
Gfx setrend_bilerp[] = {
    gsDPPipeSync(),

    gsDPSetCycleType(G_CYC_1CYCLE),
    gsDPSetTextureLOD(G_TL_TILE),
    gsSPSetGeometryMode(G_ZBUFFER | 
			G_CULL_BACK),
    gsDPSetRenderMode(G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2),
    gsDPSetCombineMode(G_CC_DECALRGBA, G_CC_DECALRGBA),

    gsSPEndDisplayList(),
};

/*
 *   TRILERP
 */
Gfx setrend_trilerp[] = {
    gsDPPipeSync(),

    gsDPSetCycleType(G_CYC_2CYCLE),
    gsDPSetTextureLOD(G_TL_LOD),
    gsSPSetGeometryMode(G_ZBUFFER | 
			G_CULL_BACK),
    gsDPSetRenderMode(G_RM_PASS, G_RM_AA_ZB_OPA_SURF2),
    gsDPSetCombineMode(G_CC_TRILERP, G_CC_DECALRGB2),

    gsSPEndDisplayList(),
};

/*
 *   BILERP * PRIM
 */
Gfx setrend_bilerp_prim[] = {
    gsDPPipeSync(),

    gsDPSetCycleType(G_CYC_1CYCLE),
    gsDPSetTextureLOD(G_TL_TILE),
    gsSPSetGeometryMode(G_ZBUFFER | 
			G_CULL_BACK),
    gsDPSetRenderMode(G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2),
    gsDPSetCombineMode(G_CC_MODULATEI_PRIM, G_CC_MODULATEI_PRIM),

    gsSPEndDisplayList(),
};

/*
 *   TRILERP * PRIM
 */
Gfx setrend_trilerp_prim[] = {
    gsDPPipeSync(),

    gsDPSetCycleType(G_CYC_2CYCLE),
    gsDPSetTextureLOD(G_TL_LOD),
    gsSPSetGeometryMode(G_ZBUFFER | 
			G_CULL_BACK),
    gsDPSetRenderMode(G_RM_PASS, G_RM_AA_ZB_OPA_SURF2),
    gsDPSetCombineMode(G_CC_TRILERP, G_CC_MODULATEI_PRIM2),

    gsSPEndDisplayList(),
};

/*
 *   DIFFUSE LIGHT
 */
Gfx setrend_light[] = {
    gsDPPipeSync(),

    gsDPSetCycleType(G_CYC_1CYCLE),
    gsSPSetGeometryMode(G_ZBUFFER | 
			G_CULL_BACK | 
			G_LIGHTING | 
			G_SHADE | 
			G_SHADING_SMOOTH),
    gsDPSetRenderMode(G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2),
    gsDPSetCombineMode(G_CC_SHADE, G_CC_SHADE),

    gsSPEndDisplayList(),
};

/*
 *   DIFFUSE LIGHT BILERP
 */
Gfx setrend_light_bilerp[] = {
    gsDPPipeSync(),

    gsDPSetCycleType(G_CYC_1CYCLE),
    gsDPSetTextureLOD(G_TL_TILE),
    gsSPSetGeometryMode(G_ZBUFFER | 
			G_CULL_BACK | 
			G_LIGHTING | 
			G_SHADE | 
			G_SHADING_SMOOTH),
    gsDPSetRenderMode(G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2),
    gsDPSetCombineMode(G_CC_MODULATERGB, G_CC_MODULATERGB),

    gsSPEndDisplayList(),
};

/*
 *   DIFFUSE LIGHT TRILERP
 */
Gfx setrend_light_trilerp[] = {
    gsDPPipeSync(),

    gsDPSetCycleType(G_CYC_2CYCLE),
    gsDPSetTextureLOD(G_TL_LOD),
    gsSPSetGeometryMode(G_ZBUFFER | 
			G_CULL_BACK | 
			G_LIGHTING | 
			G_SHADE | 
			G_SHADING_SMOOTH),
    gsDPSetRenderMode(G_RM_PASS, G_RM_AA_ZB_OPA_SURF2),
    gsDPSetCombineMode(G_CC_TRILERP, G_CC_MODULATERGB2),

    gsSPEndDisplayList(),
};

#include "I4spec.h"
/*
 *   1 SPECULAR HIGHLIGHT
 */
Gfx setrend_1spec[] = {
    gsDPPipeSync(),

    gsDPSetCycleType(G_CYC_1CYCLE),
    gsDPSetTextureLOD(G_TL_TILE),
    gsSPSetGeometryMode(G_ZBUFFER | 
			G_CULL_BACK | 
			G_LIGHTING |
			G_TEXTURE_GEN),
    gsDPSetRenderMode(G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2),
    gsDPSetCombineMode(G_CC_HILITERGBDECALA, G_CC_HILITERGBDECALA),

    gsSPTexture(0x07c0, 0x07c0, 0, G_TX_RENDERTILE, G_ON),
    gsDPLoadTextureBlock_4b(I4spec,G_IM_FMT_I,32,32, 0,
			G_TX_WRAP | G_TX_NOMIRROR, G_TX_WRAP | G_TX_NOMIRROR,
			5, 5, G_TX_NOLOD, G_TX_NOLOD),
    gsSPEndDisplayList(),
};

/*
 *   2 SPECULAR HIGHLIGHTS
 */
Gfx setrend_2spec[] = {
    gsDPPipeSync(),

    gsDPSetCycleType(G_CYC_2CYCLE),
    gsDPSetTextureLOD(G_TL_TILE),
    gsSPSetGeometryMode(G_ZBUFFER | 
			G_CULL_BACK | 
			G_LIGHTING |
			G_TEXTURE_GEN),
    gsDPSetRenderMode(G_RM_PASS, G_RM_AA_ZB_OPA_SURF2),
    gsDPSetCombineMode(G_CC_HILITERGBDECALA, G_CC_HILITERGBA2),

    gsSPTexture(0x07c0, 0x07c0, 0, G_TX_RENDERTILE, G_ON),
    gsDPLoadTextureBlock_4b(I4spec,G_IM_FMT_I,32,32, 0,
			G_TX_WRAP | G_TX_NOMIRROR, G_TX_WRAP | G_TX_NOMIRROR,
			5, 5, G_TX_NOLOD, G_TX_NOLOD),
    gsDPSetTile(G_IM_FMT_I, G_IM_SIZ_4b, 
			((((32) >>1 )+7)>>3),
                        0, G_TX_RENDERTILE+1, 0, 
			G_TX_WRAP | G_TX_NOMIRROR, 5, G_TX_NOLOD, 
			G_TX_WRAP | G_TX_NOMIRROR, 5, G_TX_NOLOD),

    gsSPEndDisplayList(),
};




/***********************************************************************
 ******************* TEXTURE LOADING DISPLAY LISTS *********************
 ***********************************************************************/

Gfx load_tex_brick_dl[] = {
    gsDPLoadTextureBlock(
		RGBA16brick, G_IM_FMT_RGBA, G_IM_SIZ_16b,
		32, 32, 0,
		G_TX_WRAP, G_TX_WRAP,
		5, 5, G_TX_NOLOD, G_TX_NOLOD),
    gsSPEndDisplayList(),
};

Gfx load_tex_checker_dl[] = {
    gsDPLoadTextureBlock(
		RGBA16checker, G_IM_FMT_RGBA, G_IM_SIZ_16b,
		32, 32, 0,
		G_TX_WRAP, G_TX_WRAP,
		5, 5, G_TX_NOLOD, G_TX_NOLOD),
    gsSPEndDisplayList(),
};

Gfx load_tex_molecule_dl[] = {
    gsDPLoadTextureBlock(
		RGBA16molecule, G_IM_FMT_RGBA, G_IM_SIZ_16b,
		32, 32, 0,
		G_TX_WRAP, G_TX_WRAP,
		5, 5, G_TX_NOLOD, G_TX_NOLOD),
    gsSPEndDisplayList(),
};

Gfx load_tex_chainlink_dl[] = {
    gsDPLoadTextureBlock(
		RGBA16chainlink, G_IM_FMT_RGBA, G_IM_SIZ_16b,
		16, 16, 0,
		G_TX_WRAP, G_TX_WRAP,
		4, 4, G_TX_NOLOD, G_TX_NOLOD),
    gsSPEndDisplayList(),
};


/***********************************************************************
 ******************* MODELS ********************************************
 ***********************************************************************/

/*
 * Teapot ************************************
 */

#include "teapot_vtx.h"

Gfx teapot_dl[] = {
#   include "teapot_tri.h"
    gsSPEndDisplayList(),
};

/*
 * Cube **************************************
 */

static Vtx cube_vtx1[] = {
  { -50,  -50,  -50, 0, 00<<8, 00<<8,    0,    0, -127, 0xff},
  {  50,  -50,  -50, 0, 32<<8, 00<<8,    0,    0, -127, 0xff},
  {  50,   50,  -50, 0, 32<<8, 32<<8,    0,    0, -127, 0xff},
  { -50,   50,  -50, 0, 00<<8, 32<<8,    0,    0, -127, 0xff},
  { -50,  -50,   50, 0, 32<<8, 32<<8,    0,    0,  127, 0xff},
  {  50,  -50,   50, 0, 00<<8, 32<<8,    0,    0,  127, 0xff},
  {  50,   50,   50, 0, 00<<8, 00<<8,    0,    0,  127, 0xff},
  { -50,   50,   50, 0, 32<<8, 00<<8,    0,    0,  127, 0xff},

};
static Vtx cube_vtx2[] = {
  { -50,  -50,  -50, 0, 00<<8, 00<<8,  -90,  -90,   -9, 0xff},
  {  50,  -50,  -50, 0, 00<<8, 32<<8,   90,  -90,   -9, 0xff},
  {  50,   50,  -50, 0, 00<<8, 00<<8,   90,   90,   -9, 0xff},
  { -50,   50,  -50, 0, 00<<8, 32<<8,  -90,   90,   -9, 0xff},
  { -50,  -50,   50, 0, 32<<8, 00<<8,  -90,  -90,    9, 0xff},
  {  50,  -50,   50, 0, 32<<8, 32<<8,   90,  -90,    9, 0xff},
  {  50,   50,   50, 0, 32<<8, 00<<8,   90,   90,    9, 0xff},
  { -50,   50,   50, 0, 32<<8, 32<<8,  -90,   90,    9, 0xff},

};

Gfx cube_dl[] = {
    gsSPVertex(&(cube_vtx1[0]), 8, 0),
    gsSP1Triangle(0, 2, 1, 0),
    gsSP1Triangle(2, 0, 3, 0),
    gsSP1Triangle(4, 5, 6, 0),
    gsSP1Triangle(6, 7, 4, 0),

    gsSPVertex(&(cube_vtx2[0]), 8, 0),
    gsSP1Triangle(1, 6, 5, 0),
    gsSP1Triangle(1, 2, 6, 0),
    gsSP1Triangle(0, 5, 4, 0),
    gsSP1Triangle(0, 1, 5, 0),
    gsSP1Triangle(3, 4, 7, 0),
    gsSP1Triangle(3, 0, 4, 0),
    gsSP1Triangle(2, 3, 7, 0),
    gsSP1Triangle(2, 7, 6, 0),

    gsSPEndDisplayList(),
};

