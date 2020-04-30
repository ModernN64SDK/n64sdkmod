
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

static Vtx v[] = {
	/* Ground */
	{	-2048, 0,   -2048, 0, 0<<9,   0<<9,   255, 255, 255, 255 },
	{	 2047, 0,   -2048, 0, 0<<9,   31<<9, 255, 255, 255, 255 },
	{	 2047, 0,    2047, 0, 31<<9, 31<<9, 255, 255, 255, 255 },
	{	-2048, 0,    2047, 0, 31<<9, 0<<9,   255, 255, 255, 255 },
	/* First (bottom) layer of clouds */
	{	-2048, 28, -2048, 0, 0<<9,   0<<9,   255, 255, 255, 255 },
	{	 2047, 28, -2048, 0, 0<<9,   31<<9, 255, 255, 255, 255 },
	{	 2047, 28,  2047, 0, 31<<9, 31<<9, 255, 255, 255, 255 },
	{	-2048, 28,  2047, 0, 31<<9, 0<<9,   255, 255, 255, 255 },
	/* Second (middle) layer of clouds */
	{	-2048, 44, -2048, 0, 31<<9, 31<<9, 255, 255, 255, 255 },
	{	 2047, 44, -2048, 0, 31<<9, 0<<9,   255, 255, 255, 255 },
	{	 2047, 44,  2047, 0, 0<<9,   0<<9,   255, 255, 255, 255 },
	{	-2048, 44,  2047, 0, 0<<9,   31<<9, 255, 255, 255, 255 },
	/* Third (top) layer of clouds */
	{	-2048, 56, -2048, 0, 0<<9,   0<<9,   255, 255, 255, 255 },
	{	 2047, 56, -2048, 0, 31<<9, 0<<9,   255, 255, 255, 255 },
	{	 2047, 56,  2047, 0, 31<<9, 31<<9, 255, 255, 255, 255 },
	{	-2048, 56,  2047, 0, 0<<9,   31<<9, 255, 255, 255, 255 },
};

#include "ground.h"
#include "sky.h"
#include "tree.h"

Gfx ground[] = {
	gsSPTexture(0x8000, 0x8000, 0, G_TX_RENDERTILE, G_ON),
	gsDPPipeSync(),
	gsDPSetRenderMode(G_RM_AA_OPA_SURF, G_RM_AA_OPA_SURF2),
	gsDPSetCombineMode(G_CC_MODULATERGBA, G_CC_MODULATERGBA),
	gsDPSetTextureFilter(G_TF_BILERP),
	gsDPLoadTextureBlock(tex_ground, G_IM_FMT_RGBA, G_IM_SIZ_16b,
		32, 32, 0,
		G_TX_WRAP | G_TX_NOMIRROR, G_TX_WRAP | G_TX_NOMIRROR,
		4, 4, G_TX_NOLOD, G_TX_NOLOD),
	gsSPVertex(&v, 4, 0),
	gsSP1Triangle(0, 1, 2, 0),
	gsSP1Triangle(0, 2, 3, 0),
	gsSPTexture (0x8000, 0x8000, 0, G_TX_RENDERTILE, G_OFF),
	gsDPPipeSync(),
	gsDPSetCombineMode(G_CC_SHADE, G_CC_SHADE),
	gsSPEndDisplayList(),
};

Gfx sky[] = {
        gsSPTexture (0x8000, 0x8000, 0, G_TX_RENDERTILE, G_ON),
	gsDPPipeSync(),
	gsDPSetCombineMode(G_CC_MODULATEIA, G_CC_MODULATEIA),
	gsDPLoadTextureBlock(tex_sky, G_IM_FMT_IA, G_IM_SIZ_16b,
		32, 32, 0,
		G_TX_WRAP | G_TX_NOMIRROR, G_TX_WRAP | G_TX_NOMIRROR,
		4, 4, G_TX_NOLOD, G_TX_NOLOD),
	/* Turn on transparency */
	gsDPSetRenderMode(G_RM_AA_XLU_SURF, G_RM_AA_XLU_SURF2),

	gsSPVertex(&v[4], 12, 0),
	/* Top layer -- draw this first */
	gsSP1Triangle(8, 9, 10, 0),
	gsSP1Triangle(8, 10, 11, 0),
	/* Middle layer */
	gsSP1Triangle(4, 5, 6, 0),
	gsSP1Triangle(4, 6, 7, 0),
	/* Bottom layer -- draw this last */
	gsSP1Triangle(0, 1, 2, 0),
	gsSP1Triangle(0, 2, 3, 0),

	/* Turn off transparency */
	gsDPPipeSync(),
	gsDPSetRenderMode(G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2),

	/* Turn off texturing */
	gsSPTexture (0x8000, 0x8000, 0, G_TX_RENDERTILE, G_OFF),
	gsDPSetCombineMode(G_CC_SHADE, G_CC_SHADE),
	gsSPEndDisplayList(),
};
