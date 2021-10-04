
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

/*---------------------------------------------------------------------*
        Copyright (C) 1997 Nintendo. (Originated by SGI)
        
        $RCSfile: static.c,v $
        $Revision: 1.6 $
        $Date: 1998/09/25 21:47:45 $
 *---------------------------------------------------------------------*/

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
#include "letters.h"

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


Lights2 litc2 = gdSPDefLights2(0x5, 0x5, 0x5,		/* ambient color */
			       100, 100, 0,		/* light color */
			       -32, -64, -32, 			/* normal */
			       50, 50, 0,		/* light color */
			       15, 30, 120); 			/* normal */


Gfx letters_setup_dl[] = {
    gsDPPipeSync(),
    gsDPSetCycleType(G_CYC_1CYCLE),
    gsSPSetGeometryMode(G_ZBUFFER | G_SHADE | G_SHADING_SMOOTH | 
			G_CULL_BACK | G_LIGHTING),
    gsSPSetLights2(litc2),
    gsDPSetCombineMode (G_CC_SHADE, G_CC_SHADE),
    gsDPSetRenderMode(G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2),
    gsSPEndDisplayList(),
};


/*  3-D Text */
#include "u64_vtx.h"
#include "metallic.h"
#include "smroad.h"

#define METALLIC_SIZE (sizeof(tex_metallic)/2) 
#define LOG2(x)		(((x)<=2)?1: \
			 ((x)<=4)?2: \
			 ((x)<=8)?3: \
			 ((x)<=16)?4: \
			 ((x)<=32)?5: \
			 ((x)<=64)?6: \
			 ((x)<=128)?7: \
			 ((x)<=256)?8: \
			 ((x)<=512)?9: \
			 ((x)<=1024)?10: \
			 11)


/*
 * The following two display lists are very similar. They set up
 * the texture for the "u64" letters, which is used to achieve
 * a "chrome" effect. Two different techniques are demonstrated;
 * a 1D texture and a 2D texture.
 *
 * The 1D texture is more compact, and excellent for objects with
 * very high curvature or intricate detail. The theory is, since
 * the reflections will be complex, a simple approximation will
 * suffice, so just fill a 1D texture map up with a smooth ramp
 * between many of the colors in the scene.
 *
 * The 2D texture map uses a "fisheye" projection of the environment
 * (actually, in this case the projection is a different, but similarly
 * colored environment). Generating texture coordinates that index
 * into this image produce a simple "enviromental mapping" effect.
 *
 */

Gfx u64_chrome1D_dl[] = {
    gsDPPipeSync(),
    gsDPSetCycleType(G_CYC_1CYCLE),
    gsSPClearGeometryMode(G_CULL_BOTH),
    gsSPSetGeometryMode(G_ZBUFFER | G_SHADE | G_SHADING_SMOOTH | 
			G_CULL_BACK | G_LIGHTING),
    gsSPSetGeometryMode(G_TEXTURE_GEN),
    gsDPSetCombineMode(G_CC_DECALRGB, G_CC_DECALRGB),
    gsDPSetRenderMode(G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2),
    gsDPSetColorDither(G_CD_BAYER),
    gsDPSetTexturePersp(G_TP_PERSP),
    gsDPSetTextureLOD(G_TL_TILE),
    gsDPSetTextureFilter(G_TF_BILERP),

    /* 1D chrome (linear random color map) */
    gsSPTexture((METALLIC_SIZE-1)<<6, 0x0, 0, G_TX_RENDERTILE, G_ON),
    gsDPLoadTextureBlock(tex_metallic, G_IM_FMT_RGBA, G_IM_SIZ_16b, METALLIC_SIZE, 1, 0,
		    G_TX_WRAP | G_TX_NOMIRROR, G_TX_WRAP | G_TX_NOMIRROR,
		    LOG2(METALLIC_SIZE), G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD),

    gsSPEndDisplayList()
};


