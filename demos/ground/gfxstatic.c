
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
#include "texture.h"

/*
 * ground
 */
static Vtx ground_vtx[] = {
  { -50,    0,   50, 0, 00<<10, 00<<10, 0xff, 0x00, 0x00, 0xff},
  {  50,    0,   50, 0, 32<<10, 00<<10, 0x00, 0xff, 0x00, 0xff},
  {  50,    0,  -50, 0, 32<<10, 32<<10, 0x00, 0x00, 0xff, 0xff},
  { -50,    0,  -50, 0, 00<<10, 32<<10, 0xff, 0xff, 0xff, 0xff},


};

Gfx ground_texture_setup_dl[] = {
    gsDPPipeSync(),

    gsDPSetCycleType(G_CYC_1CYCLE),
    gsSPSetGeometryMode(G_SHADE | G_SHADING_SMOOTH | G_ZBUFFER ),
    gsDPSetRenderMode(G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2),
    gsDPSetCombineMode(G_CC_BLENDRGBA, G_CC_BLENDRGBA),
    gsDPSetTexturePersp(G_TP_PERSP),

    gsSPEndDisplayList(),
};

Gfx ground_dl[] = {
    gsSPVertex(&(ground_vtx[0]), 4, 0),
/*
 * This is now done in game.c
 *
    gsSP1Triangle(0, 2, 1, 0),
    gsSP1Triangle(2, 0, 3, 0),
*/
    gsSPEndDisplayList(),
};


/*
 * BALL
 */
#include "sphere_vtx.h"
Gfx ball_dl[] = {
#   include "sphere_dl.h"
    gsSPEndDisplayList()
};

Lights1 light = gdSPDefLights1(
			 12,  12,  60,
			150, 150,  75,     30,  40,   0
);
Gfx ball_setup_dl[] = {
    gsDPPipeSync(),
    gsSPTexture(0,0,0,0,G_OFF),
    gsSPSetGeometryMode(G_LIGHTING | G_SHADE | G_SHADING_SMOOTH | G_ZBUFFER),
    gsDPSetCombineMode(G_CC_SHADE, G_CC_SHADE),
    gsSPSetLights1(light),

    gsSPEndDisplayList()
};

Gfx ball_shadow_setup_dl[] = {
    gsDPPipeSync(),
    gsSPTexture(0,0,0,0,G_OFF),
    gsSPClearGeometryMode(G_LIGHTING | G_SHADE | G_SHADING_SMOOTH),
    gsSPSetGeometryMode(G_ZBUFFER),
    gsDPSetCombineMode(G_CC_PRIMITIVE, G_CC_PRIMITIVE),
    gsDPSetRenderMode(G_RM_AA_ZB_XLU_DECAL, G_RM_AA_ZB_XLU_DECAL2),
    gsDPSetPrimColor(0,0,0,0,0,64),

    gsSPEndDisplayList()
};
