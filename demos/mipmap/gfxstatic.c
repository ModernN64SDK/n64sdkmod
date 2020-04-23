
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

/*
 * Cube
 */
static Vtx cube_vtx1[] = {
  { -50,  -50,  -50, 0, 00<<8, 00<<8, 0xff, 0x00, 0x00, 0xff},
  {  50,  -50,  -50, 0, 32<<8, 00<<8, 0x00, 0xff, 0x00, 0xff},
  {  50,   50,  -50, 0, 32<<8, 32<<8, 0x00, 0x00, 0xff, 0xff},
  { -50,   50,  -50, 0, 00<<8, 32<<8, 0xff, 0xff, 0xff, 0xff},
  { -50,  -50,   50, 0, 32<<8, 32<<8, 0xff, 0xff, 0x00, 0xff},
  {  50,  -50,   50, 0, 00<<8, 32<<8, 0x00, 0x00, 0x00, 0xff},
  {  50,   50,   50, 0, 00<<8, 00<<8, 0x00, 0xff, 0xff, 0xff},
  { -50,   50,   50, 0, 32<<8, 00<<8, 0xff, 0x00, 0xff, 0xff},

};
static Vtx cube_vtx2[] = {
  { -50,  -50,  -50, 0, 00<<8, 00<<8, 0xff, 0x00, 0x00, 0xff},
  {  50,  -50,  -50, 0, 00<<8, 32<<8, 0x00, 0xff, 0x00, 0xff},
  {  50,   50,  -50, 0, 00<<8, 00<<8, 0x00, 0x00, 0xff, 0xff},
  { -50,   50,  -50, 0, 00<<8, 32<<8, 0xff, 0xff, 0xff, 0xff},
  { -50,  -50,   50, 0, 32<<8, 00<<8, 0xff, 0xff, 0x00, 0xff},
  {  50,  -50,   50, 0, 32<<8, 32<<8, 0x00, 0x00, 0x00, 0xff},
  {  50,   50,   50, 0, 32<<8, 00<<8, 0x00, 0xff, 0xff, 0xff},
  { -50,   50,   50, 0, 32<<8, 32<<8, 0xff, 0x00, 0xff, 0xff},

};
/*
 * runway */
static Vtx runway_vtx[] = {
  { -100,  -100,  -400, 0, 00<<9, 00<<9, 0xff, 0x00, 0x00, 0xff},
  { -100,   200,  -400, 0, 00<<9, 32<<9, 0x00, 0xff, 0x00, 0xff},
  { -100,   200,   400, 0, 32<<9, 32<<9, 0x00, 0x00, 0xff, 0xff},
  { -100,  -100,   400, 0, 32<<9, 00<<9, 0xff, 0xff, 0xff, 0xff},
  {  100,  -100,  -400, 0, 00<<9, 32<<9, 0xff, 0x00, 0x00, 0xff},
  {  100,  -100,   400, 0, 32<<9, 32<<9, 0xff, 0xff, 0xff, 0xff},
};

Gfx cube_texture_setup_dl[] = {
    gsDPPipeSync(),

    gsDPSetCycleType(G_CYC_1CYCLE),
    gsSPSetGeometryMode(G_SHADE | G_SHADING_SMOOTH | G_ZBUFFER | G_CULL_BACK),
    gsDPSetRenderMode(G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2),
    gsDPSetCombineMode(G_CC_DECALRGBA, G_CC_DECALRGBA),
    gsDPSetTexturePersp(G_TP_PERSP),

    gsSPEndDisplayList(),
};

Gfx cube_textureMM_setup_dl[] = {
    gsDPPipeSync(),

    gsDPSetCycleType(G_CYC_1CYCLE),
    gsSPSetGeometryMode(G_SHADE | G_SHADING_SMOOTH | G_ZBUFFER | G_CULL_BACK),
    gsDPSetRenderMode(G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2),
    gsDPSetCombineMode(G_CC_DECALRGB, G_CC_DECALRGB),
    gsDPSetTexturePersp(G_TP_PERSP),
    gsDPSetTextureDetail (G_TD_CLAMP),
    gsDPSetTextureLOD(G_TL_LOD),

    gsSPEndDisplayList(),
};

Gfx cube_textureMM3_setup_dl[] = {
    gsDPPipeSync(),

    gsDPSetCycleType(G_CYC_2CYCLE),
    gsSPSetGeometryMode(G_SHADE | G_SHADING_SMOOTH | G_ZBUFFER | G_CULL_BACK),
    gsDPSetRenderMode(G_RM_PASS, G_RM_AA_ZB_OPA_SURF2),
    gsDPSetCombineMode(G_CC_TRILERP, G_CC_DECALRGB2),
    gsDPSetTexturePersp(G_TP_PERSP),
    gsDPSetTextureDetail (G_TD_CLAMP),
    gsDPSetTextureLOD(G_TL_LOD),

    gsSPEndDisplayList(),
};

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

Gfx load_tex_mymip_dl[] = {
    gsDPLoadTextureBlock(
		RGBA16mymip, G_IM_FMT_RGBA, G_IM_SIZ_16b,
		16, 16, 0,
		G_TX_WRAP, G_TX_WRAP,
		4, 4, G_TX_NOLOD, G_TX_NOLOD),
    gsSPEndDisplayList(),
};

Gfx border_dl[] = {
    gsDPPipeSync(),
    gsDPSetCycleType(G_CYC_FILL),
    gsDPSetRenderMode(G_RM_OPA_SURF, G_RM_OPA_SURF),
    gsDPSetFillColor(GPACK_RGBA5551(255,255,0,1) << 16 |
                     GPACK_RGBA5551(255,255,0,1)),
    gsDPFillRectangle(0, 119, SCREEN_WD-1, 122),
    gsDPFillRectangle(156, 0, 159, SCREEN_HT-1),

    gsDPPipeSync(),
    gsDPSetCycleType(G_CYC_1CYCLE),

    gsSPEndDisplayList(),
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

Gfx runway_dl[] = {
    gsSPVertex(&(runway_vtx[0]), 6, 0),
    gsSP1Triangle(0, 1, 2, 0),
    gsSP1Triangle(2, 3, 0, 0),
    gsSP1Triangle(0, 5, 4, 0),
    gsSP1Triangle(0, 3, 5, 0),
    gsSPEndDisplayList(),
};
