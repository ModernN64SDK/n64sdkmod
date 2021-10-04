/*---------------------------------------------------------------------*
	Copyright (C) 1997, Nintendo.
	
	File		static.c
	Coded    by	Yoshitaka Yasumoto.	Sep 12, 1997.
	Modified by	
	
	$Id: static.c,v 1.1 1997/10/13 08:59:25 yasu Exp $
 *---------------------------------------------------------------------*/
#include	<ultra64.h>
#include	<PR/gzsort.h>
#include	"system.h"

/*--- View port parameter ---*/
Vp      viewport = {
  SCREEN_WD*2, -SCREEN_HT*2, G_MAXZ/2, 0,
  SCREEN_WD*2,  SCREEN_HT*2, G_MAXZ/2, 0,
};

/*--- Macro to set render mode ---*/
#define	OTHERMODE_A(cyc)	(G_CYC_##cyc|G_PM_1PRIMITIVE|G_TP_PERSP|\
				 G_TD_CLAMP|G_TL_TILE|G_TT_NONE|G_TF_BILERP|\
				 G_TC_FILT|G_CK_NONE|G_CD_DISABLE|G_AD_DISABLE)
#define	OTHERMODE_B(rm1,rm2)	(G_AC_NONE|G_ZS_PRIM|G_RM_##rm1|G_RM_##rm2)

/*------ Clear up screen ------*/
Gfx	clearCfb[] = {
  gsDPSetColorImage(G_IM_FMT_RGBA, G_IM_SIZ_16b, SCREEN_WD, CFB_SEGMENT<<24),
  gsDPSetOtherMode(OTHERMODE_A(FILL), OTHERMODE_B(NOOP, NOOP2)),
  gsDPSetScissor(G_SC_NON_INTERLACE, 0, 0, SCREEN_WD, SCREEN_HT),
  gsDPSetCombineMode(G_CC_SHADE, G_CC_SHADE),
  gsDPSetFillColor(GPACK_RGBA5551(64,64,255,1) << 16 | 
		   GPACK_RGBA5551(64,64,255,1)),
  gsDPFillRectangle(0, 0, SCREEN_WD-1, SCREEN_HT-1),
  gsSPEndDisplayList(),
};

/*------ Finish and sync ------*/
Gfx	gfxFinish[] = {
  gsDPPipeSync(),
  gsDPFullSync(),
  gsSPEndDisplayList(),
};

/*--- RDPCMD to switch into Shade Triangle Mode ---*/
Gfx     rdpcmdShPoly[] = {
  gsDPPipeSync(),
  gsDPSetOtherMode(OTHERMODE_A(1CYCLE), OTHERMODE_B(OPA_SURF,OPA_SURF2)),
  gsDPSetCombineMode(G_CC_SHADE, G_CC_SHADE),
  gsSPEndDisplayList(),
};

/*------ Quadrangle ------*/
zVtxSrc	quadVtxSrc[] = {	// Must be aligned 8.
  /* X     Y    Z  */
  {  32,  32,   0  },		/* Vtx 0 */
  { -32,  32,   0  },		/* Vtx 1 */
  {  32, -32,   0  },		/* Vtx 2 */
  { -32, -32,   0  },		/* Vtx 3 */
};

/*--- Cube vertices colors ---*/
zColor  quadColor[] = {
{ 255,   0,   0, 255 },		/* Vtx 0 */
{   0, 255,   0, 255 },		/* Vtx 1 */
{   0,   0, 255, 255 },		/* Vtx 2 */
{   0, 255, 255, 255 },		/* Vtx 3 */
};

/*======== End of static.c ========*/
