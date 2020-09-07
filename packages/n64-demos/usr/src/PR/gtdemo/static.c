
/*
 * Copyright 1995, Silicon Graphics, Inc.
 * ALL RIGHTS RESERVED
 *
 * UNPUBLISHED -- Rights reserved under the copyright laws of the United
 * States.   Use of a copyright notice is precautionary only and does not
 * imply publication or disclosure.
 *
 * U.S. GOVERNMENT RESTRICTED RIGHTS LEGEND:
 * Use, duplication or disclosure by the Government is subject to restrictions
 * as set forth in FAR 52.227.19(c)(2) or subparagraph (c)(1)(ii) of the Rights
 * in Technical Data and Computer Software clause at DFARS 252.227-7013 and/or
 * in similar or successor clauses in the FAR, or the DOD or NASA FAR
 * Supplement.  Contractor/manufacturer is Silicon Graphics, Inc.,
 * 2011 N. Shoreline Blvd. Mountain View, CA 94039-7311.
 *
 * THE CONTENT OF THIS WORK CONTAINS CONFIDENTIAL AND PROPRIETARY
 * INFORMATION OF SILICON GRAPHICS, INC. ANY DUPLICATION, MODIFICATION,
 * DISTRIBUTION, OR DISCLOSURE IN ANY FORM, IN WHOLE, OR IN PART, IS STRICTLY
 * PROHIBITED WITHOUT THE PRIOR EXPRESS WRITTEN PERMISSION OF SILICON
 * GRAPHICS, INC.
 *
 */

/*
 * File:	static.c
 * Creator:	hsa@sgi.com
 * Create Date:	Tue Oct 31 14:15:44 PST 1995
 *
 * This file holds tiny display list segments that are 'static' data.
 *
 */

#include <ultra64.h>
#include "gtdemo.h"

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
			  G_TEXTURE_GEN_LINEAR | G_LOD | G_ZBUFFER),
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
    gsDPSetColorDither(G_CD_BAYER),
    gsDPSetCycleType(G_CYC_1CYCLE),
    gsDPPipeSync(),
    gsSPEndDisplayList(),
};

#include "../Texture/RGBA16checkerMM.h"

Gfx floor_setup_dl[] = {
    gsDPPipeSync (),
    gsSPClearGeometryMode(G_SHADE | G_SHADING_SMOOTH | G_CULL_BOTH |
			  G_FOG | G_LIGHTING | G_TEXTURE_GEN |
			  G_TEXTURE_GEN_LINEAR | G_LOD),

    gsSPSetGeometryMode(G_ZBUFFER),

    gsDPSetTexturePersp(G_TP_PERSP),
    gsDPSetTextureFilter(G_TF_BILERP),

    gsDPSetRenderMode(G_RM_PASS, G_RM_OPA_SURF2),

    gsSPTexture(0x8000, 0x8000, 5, G_TX_RENDERTILE, G_ON),
    gsDPSetCombineMode(G_CC_TRILERP, G_CC_DECALRGB2),
    gsDPSetTextureDetail (G_TD_CLAMP),

    /* set up all the tiles */
    gsSPDisplayList(RGBA16checkerMM_dl),

    gsDPSetTextureLOD(G_TL_LOD),
    gsDPPipelineMode(G_PM_NPRIMITIVE),
    gsDPSetCycleType(G_CYC_2CYCLE),
    gsSPEndDisplayList(),
};

