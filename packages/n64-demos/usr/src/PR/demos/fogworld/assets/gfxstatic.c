
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
 * File:	gfxstatic.c
 *
 * This file holds display list segments that are 'static' data.
 *
 */

#include <ultra64.h>
#include "boot.h"
#include "game.h"
#include "ter0_header.h"



/***********************************************************************
 ******************* RENDERING SETUP DISPLAY LISTS *********************
 ***********************************************************************/

/*
 *   DIFFUSE LIGHT
 */
Gfx setrend_light[] = {
    gsDPPipeSync(),

    gsDPSetCycleType(G_CYC_2CYCLE),
    gsSPSetGeometryMode(G_ZBUFFER | 
			G_CULL_BACK | 
			G_LIGHTING | 
			G_SHADE | 
			G_SHADING_SMOOTH),
    gsDPSetRenderMode(G_RM_PASS, G_RM_AA_ZB_OPA_SURF2),
    gsDPSetCombineMode(G_CC_SHADE, G_CC_SHADE),

    gsSPEndDisplayList(),
};

/*
 *   DIFFUSE LIGHT FOG
 */
Gfx setrend_light_fog[] = {
    gsDPPipeSync(),

    gsDPSetCycleType(G_CYC_2CYCLE),
    gsSPSetGeometryMode(G_FOG |
			G_ZBUFFER | 
			G_CULL_BACK | 
			G_LIGHTING | 
			G_SHADE | 
			G_SHADING_SMOOTH),
    gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_AA_ZB_OPA_SURF2),
    gsDPSetCombineMode(G_CC_SHADE, G_CC_SHADE),

    gsSPEndDisplayList(),
};

/*
 *   LINE
 */
Gfx setrend_line[] = {
    gsDPPipeSync(),

    gsDPSetCycleType(G_CYC_2CYCLE),
    gsSPSetGeometryMode(G_ZBUFFER |
                        G_SHADE |
			G_SHADING_SMOOTH ),
    gsDPSetRenderMode(G_RM_PASS, G_RM_AA_ZB_XLU_LINE2),
    gsDPSetCombineMode(G_CC_SHADE, G_CC_SHADE),

    gsSPEndDisplayList(),
};

/*
 *   LINE FOG
 */
Gfx setrend_line_fog[] = {
    gsDPPipeSync(),

    gsDPSetCycleType(G_CYC_2CYCLE),
    gsSPSetGeometryMode(G_ZBUFFER |
			G_FOG |
                        G_SHADE |
			G_SHADING_SMOOTH ),
    gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_AA_ZB_XLU_LINE2),
    gsDPSetCombineMode(G_CC_SHADE, G_CC_SHADE),

    gsSPEndDisplayList(),
};

/***********************************************************************
 ******************* MODELS ********************************************
 ***********************************************************************/

/*
 * Terrain **************************************
 */

Mtx terr_shift1 = {
	0x00010000,	0x00000000,
	0x00000001,	0x00000000,
	0x00000000,	0x00010000,
	0x04000000,	0x00000001,

	0x00000000,	0x00000000,
	0x00000000,	0x00000000,
	0x00000000,	0x00000000,
	0x00000000,	0x00000000,
};
Mtx terr_shift2 = {
	0x00010000,	0x00000000,
	0x00000001,	0x00000000,
	0x00000000,	0x00010000,
	0xfc000000,	0x00000001,

	0x00000000,	0x00000000,
	0x00000000,	0x00000000,
	0x00000000,	0x00000000,
	0x00000000,	0x00000000,
};
Mtx terr_shift3 = {
	0x00010000,	0x00000000,
	0x00000001,	0x00000000,
	0x00000000,	0x00010000,
	0x00000000,	0x04000001,

	0x00000000,	0x00000000,
	0x00000000,	0x00000000,
	0x00000000,	0x00000000,
	0x00000000,	0x00000000,
};
Mtx terr_shift4 = {
	0x00010000,	0x00000000,
	0x00000001,	0x00000000,
	0x00000000,	0x00010000,
	0x00000000,	0xfc000001,

	0x00000000,	0x00000000,
	0x00000000,	0x00000000,
	0x00000000,	0x00000000,
	0x00000000,	0x00000000,
};


#include "ter0_data.h"


Gfx terrain_dl[] = {
    gsSPDisplayList(terrain0_dl),
    gsSPMatrix(&terr_shift1,G_MTX_MODELVIEW|G_MTX_MUL|G_MTX_PUSH),
    gsSPDisplayList(terrain0_dl),
    gsSPMatrix(&terr_shift3,G_MTX_MODELVIEW|G_MTX_MUL|G_MTX_NOPUSH),
    gsSPDisplayList(terrain0_dl),
    gsSPPopMatrix(G_MTX_MODELVIEW),
    gsSPMatrix(&terr_shift3,G_MTX_MODELVIEW|G_MTX_MUL|G_MTX_PUSH),
    gsSPDisplayList(terrain0_dl),
    gsSPMatrix(&terr_shift2,G_MTX_MODELVIEW|G_MTX_MUL|G_MTX_NOPUSH),
    gsSPDisplayList(terrain0_dl),
    gsSPPopMatrix(G_MTX_MODELVIEW),
    gsSPMatrix(&terr_shift2,G_MTX_MODELVIEW|G_MTX_MUL|G_MTX_PUSH),
    gsSPDisplayList(terrain0_dl),
    gsSPMatrix(&terr_shift4,G_MTX_MODELVIEW|G_MTX_MUL|G_MTX_NOPUSH),
    gsSPDisplayList(terrain0_dl),
    gsSPPopMatrix(G_MTX_MODELVIEW),
    gsSPMatrix(&terr_shift4,G_MTX_MODELVIEW|G_MTX_MUL|G_MTX_PUSH),
    gsSPDisplayList(terrain0_dl),
    gsSPMatrix(&terr_shift1,G_MTX_MODELVIEW|G_MTX_MUL|G_MTX_NOPUSH),
    gsSPDisplayList(terrain0_dl),
    gsSPPopMatrix(G_MTX_MODELVIEW),
    gsSPEndDisplayList(),
};
