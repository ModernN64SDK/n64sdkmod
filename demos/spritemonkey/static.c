
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
 * Create Date:	Thu Jun 22 09:28:01 PDT 1995
 *
 * This file holds display list segments that are 'static' data.
 *
 */

#include <ultra64.h>
#include "sprite.h"
#include "static.h"

/* initialize the RSP state: */
Gfx rspinit_dl[] = 
{
  gsSPClearGeometryMode(G_SHADE | G_SHADING_SMOOTH | G_CULL_BOTH |
			G_FOG | G_LIGHTING | G_TEXTURE_GEN |
			G_TEXTURE_GEN_LINEAR | G_LOD ),
  gsSPTexture(0, 0, 0, 0, G_OFF),
  gsSPSetGeometryMode(G_SHADE | G_SHADING_SMOOTH),
  gsSPEndDisplayList(),
};

/* initialize the RDP state: */

Gfx rdpinit_dl[] = 
{
  gsDPSetCycleType(G_CYC_1CYCLE),    
  gsDPPipelineMode(G_PM_NPRIMITIVE), 
  gsDPSetTextureLOD(G_TL_TILE),
  gsDPSetTextureLUT(G_TT_NONE),
  gsDPSetTextureDetail(G_TD_CLAMP),
  gsDPSetTexturePersp(G_TP_NONE),
  gsDPSetTextureFilter(G_TF_BILERP),
  gsDPSetTextureConvert(G_TC_FILT),
  gsDPSetCombineMode(G_CC_DECALRGB, G_CC_DECALRGB),
  gsDPSetCombineKey(G_CK_NONE),
  gsDPSetAlphaCompare(G_AC_NONE),
  gsDPSetRenderMode(G_RM_OPA_SURF, G_RM_OPA_SURF2),
  gsDPSetColorDither(G_CD_DISABLE),
  gsDPPipeSync(),
  gsSPEndDisplayList(),
};

#include "dk.h"
#include "dk7.h"