Gfx u64_chrome2D_dl[] = {
    gsDPPipeSync(),
    gsDPSetCycleType(G_CYC_1CYCLE),
    gsSPClearGeometryMode(G_CULL_BOTH),
    gsSPSetGeometryMode(G_ZBUFFER | G_SHADE | G_SHADING_SMOOTH | 
			G_CULL_BACK | G_LIGHTING),
    gsSPSetGeometryMode(G_TEXTURE_GEN),
    gsDPSetCombineMode(G_CC_DECALRGB, G_CC_DECALRGB),
    gsDPSetRenderMode(G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2),
    gsDPSetColorDither(G_CD_BAYER),
    gsDPSetTexturePersp(G_TP_PERSP),
    gsDPSetTextureLOD(G_TL_TILE),
    gsDPSetTextureFilter(G_TF_BILERP),

    /* 2D chrome (environment map) */
    gsSPTexture(31<<6, 31<<6, 0, G_TX_RENDERTILE, G_ON),
    gsDPLoadTextureBlock(tex_smroad, G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 32, 0,
		    G_TX_WRAP | G_TX_NOMIRROR, G_TX_WRAP | G_TX_NOMIRROR,
		    5, 5, G_TX_NOLOD, G_TX_NOLOD),

    gsSPEndDisplayList()
};


/* data for the letters */
Gfx u64_geom_dl[] = {

#include "u64_tri.h"

    gsSPEndDisplayList()
};


Gfx setting_setup_dl[] = {

    gsDPPipeSync (),
    gsSPClearGeometryMode(G_SHADE | G_SHADING_SMOOTH | G_CULL_BOTH |
			  G_ZBUFFER | G_FOG | G_LIGHTING | G_TEXTURE_GEN |
			  G_TEXTURE_GEN_LINEAR | G_LOD ),
    gsSPSetGeometryMode(G_SHADING_SMOOTH | G_SHADE ),
    gsDPSetColorDither(G_CD_DISABLE),
    gsDPSetTexturePersp(G_TP_PERSP),
    gsDPSetTextureFilter(G_TF_BILERP),

    gsSPEndDisplayList(),
};

static Vtx v[] = {
	/* Ground */
	{	 -16, -16, 0, 0, 0<<6,   0<<6,   255, 255, 255, 255 },
	{	  16, -16, 0, 0, 0<<6,   31<<6, 255, 255, 255, 255 },
	{	  16,  16, 0, 0, 31<<6, 31<<6, 255, 255, 255, 255 },
	{	 -16,  16, 0, 0, 31<<6, 0<<6,   255, 255, 255, 255 },
};

#include "mahogany.h"
    
Gfx setting_geom_dl[] = {
	gsSPTexture(0x8000, 0x8000, 0, G_TX_RENDERTILE, G_ON),
	gsDPPipeSync(),
	gsDPSetRenderMode(G_RM_AA_OPA_SURF, G_RM_AA_OPA_SURF2),
	gsDPSetCombineMode(G_CC_MODULATERGBA, G_CC_MODULATERGBA),
	gsDPSetTextureFilter(G_TF_BILERP),
	gsDPLoadTextureBlock(mahogany, G_IM_FMT_RGBA, G_IM_SIZ_16b,
		32, 32, 0,
		G_TX_WRAP | G_TX_NOMIRROR, G_TX_WRAP | G_TX_NOMIRROR,
		5, 5, G_TX_NOLOD, G_TX_NOLOD),
	gsSPVertex(&v, 4, 0),
	gsSP1Triangle(0, 1, 2, 0),
	gsSP1Triangle(0, 2, 3, 0),
	gsSPTexture (0x8000, 0x8000, 0, G_TX_RENDERTILE, G_OFF),
	gsDPPipeSync(),
	gsDPSetCombineMode(G_CC_SHADE, G_CC_SHADE),
	gsSPEndDisplayList(),
};

