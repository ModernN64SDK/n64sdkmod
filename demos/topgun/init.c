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

#include <ultra64.h>
#include "topgun.h"

/*
 * Remember, viewport structures have 2 bits of fraction in them.
 */
static Vp vp = {
	SCREEN_WD*2, SCREEN_HT*2, G_MAXZ/2, 0,	/* scale */
	SCREEN_WD*2, SCREEN_HT*2, G_MAXZ/2, 0,	/* translate */
};

Gfx setup_rdpstate[MAX_STATIC_GFX_SIZE] = {

	/*
	 * Image pointers are translated via RSP segment registers.
	 * Preset segment register 0 with 0.
	 */

	gsSPSegment(0, 0),

	/*
	 * This RDP initialization sequence will setup the following modes
	 *
	 * 1 cycle mode, point sampled, non-zbuffer, smooth shaded opaque polygons
	 */


	gsDPPipeSync(),


	/*
	 * scissor must be larger than background clear rectangle
	 */

	/* dp */	gsDPSetScissor(G_SC_NON_INTERLACE, 0, 0, SCREEN_WD,
			 SCREEN_HT),
	/* dp */	gsDPSetCycleType(G_CYC_1CYCLE),

/* XXX must do KSEG0_TO_PHYSICAL(zbuffer), referencing zbuffer now will use segment number 2 */
	/* tx */	gsDPSetTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b, SCREEN_WD, zbuffer),
	/* tx */	gsDPSetTextureLOD(G_TL_TILE),
	/* tx */	gsDPSetTextureLUT(G_TT_NONE),
	/* tx */	gsDPSetTextureDetail(G_TD_CLAMP),
	/* tx */	gsDPSetTexturePersp(G_TP_NONE),
	/* tx */	gsDPSetTileSize(0, 0, 0, 0, 0), /* needed for verilog simulation */
	/* tx */	gsDPSetTileSize(1, 0, 0, 0, 0), /* needed for verilog simulation */
	/* tx */	gsDPSetTile(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0), /* needed for verilog simulation */
	/* tx */	gsDPSetTile(0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0), /* needed for verilog simulation */
	/* tf */	gsDPSetTextureFilter(G_TF_BILERP),
	/* tf */	gsDPSetTextureConvert(G_TC_FILT),
	/* cc */	gsDPSetCombineMode(G_CC_SHADE, G_CC_SHADE),
	/* cc */	gsDPSetCombineKey(G_CK_NONE),
	/* bl */	gsDPSetAlphaCompare(G_AC_NONE),
	/* bl */	gsDPSetDepthSource(G_ZS_PIXEL),
	/* bl */	gsDPSetRenderMode(G_RM_OPA_SURF, G_RM_OPA_SURF2),
	/* bl */	gsDPSetBlendMask(0xff),

	/* mi */	gsDPSetColorDither(G_CD_DISABLE),
/* XXX must do KSEG0_TO_PHYSICAL(zbuffer), referencing zbuffer now will use segment number 2 */
	/* mi */	gsDPSetDepthImage(zbuffer),
			gsSPEndDisplayList(),
};

Gfx setup_rspstate[MAX_STATIC_GFX_SIZE] = {
/* XXX Initialize the matrix stack after we change host init DMEM */
	gsSPViewport(&vp),
	gsSPSetGeometryMode(G_ZBUFFER | G_SHADE | G_SHADING_SMOOTH | G_CULL_BACK),
	gsSPEndDisplayList(),
};

Gfx clear_fb[MAX_STATIC_GFX_SIZE] = {

	gsDPPipeSync(),
	gsDPSetCycleType(G_CYC_FILL),

	/*
	 * clear color, cvg = FULL or 1
	 */

	gsDPSetColorImage(G_IM_FMT_RGBA, G_IM_SIZ_16b, SCREEN_WD, cfb_16_a),
	gsDPSetFillColor(GPACK_RGBA5551(100,100,255,1) << 16 | GPACK_RGBA5551(100,100,255,1)),
#ifdef REALCLEAR
	gsDPFillRectangle(0, 0, SCREEN_WD-1, SCREEN_HT-1),
	gsDPNoOp(), 
#else
	gsDPSetColorImage(5, G_IM_SIZ_16b, SCREEN_WD, cfb_16_a),
	gsDPSetColorImage(G_IM_FMT_RGBA, G_IM_SIZ_16b, SCREEN_WD, cfb_16_a),
#endif

	/*
	 * clear z, z = max z, dz = 0
	 */

	gsDPPipeSync(),
/* XXX must do KSEG0_TO_PHYSICAL(zbuffer), referencing zbuffer now will use segment number 2 */
	gsDPSetColorImage(G_IM_FMT_RGBA, G_IM_SIZ_16b, SCREEN_WD, zbuffer),
	gsDPSetFillColor(GPACK_ZDZ(G_MAXFBZ, 0) << 16 | GPACK_ZDZ(G_MAXFBZ, 0)),
#ifdef REALCLEAR
	gsDPFillRectangle(0, 0, SCREEN_WD-1, SCREEN_HT-1),
	gsDPNoOp(), 
#else
	gsDPSetColorImage(5, G_IM_SIZ_16b, SCREEN_WD, zbuffer),
	gsDPSetColorImage(G_IM_FMT_RGBA, G_IM_SIZ_16b, SCREEN_WD, zbuffer),
#endif

	gsDPPipeSync(),
	gsDPSetColorImage(G_IM_FMT_RGBA, G_IM_SIZ_16b, SCREEN_WD, cfb_16_a),
	gsDPSetCycleType(G_CYC_1CYCLE),
	gsSPEndDisplayList(),
};
