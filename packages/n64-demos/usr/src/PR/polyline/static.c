
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
#include "lines.h"
#include "static.h"

/* Remember, viewport structures have 2 bits of fraction in them */
static Vp vp = 
{
  SCREEN_WD*2, SCREEN_HT*2, G_MAXZ/2, 0,	/* scale */
  SCREEN_WD*2, SCREEN_HT*2, G_MAXZ/2, 0,	/* translate */
};

Vp smallvp = 
{
  SCREEN_WD,   SCREEN_HT,   G_MAXZ/2, 0,	/* scale */
  SCREEN_WD*2, SCREEN_HT*2, G_MAXZ/2, 0,	/* translate */
};


/* initialize the RSP state: */
Gfx rspinit_dl[] = 
{
  gsSPViewport(&smallvp),
  gsSPClearGeometryMode(G_SHADE | G_SHADING_SMOOTH | G_CULL_BOTH |
			G_FOG | G_LIGHTING | G_TEXTURE_GEN |
			G_TEXTURE_GEN_LINEAR | G_LOD ),
  gsSPTexture(0, 0, 0, 0, G_OFF),
  gsSPSetGeometryMode(G_SHADE | G_SHADING_SMOOTH | G_ZBUFFER),
  gsSPEndDisplayList(),
};

/* initialize the RDP state: */
Gfx rdpinit_dl[] = 
{
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
  gsDPSetColorDither(G_CD_DISABLE),
  gsDPSetDepthImage(zbuffer),
  gsDPPipeSync(),
  gsSPEndDisplayList(),
};

static Vtx background_vtx[] =  
{
  {        1,  238,    10, 0, 0, 0,    0, 0xff,    0, 0xff	},
  {      318,  238,    10, 0, 0, 0,    0,    0,    0, 0xff	},
  {      318,    1,    10, 0, 0, 0,    0,    0, 0xff, 0xff	},
  {        1,    1,    10, 0, 0, 0, 0xff,    0,    0, 0xff	},
};

Gfx background_dl[] = 
{
  /* Setup display modes antialiased in 1 cycle */
  gsDPPipeSync(),

  gsSPVertex(&(background_vtx[0]), 4, 0),
  gsSP1Triangle(0, 1, 2, 0),
  gsSP1Triangle(0, 2, 3, 0),

  gsSPEndDisplayList(),
};

static Vtx lborder[] =
{
  {   1,    1,  0,   0, 0, 0, 0xff, 0xff, 0xff, 0xff }, 
  { 318,    1,  0,   0, 0, 0, 0xff, 0xff, 0xff, 0xff }, 
  { 318,  238,  0,   0, 0, 0, 0xff, 0xff, 0xff, 0xff }, 
  {   1,  238,  0,   0, 0, 0, 0xff, 0xff, 0xff, 0xff },   

  {   2,    1,  0,   0, 0, 0, 0xff, 0xff, 0xff, 0xff }, 
  { 317,    1,  0,   0, 0, 0, 0xff, 0xff, 0xff, 0xff }, 
  { 317,  238,  0,   0, 0, 0, 0xff, 0xff, 0xff, 0xff }, 
  {   2,  238,  0,   0, 0, 0, 0xff, 0xff, 0xff, 0xff },   
};

Gfx border1[] =
{
  gsDPPipeSync(),  
  gsSPVertex(&lborder, 8, 0),

  gsSPLine3D(0, 1, 0), 
  gsSPLine3D(2, 3, 0), 

  gsSPLine3D(4, 7, 0), 
  gsSPLine3D(5, 6, 0), 

  gsSPEndDisplayList(),
};


#include "sphere.dat"

Gfx sphere[] = {
    gsDPPipeSync(),
#include "sphere.dl"
    gsSPEndDisplayList(),
};

