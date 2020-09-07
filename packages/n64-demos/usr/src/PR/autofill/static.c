
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

#include <ultra64.h>
#include "fill.h"
#include "static.h"

/* Remember, viewport structures have 2 bits of fraction in them */
static Vp vp = 
{
  SCREEN_WD*2, SCREEN_HT*2, G_MAXZ/2, 0,	/* scale */
  SCREEN_WD*2, SCREEN_HT*2, G_MAXZ/2, 0,	/* translate */
};

/* initialize the RSP state: */
Gfx rspinit_dl[] = 
{
  gsSPViewport(&vp),
  gsSPClearGeometryMode(G_SHADE | G_SHADING_SMOOTH | G_CULL_BOTH |
			G_FOG | G_LIGHTING | G_TEXTURE_GEN |
			G_TEXTURE_GEN_LINEAR | G_LOD | G_ZBUFFER),
  gsSPTexture(0, 0, 0, 0, G_OFF),
  gsSPEndDisplayList(),
};

/* initialize the RDP state: */
Gfx rdpinit_dl[] = 
{
  gsDPSetCycleType(G_CYC_1CYCLE),
  gsDPPipelineMode(G_PM_NPRIMITIVE),
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
  gsDPSetColorDither(G_CD_BAYER),
  gsDPSetDepthImage(zbuffer),
  gsDPPipeSync(),
  gsSPEndDisplayList(),
};

static Vtx background_vtx[] =  
{
  {       20,   20, -1500, 0, 0, 0,    0, 0xff,    0, 0xff	},
  {      300,   20, -1500, 0, 0, 0,    0,    0,    0, 0xff	},
  {      300,   20,    -5, 0, 0, 0,    0,    0, 0xff, 0xff	},
  {       20,   20,    -5, 0, 0, 0, 0xff,    0,    0, 0xff	},
};

Gfx background_dl[] = 
{
  /* Setup display modes antialiased in 1 cycle */
  gsDPPipeSync(),

  gsSPVertex(&(background_vtx[0]), 4, 0),
  gsSP1Triangle(2, 1, 0, 2),
  gsSP1Triangle(3, 2, 0, 3),

  gsSPEndDisplayList(),
};


