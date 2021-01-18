/*====================================================================
 * gfxinit.c
 *
 * Synopsis:
 *
 * Graphics pipeline initialization.
 *
 * Copyright 1993, Silicon Graphics, Inc.
 * All Rights Reserved.
 *
 * This is UNPUBLISHED PROPRIETARY SOURCE CODE of Silicon Graphics,
 * Inc.; the contents of this file may not be disclosed to third
 * parties, copied or duplicated in any form, in whole or in part,
 * without the prior written permission of Silicon Graphics, Inc.
 *
 * RESTRICTED RIGHTS LEGEND:
 * Use, duplication or disclosure by the Government is subject to
 * restrictions as set forth in subdivision (c)(1)(ii) of the Rights
 * in Technical Data and Computer Software clause at DFARS
 * 252.227-7013, and/or in similar or successor clauses in the FAR,
 * DOD or NASA FAR Supplement. Unpublished - rights reserved under the
 * Copyright Laws of the United States.
 *====================================================================*/

#include <ultra64.h>
#include "gfx.h"

/*
 * Remember, viewport structures have 2 bits of fraction in them.
 */
static Vp vp = {
    SCREEN_WD*2, SCREEN_HT*2, G_MAXZ/2, 0,	/* scale */
    SCREEN_WD*2, SCREEN_HT*2, G_MAXZ/2, 0,	/* translate */
};

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

    /* bl */	gsDPSetRenderMode(G_RM_OPA_SURF, G_RM_OPA_SURF2),
		gsDPSetColorDither(G_CD_BAYER),

    gsSPEndDisplayList(),
};

/* intialize the RSP state: */
Gfx setup_rspstate[] = {
    gsSPViewport(&vp),
    gsSPClearGeometryMode((G_ZBUFFER | G_SHADE | G_SHADING_SMOOTH | 
			   G_CULL_BOTH | G_FOG | G_LIGHTING)),
    gsSPSetGeometryMode(G_ZBUFFER | G_SHADE | G_SHADING_SMOOTH | G_CULL_BACK),
    gsSPTexture(0, 0, 0, 0, G_OFF),
    gsSPEndDisplayList(),
};


