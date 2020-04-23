
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
 * Create Date:	Tue Apr 11 17:45:53 PDT 1995
 *
 * This file holds display list segments that are 'static' data.
 *
 */

#include <ultra64.h>
#include "app.h"

/*
 * Remember, viewport structures have 2 bits of fraction in them.
 */
static Vp vp = {
	SCREEN_WD*2, SCREEN_HT*2, G_MAXZ/2, 0,	/* scale */
	SCREEN_WD*2, SCREEN_HT*2, G_MAXZ/2, 0,	/* translate */
};

/* initialize the RSP state: */
Gfx rspinit_dl[] = {
    gsSPViewport(&vp),
    gsSPClearGeometryMode(G_SHADE | G_SHADING_SMOOTH | G_CULL_BOTH |
			  G_FOG | G_LIGHTING | G_TEXTURE_GEN |
			  G_TEXTURE_GEN_LINEAR | G_LOD ),
    gsSPTexture(0, 0, 0, 0, G_OFF),
    gsSPSetGeometryMode(G_SHADE | G_SHADING_SMOOTH),
    gsSPEndDisplayList(),
};

/* initialize the RDP state: */
Gfx rdpinit_dl[] = {
    gsDPSetCycleType(G_CYC_1CYCLE),
    gsDPPipelineMode(G_PM_1PRIMITIVE),
    gsDPSetScissor(G_SC_NON_INTERLACE, 0, 0, SCREEN_WD, SCREEN_HT),
    gsDPSetTextureLOD(G_TL_TILE),
    gsDPSetTextureLUT(G_TT_NONE),
    gsDPSetTextureDetail(G_TD_CLAMP),
    gsDPSetTexturePersp(G_TP_PERSP),
    gsDPSetTextureFilter(G_TF_BILERP),
    gsDPSetTextureConvert(G_TC_FILT),
    gsDPSetCombineMode(G_CC_SHADE, G_CC_SHADE),
    gsDPSetCombineKey(G_CK_NONE),
    gsDPSetAlphaCompare(G_AC_NONE),
    gsDPSetRenderMode(G_RM_OPA_SURF, G_RM_OPA_SURF2),
    gsDPSetColorDither(G_CD_DISABLE),
    gsDPSetColorDither(G_CD_NOISE),
    gsDPSetColorDither(G_CD_BAYER),
    gsDPSetColorDither(G_CD_MAGICSQ),
    gsDPSetDepthImage(rsp_czb),
    gsDPPipeSync(),
    gsSPEndDisplayList(),
};


/* clear the color frame buffer: */

Gfx clearcfb_dl[] = {
    gsDPSetCycleType(G_CYC_FILL),
    gsDPSetColorImage(G_IM_FMT_RGBA, G_IM_SIZ_16b, SCREEN_WD, rsp_cfb),
    gsDPSetFillColor(GPACK_RGBA5551(64,64,64,1) << 16 | 
		     GPACK_RGBA5551(64,64,64,1)),
    gsDPFillRectangle(0, 0, SCREEN_WD-1, SCREEN_HT-1),
    gsDPPipeSync(),
    gsDPSetFillColor(GPACK_RGBA5551(64,64,255,1) << 16 | 
		     GPACK_RGBA5551(64,64,255,1)),
    gsDPFillRectangle(0, 0, SCREEN_WD-1, SCREEN_HT-1),
    gsSPEndDisplayList(),
};

Gfx clearczb_dl[] = {
    gsDPPipeSync(),
    gsDPSetCycleType(G_CYC_FILL),
    gsDPSetColorImage(G_IM_FMT_RGBA, G_IM_SIZ_16b, SCREEN_WD, rsp_czb),
        gsDPSetFillColor(GPACK_ZDZ(G_MAXFBZ, 0) << 16 | GPACK_ZDZ(G_MAXFBZ, 0)),
    gsDPFillRectangle(0, 0, SCREEN_WD-1, SCREEN_HT-1),
    gsSPEndDisplayList(),
};


static Vtx tex_vtx[] =  {
        { -64,  64, -5, 0, (  0 << 6), (  0 << 6), 0xff, 0xff, 0xff, 0xff},
        {  64,  64, -5, 0, (127 << 6), (  0 << 6), 0xff, 0xff, 0xff, 0xff},
        {  64, -64, -5, 0, (127 << 6), (127 << 6), 0xff, 0xff, 0xff, 0xff},
        { -64, -64, -5, 0, (  0 << 6), (127 << 6), 0xff, 0xff, 0xff, 0xff},
};

