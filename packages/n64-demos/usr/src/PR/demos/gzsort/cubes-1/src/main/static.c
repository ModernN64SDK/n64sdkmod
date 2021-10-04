/*---------------------------------------------------------------------*
	Copyright (C) 1997, Nintendo.
	
	File		static.c
	Coded    by	Yoshitaka Yasumoto.	Sep 12, 1997.
	Modified by	
	
	$Id: static.c,v 1.3 1997/10/13 08:59:13 yasu Exp $
 *---------------------------------------------------------------------*/
#include	<ultra64.h>
#include	<PR/gzsort.h>
#include	"system.h"

#define	OTHERMODE_A(cyc)	(G_CYC_##cyc|G_PM_1PRIMITIVE|G_TP_PERSP|\
				 G_TD_CLAMP|G_TL_TILE|G_TT_NONE|G_TF_BILERP|\
				 G_TC_FILT|G_CK_NONE|G_CD_DISABLE|G_AD_DISABLE)
#define	OTHERMODE_B(rm1,rm2)	(G_AC_NONE|G_ZS_PRIM|G_RM_##rm1|G_RM_##rm2)

/*------ Clear Screen ------*/
Gfx	clearCfb[] = {
  gsDPSetColorImage(G_IM_FMT_RGBA, G_IM_SIZ_16b, SCREEN_WD, CFB_SEGMENT<<24),
  gsDPSetOtherMode(OTHERMODE_A(FILL), OTHERMODE_B(NOOP,NOOP2)),
  gsDPSetScissor(G_SC_NON_INTERLACE, 0, 0, SCREEN_WD, SCREEN_HT),
  gsDPSetCombineMode(G_CC_SHADE, G_CC_SHADE),
  gsDPSetFillColor(GPACK_RGBA5551(64,64,255,1) << 16 | 
		   GPACK_RGBA5551(64,64,255,1)),
  gsDPFillRectangle(0, 0, SCREEN_WD-1, SCREEN_HT-1),
  gsSPEndDisplayList(),
};

/*------ Finish Gfx ------*/
Gfx	gfxFinish[] = {
  gsDPPipeSync(),
  gsDPSetOtherMode(OTHERMODE_A(FILL), OTHERMODE_B(NOOP,NOOP2)),
  gsDPSetFillColor(GPACK_RGBA5551(128,255,128,1) << 16 | 
		   GPACK_RGBA5551(128,255,128,1)),
  gsDPFillRectangle(30, 201, 30, 202),		// gfxFinish[3]
  gsDPFillRectangle(30, 201, 30, 202),		// gfxFinish[4]
  gsDPPipeSync(),
  gsDPSetFillColor(GPACK_RGBA5551(128,128,255,1) << 16 | 
		   GPACK_RGBA5551(128,128,255,1)),
  gsDPFillRectangle(30, 204, 30, 205),		// gfxFinish[7]
  gsDPFillRectangle(30, 204, 30, 205),		// gfxFinish[8]
  gsDPPipeSync(),
  gsDPSetFillColor(GPACK_RGBA5551(255,128,128,1) << 16 | 
		   GPACK_RGBA5551(255,128,128,1)),
  gsDPFillRectangle(30, 206, 30, 207),		// gfxFinish[11]
  gsDPPipeSync(),  
  gsDPSetFillColor(GPACK_RGBA5551(255,255,255,1) << 16 | 
		   GPACK_RGBA5551(255,255,255,1)),
  gsDPFillRectangle(30+  0, 200, 30+  0, 209),
  gsDPFillRectangle(30+167, 200, 30+167, 209),
  gsDPFullSync(),
  gsSPEndDisplayList(),
};

/*======== End of static.c ========*/
