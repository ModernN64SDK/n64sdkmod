/*
  static.c
    Copyright (C) 1999, NINTENDO Co,Ltd., MONEGI CORPORATION.
    Jan. 28, 1999.
*/

#include <ultra64.h>
#include "FrameBuffer.h"

#ifdef	USE_CFB32
extern u32 rsp_cfb[];
#else	/* USE_CFB32 */
extern u16 rsp_cfb[];
#endif	/* USE_CFB32 */

/* initialize the RSP state: */
Gfx rspinit_dl[] = {
#ifdef	USE_CFB32
    gsDPSetColorImage(G_IM_FMT_RGBA, G_IM_SIZ_32b, SCREEN_WD, rsp_cfb),
#else	/* USE_CFB32 */
    gsDPSetColorImage(G_IM_FMT_RGBA, G_IM_SIZ_16b, SCREEN_WD, rsp_cfb),
#endif	/* USE_CFB32 */
	gsSPEndDisplayList(),
};

/* initialize the RDP state: */
Gfx rdpinit_dl[] = {
    gsDPPipeSync(),
    gsDPPipelineMode(G_PM_1PRIMITIVE),
    gsDPSetTextureLOD(G_TL_TILE),
    gsDPSetTextureLUT(G_TT_NONE),
    gsDPSetTextureDetail(G_TD_CLAMP),
    gsDPSetTexturePersp(G_TP_NONE),
    gsDPSetTextureFilter(G_TF_BILERP),
    gsDPSetTextureConvert(G_TC_FILT),
    gsDPSetCombineMode(G_CC_SHADE, G_CC_SHADE),
    gsDPSetCombineKey(G_CK_NONE),
    gsDPSetAlphaCompare(G_AC_NONE),
    gsDPSetColorDither(G_CD_DISABLE),
    gsDPSetScissor(G_SC_NON_INTERLACE, 0, 0,320, 240),
	gsSPEndDisplayList(),
};


/* clear the color frame buffer: */

Gfx clearcfb_dl[] = {
    gsDPSetCycleType(G_CYC_FILL),
    gsDPSetFillColor(GPACK_RGBA5551(255,255,255,1) << 16 | 
		     GPACK_RGBA5551(255,255,255,1)),
    gsDPFillRectangle(0, 0, SCREEN_WD-1, SCREEN_HT-1),
    gsDPPipeSync(),
    gsDPSetCycleType(G_CYC_1CYCLE),
    gsSPEndDisplayList(),
};