/* a 32x32 RGBA16 texture: */
#define gsDPLoadTextureBlockMM32(timg, fmt, siz, width, height, length,        \
                pal, cms, cmt, masks, maskt, shifts, shiftt)            \
        gsDPTileSync(),                                                 \
        gsDPSetTextureImage(fmt, siz, 1, timg),                         \
        gsDPSetTile(fmt, siz, 0, 0, G_TX_LOADTILE, 0 , cmt, maskt,    \
                shiftt, cms, masks, shifts),                            \
        gsDPLoadSync(),                                                 \
        gsDPLoadBlock(G_TX_LOADTILE, 0, 0, length-1,0x0),               \
        gsDPSetTile(fmt, siz, ((((width) * siz##_BYTES)+7)>>3), 0,    \
                1, pal, cmt, maskt, 0, cms, masks,                      \
                0),                                                     \
        gsDPSetTileSize(1, 0, 0,                                        \
                (32-1) << G_TEXTURE_IMAGE_FRAC,                         \
                (32-1) << G_TEXTURE_IMAGE_FRAC),                        \
        gsDPSetTile(fmt, siz, ((((16) * siz##_BYTES)+7)>>3), 256,       \
                2, pal, cmt, (maskt - 1), 1, cms, (masks -1),           \
                1),                                                     \
        gsDPSetTileSize(2, 0, 0,                                        \
                (16-1) << G_TEXTURE_IMAGE_FRAC,                         \
                (16-1) << G_TEXTURE_IMAGE_FRAC),                        \
        gsDPSetTile(fmt, siz, ((((8) * siz##_BYTES)+7)>>3), 320,        \
                3, pal, cmt, (maskt-2), 2, cms, (masks-2),              \
                2),                                                     \
        gsDPSetTileSize(3, 0, 0,                                        \
                (8-1) << G_TEXTURE_IMAGE_FRAC,                          \
                (8-1) << G_TEXTURE_IMAGE_FRAC),                         \
        gsDPSetTile(fmt, siz, ((((4) * siz##_BYTES)+7)>>3), 336,        \
                4, pal, cmt, (maskt-3), 3, cms, (masks-3),              \
                3),                                                     \
        gsDPSetTileSize(4, 0, 0,                                        \
                (4-1) << G_TEXTURE_IMAGE_FRAC,                          \
                (4-1) << G_TEXTURE_IMAGE_FRAC),                         \
        gsDPSetTile(fmt, siz, ((((2) * siz##_BYTES)+7)>>3), 340,        \
                5, pal, cmt, (maskt-4), 4, cms, (masks-4),              \
                4),                                                     \
        gsDPSetTileSize(5, 0, 0,                                        \
                (2-1) << G_TEXTURE_IMAGE_FRAC,                          \
                (2-1) << G_TEXTURE_IMAGE_FRAC),                         \
        gsDPSetTile(fmt, siz, ((((1) * siz##_BYTES)+7)>>3), 342,        \
                6, pal, cmt, (maskt-5), 5, cms, (masks-5),              \
                5),                                                     \
        gsDPSetTileSize(6, 0, 0,                                        \
                (1-1) << G_TEXTURE_IMAGE_FRAC,                          \
                (1-1) << G_TEXTURE_IMAGE_FRAC)

#include "RGBA16field32.h"
#include "RGBA16forest32.h"
#include "RGBA16drainage32.h"
#include "I8water32.h"
#include "RGBA16leaf32.h"
#include "RGBA16sand64x32.h"
#include "gorge_vtx.h"

/*
 * Load the projection Matrix and
 * multiply it by the Viewing matrix
 */
Gfx matrices_dl[] = {
    gsSPMatrix(OS_K0_TO_PHYSICAL(&(dynamic.projection)),
	       G_MTX_PROJECTION|G_MTX_LOAD|G_MTX_NOPUSH),
    gsSPMatrix(OS_K0_TO_PHYSICAL(&(dynamic.viewing)),
	       G_MTX_MODELVIEW|G_MTX_LOAD|G_MTX_NOPUSH),
    gsSPEndDisplayList(),
};


Gfx textri_dl[] = {
    gsSPClearGeometryMode(G_SHADE | G_SHADING_SMOOTH | G_ZBUFFER ),
    gsSPSetGeometryMode(G_SHADE | G_SHADING_SMOOTH | G_ZBUFFER | G_CULL_BACK ),
    gsDPPipeSync(),
    gsDPSetTextureFilter(G_TF_BILERP),
    gsDPSetTexturePersp(G_TP_PERSP),
    gsSPTexture(0xffff, 0xffff, 5, 1, G_ON),

    #include "gorge_dl.h"

    gsSPTexture(0, 0, 0, 0, G_OFF),
    gsDPSetCycleType(G_CYC_1CYCLE),
    gsSPEndDisplayList(),
};


Gfx river_dl[] =
{
    #include "river_dl.h"
    gsSPTexture(0, 0, 0, 0, G_OFF),
    gsDPSetCycleType(G_CYC_1CYCLE),
    gsSPEndDisplayList(),
};
