/*---------------------------------------------------------------------
	Copyright (C) 1997, Nintendo.
	
	File		static.c
	Coded    by	Yoshitaka Yasumoto.	Feb  6, 1997.
	Modified by	
	Comments	
	
	$Id: static.c,v 1.2 1997/08/07 03:24:22 yasu Exp $
  ---------------------------------------------------------------------*/
#include	<ultra64.h>
#include	"system.h"

Gfx rdpInit_dl[] = {
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
  gsSPEndDisplayList(),
};

Gfx clearCfb_dl[] = {
  gsDPPipeSync(),
  gsDPSetCycleType(G_CYC_FILL),
  gsDPSetRenderMode(G_RM_OPA_SURF, G_RM_OPA_SURF2),
  gsDPSetScissor(G_SC_NON_INTERLACE, 0, 0, SCREEN_WD, SCREEN_HT),
  gsDPSetFillColor(GPACK_RGBA5551(16,16,16,1) << 16 | 
		   GPACK_RGBA5551(16,16,16,1)),
  gsDPFillRectangle(0, 0, SCREEN_WD-1, SCREEN_HT-1),
  gsDPPipeSync(),
};

Gfx spriteInit_dl[] = {
  gsDPPipeSync(),
  gsDPSetTexturePersp(G_TP_NONE),
  gsDPSetTextureLOD(G_TL_TILE),
  gsDPSetTextureLUT(G_TT_NONE),
  gsDPSetTextureConvert(G_TC_FILT),
  gsDPSetAlphaCompare(G_AC_THRESHOLD),
  gsDPSetBlendColor(0, 0, 0, 0x01),
  gsDPSetCombineMode(G_CC_DECALRGBA, G_CC_DECALRGBA),
  gsSPEndDisplayList(),
};

/*======== End of static.c ========*/
