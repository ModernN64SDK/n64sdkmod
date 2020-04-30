
/**************************************************************************
 *                                                                        *
 *               Copyright (C) 1994, Silicon Graphics, Inc.               *
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
 * Create Date:	Tue Apr 11 17:45:53 PDT 1995
 *
 *
 * This file holds tiny display list segments that are 'static' data.
 *
 */

#include <ultra64.h>
#include "topgun.h"

/*
 * Remember, viewport structures have 2 bits of fraction in them.
 */
static Vp vp = {
	SCREEN_WD*2, SCREEN_HT*2, G_MAXZ/2, 0,	/* scale */
	SCREEN_WD*2, SCREEN_HT*2, G_MAXZ/2, 0,	/* translate */
};

Gfx rspinit_dl[] = {
    gsSPViewport(&vp),
    gsSPClearGeometryMode(0xffffffff),
    gsSPSetGeometryMode(G_SHADE | G_SHADING_SMOOTH),
    gsSPEndDisplayList(),
};

Gfx rdpinit_dl[] = {
    gsDPPipeSync(),
    gsDPSetScissor(G_SC_NON_INTERLACE, 0, 0, SCREEN_WD, SCREEN_HT),
    gsDPSetCombineMode(G_CC_SHADE, G_CC_SHADE),
    gsDPSetCycleType(G_CYC_1CYCLE),
    gsDPSetTextureLOD(G_TL_TILE),
    gsDPSetTextureLUT(G_TT_NONE),
    gsDPSetTextureDetail(G_TD_CLAMP),
    gsDPSetTexturePersp(G_TP_NONE),
    gsDPSetTextureFilter(G_TF_BILERP),
    gsDPSetTextureConvert(G_TC_FILT),
    gsDPSetCombineKey(G_CK_NONE),
    gsDPSetAlphaCompare(G_AC_NONE),
    gsDPSetRenderMode(G_RM_OPA_SURF, G_RM_OPA_SURF2),
    gsDPSetBlendMask(0xff),
    gsDPSetColorDither(G_CD_BAYER),
    gsDPPipeSync(),
    gsSPEndDisplayList(),
};

/*
 * Clear the Z buffer.  NOTE!!! that the mode is set to FILL, and the
 * color image is set to the Z buffer on return.  (assumes that following
 * code is going to change this when clearing the frame buffer.)
 */
Gfx clear_zbuffer[] = {

	/*
	 * clear z, z = max z, dz = 0
	 */
	gsDPSetCycleType(G_CYC_FILL),
	/* gsDPSetFillColor(GPACK_ZDZ(G_MAXFBZ, 0) << 16 | GPACK_ZDZ(G_MAXFBZ, 0)), */
	gsDPSetFillColor(0xff00ffff),
	gsDPFillRectangle(0, 0, SCREEN_WD-1, SCREEN_HT-1),
	gsSPEndDisplayList()

};