#define TXT_SHFT	6
static Vtx floor_vtx[25] =  {
	{ -64, 0, -64, 0, (0 << TXT_SHFT), (0 << TXT_SHFT), 0, 0, 0, 0xff },
	{ -32, 0, -64, 0, (31 << TXT_SHFT), (0 << TXT_SHFT), 0, 0, 0, 0xff },
	{ 0, 0, -64, 0, (63 << TXT_SHFT), (0 << TXT_SHFT), 0, 0, 0, 0xff },
	{ 32, 0, -64, 0, (95 << TXT_SHFT), (0 << TXT_SHFT), 0, 0, 0, 0xff },
	{ 64, 0, -64, 0, (127 << TXT_SHFT), (0 << TXT_SHFT), 0, 0, 0, 0xff },
	{ -64, 0, -32, 0, (0 << TXT_SHFT), (31 << TXT_SHFT), 0, 0, 0, 0xff },
	{ -32, 0, -32, 0, (31 << TXT_SHFT), (31 << TXT_SHFT), 0, 0, 0, 0xff },
	{ 0, 0, -32, 0, (63 << TXT_SHFT), (31 << TXT_SHFT), 0, 0, 0, 0xff },
	{ 32, 0, -32, 0, (95 << TXT_SHFT), (31 << TXT_SHFT), 0, 0, 0, 0xff },
	{ 64, 0, -32, 0, (127 << TXT_SHFT), (31 << TXT_SHFT), 0, 0, 0, 0xff },
	{ -64, 0, 0, 0, (0 << TXT_SHFT), (63 << TXT_SHFT), 0, 0, 0, 0xff },
	{ -32, 0, 0, 0, (31 << TXT_SHFT), (63 << TXT_SHFT), 0, 0, 0, 0xff },
	{ 0, 0, 0, 0, (63 << TXT_SHFT), (63 << TXT_SHFT), 0, 0, 0, 0xff },
	{ 32, 0, 0, 0, (95 << TXT_SHFT), (63 << TXT_SHFT), 0, 0, 0, 0xff },
	{ 64, 0, 0, 0, (127 << TXT_SHFT), (63 << TXT_SHFT), 0, 0, 0, 0xff },
	{ -64, 0, 32, 0, (0 << TXT_SHFT), (95 << TXT_SHFT), 0, 0, 0, 0xff },
	{ -32, 0, 32, 0, (31 << TXT_SHFT), (95 << TXT_SHFT), 0, 0, 0, 0xff },
	{ 0, 0, 32, 0, (63 << TXT_SHFT), (95 << TXT_SHFT), 0, 0, 0, 0xff },
	{ 32, 0, 32, 0, (95 << TXT_SHFT), (95 << TXT_SHFT), 0, 0, 0, 0xff },
	{ 64, 0, 32, 0, (127 << TXT_SHFT), (95 << TXT_SHFT), 0, 0, 0, 0xff },
	{ -64, 0, 64, 0, (0 << TXT_SHFT), (127 << TXT_SHFT), 0, 0, 0, 0xff },
	{ -32, 0, 64, 0, (31 << TXT_SHFT), (127 << TXT_SHFT), 0, 0, 0, 0xff },
	{ 0, 0, 64, 0, (63 << TXT_SHFT), (127 << TXT_SHFT), 0, 0, 0, 0xff },
	{ 32, 0, 64, 0, (95 << TXT_SHFT), (127 << TXT_SHFT), 0, 0, 0, 0xff },
	{ 64, 0, 64, 0, (127 << TXT_SHFT), (127 << TXT_SHFT), 0, 0, 0, 0xff },
};
#undef TXT_SHFT

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


/* TURBO DATA */

/* dummy object to send global state: */
gtState	dpGlobalObj =
{
    0x0,	/* renderState */
    0x0,	/* textureState */
    0,		/* vtxCount */
    0,		/* vtxV0 */
    0,		/* triCount */
    0x0,	/* pad1 */
    NULL,	/* RDPCmds */
    gsDPClearOtherMode(),				/* rdpOthermode */
    {	/* integer portion: */
	0x00010000, 0x00000000,			/* transform */
	0x00000001, 0x00000000,
	0x00000000, 0x00010000,
	0x00000000, 0x00000001,
	/* fractional portion: */
	0x00000000, 0x00000000,
	0x00000000, 0x00000000,
	0x00000000, 0x00000000,
	0x00000000, 0x00000000,
    },
};

/* this finishes the DP with a sync: */
static Gfx	finalDPCmds[] =
{
	gsDPFullSync(),
	gsDPEndDisplayList(),
};
gtState	dpFinalObj =
{
    0x0,	/* renderState */
    0x0,	/* textureState */
    0,		/* vtxCount */
    0,		/* vtxV0 */
    0,		/* triCount */
    0x0,	/* pad1 */
    (Gfx *)finalDPCmds,	/* RDPCmds */
    gsDPClearOtherMode(),				/* rdpOthermode */
    {	/* integer portion: */
	0x00010000, 0x00000000,			/* transform */
	0x00000001, 0x00000000,
	0x00000000, 0x00010000,
	0x00000000, 0x00000001,
	/* fractional portion: */
	0x00000000, 0x00000000,
	0x00000000, 0x00000000,
	0x00000000, 0x00000000,
	0x00000000, 0x00000000,
    }
};






