
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
#include "bumpmap.h"

/* Z-buffer.  Defined in zbuf.c */
extern unsigned short zbuffer[];

extern unsigned short obj0_tlut_real[];


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
			  G_TEXTURE_GEN_LINEAR | G_LOD),
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
    gsDPSetColorDither(G_CD_ENABLE),
    gsDPPipeSync(),
    gsSPEndDisplayList(),
};


/* clear the color frame buffer: */

Gfx clearcfb_dl[] = {
    gsDPSetCycleType(G_CYC_FILL),
    gsDPSetRenderMode(G_RM_OPA_SURF, G_RM_OPA_SURF2),

      gsDPSetColorImage(G_IM_FMT_RGBA, G_IM_SIZ_16b, SCREEN_WD,
			OS_K0_TO_PHYSICAL(zbuffer)),
      gsDPSetFillColor(GPACK_ZDZ(G_MAXFBZ, 0) << 16 | GPACK_ZDZ(G_MAXFBZ, 0)),
      gsDPFillRectangle(0, 0, SCREEN_WD-1, SCREEN_HT-1),
      gsDPPipeSync(),


    gsDPSetColorImage(G_IM_FMT_RGBA, G_IM_SIZ_16b, SCREEN_WD, rsp_cfb),
    gsDPSetFillColor(GPACK_RGBA5551(0,0,0,1) << 16 | 
		     GPACK_RGBA5551(0,0,0,1)),
    gsDPFillRectangle(0, 0, SCREEN_WD-1, SCREEN_HT-1),
    gsDPPipeSync(),
    gsDPSetDepthImage( OS_K0_TO_PHYSICAL(zbuffer) ),
    gsDPPipeSync(),
    gsSPEndDisplayList(),
};

/* 
 * This group of vertices, and accompanying display list, demonstrate
 * simple textured triangles.
 *
 * The texture coordinates are texel-space coordinates that
 * are shifted up 6 bits; 5 bits because the texture coordinates
 * are S10.5 format, and 1 more bit so we can use a texture scaling
 * of 0.5, and get exact coordinates.
 *
 * The coordinates larger than the texture resolution, along with the
 * proper mode settings, cause the texture to repeat on itself.
 */

#include "obj_dls.h"

static Vtx tex_vtx[] =  {
        { -64,  64, -5, 0, (  0 << 6), (  0 << 6), 0xff, 0xff, 0xff, 0xff},
        {  64,  64, -5, 0, (127 << 6), (  0 << 6), 0xff, 0xff, 0xff, 0xff},
        {  64, -64, -5, 0, (127 << 6), (127 << 6), 0xff, 0xff, 0xff, 0xff},
        { -64, -64, -5, 0, (  0 << 6), (127 << 6), 0xff, 0xff, 0xff, 0xff},
};

Gfx textri_dl[] = {

    gsDPPipeSync(),
    gsDPSetCycleType(G_CYC_1CYCLE),

#define DO_ZBUF

#ifdef DO_ZBUF
    gsDPSetRenderMode(G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2),
    gsSPSetGeometryMode(G_ZBUFFER | G_SHADE | G_SHADING_SMOOTH),
#else
    gsDPSetRenderMode(G_RM_AA_OPA_SURF, G_RM_AA_OPA_SURF2),
    gsSPClearGeometryMode(G_SHADE | G_SHADING_SMOOTH),
#endif

    /*
     * The texture scaling parameters are .16 format. In order to
     * get an exact texture scale of 1.0, we scale above by 2.0, and
     * scale down here by 0.5
     */
    gsSPTexture(0x8000, 0x8000, 0, G_TX_RENDERTILE, G_ON),
    gsDPSetCombineMode(G_CC_DECALRGB, G_CC_DECALRGB),
    gsDPSetCombineMode(G_CC_MODULATERGBDECALA, G_CC_MODULATERGBDECALA),
    gsDPSetTextureLOD(G_TL_TILE),
    gsDPPipeSync(),
#define DYNAMIC_LIGHTING
#ifdef DYNAMIC_LIGHTING
    gsDPLoadTLUT_pal256( OS_K0_TO_PHYSICAL(obj0_tlut_real) ),
#else
    gsDPLoadTLUT_pal256( obj0_tlut ),
#endif
    gsDPPipeSync(),
    gsDPSetTextureLUT( G_TT_RGBA16 ),
    gsDPSetTextureFilter(G_TF_BILERP),

    gsSPDisplayList( obj0_dl ),

    gsDPSetTextureLUT( G_TT_NONE ),
    gsSPTexture(0, 0, 0, 0, G_OFF), /* done; turn texture off */
    gsSPEndDisplayList(),
};

