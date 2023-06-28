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

#include "overlay.h"
#include <assert.h>

static Dynamic dynamic;

static void
drawSquareTx(OSTask *tlistp, int draw_buffer);

/*
 * this function must be located at top of segment.
 */
void 
drawSquare_Texture(OSTask *tlistp, int draw_buffer)
{
  drawSquareTx(tlistp, draw_buffer);
}

/*
 * Remember, viewport structures have 2 bits of fraction in them.
 */
static Vp vp = {
	SCREEN_WD*2, SCREEN_HT*2, G_MAXZ/2, 0,	/* scale */
	SCREEN_WD*2, SCREEN_HT*2, G_MAXZ/2, 0,	/* translate */
};

/* initialize the RSP state: */

static Gfx rspinit_dl[] = {
    gsSPViewport(&vp),
    gsSPClearGeometryMode(G_SHADE | G_SHADING_SMOOTH | G_CULL_BOTH |
			  G_FOG | G_LIGHTING | G_TEXTURE_GEN |
			  G_TEXTURE_GEN_LINEAR | G_LOD),
    gsSPTexture(0, 0, 0, 0, G_OFF),
    gsSPSetGeometryMode(G_SHADE | G_SHADING_SMOOTH),
    gsSPEndDisplayList(),
};

/* initialize the RDP state: */

static Gfx rdpinit_dl[] = {
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
    gsDPPipeSync(),
    gsSPEndDisplayList(),
};


/* clear the color frame buffer: */

static Gfx clearcfb_dl[] = {
    gsDPSetCycleType(G_CYC_FILL),
    gsDPSetColorImage(G_IM_FMT_RGBA, G_IM_SIZ_16b, SCREEN_WD, rsp_cfb),
    gsDPSetFillColor(GPACK_RGBA5551(64,64,64,1) << 16 | 
		     GPACK_RGBA5551(64,64,64,1)),
    gsDPFillRectangle(0, 0, SCREEN_WD-1, SCREEN_HT-1),
    gsDPPipeSync(),
    gsDPSetFillColor(GPACK_RGBA5551(64,64,255,1) << 16 | 
		     GPACK_RGBA5551(64,64,255,1)),
    gsDPFillRectangle(20, 20, SCREEN_WD-20, SCREEN_HT-20),
    gsSPEndDisplayList(),
};

/* 
 * This group of vertices, and accompanying display list, demonstrate
 * simple textured triangles.
 *
 * The texture coordinates are texel-space coordinates that
 * are shifted up 6 bits; 5 bits because the texture coordinates
 * are S10.5 format, and 1 more bit so we can use a texture scaling
 * of 0.5, and get exact coordinates.
 *
 * The coordinates larger than the texture resolution, along with the
 * proper mode settings, cause the texture to repeat on itself.
 */
static Vtx tex_vtx[] =  {
        { -64,  64, -5, 0, (  0 << 6), (  0 << 6), 0xff, 0xff, 0xff, 0xff},
        {  64,  64, -5, 0, (127 << 6), (  0 << 6), 0xff, 0xff, 0xff, 0xff},
        {  64, -64, -5, 0, (127 << 6), (127 << 6), 0xff, 0xff, 0xff, 0xff},
        { -64, -64, -5, 0, (  0 << 6), (127 << 6), 0xff, 0xff, 0xff, 0xff},
};

/* a 32x32 RGBA16 texture: */
#include "src/main/brick.h"

static Gfx texsqr_dl[] = {
    gsSPMatrix(OS_K0_TO_PHYSICAL(&(dynamic.projection)),
	       G_MTX_PROJECTION|G_MTX_LOAD|G_MTX_NOPUSH),
    gsSPMatrix(OS_K0_TO_PHYSICAL(&(dynamic.modeling)),
	       G_MTX_MODELVIEW|G_MTX_LOAD|G_MTX_NOPUSH),
    gsDPPipeSync(),
    gsDPSetCycleType(G_CYC_1CYCLE),
    gsDPSetRenderMode(G_RM_AA_OPA_SURF, G_RM_AA_OPA_SURF2),
    gsSPClearGeometryMode(G_SHADE | G_SHADING_SMOOTH),
    /*
     * The texture scaling parameters are .16 format. In order to
     * get an exact texture scale of 1.0, we scale above by 2.0, and
     * scale down here by 0.5
     */
    gsSPTexture(0x8000, 0x8000, 0, G_TX_RENDERTILE, G_ON),
    gsDPSetCombineMode(G_CC_DECALRGB, G_CC_DECALRGB),
    gsDPSetTextureFilter(G_TF_BILERP),
    gsDPLoadTextureBlock(brick, G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 32, 0,
			 G_TX_WRAP | G_TX_MIRROR, G_TX_WRAP | G_TX_MIRROR,
			 5, 5, G_TX_NOLOD, G_TX_NOLOD),
    gsSPVertex(&(tex_vtx[0]), 4, 0),
    gsSP1Triangle(0, 1, 2, 0),
    gsSP1Triangle(0, 2, 3, 0),
    gsSPTexture(0, 0, 0, 0, G_OFF), /* done; turn texture off */
    gsSPEndDisplayList(),
};

static void
drawSquareTx(OSTask *tlistp, int draw_buffer)
{
	Dynamic *dynamicp = &dynamic;
	Gfx *glistp;
	static float theta = 0.0;

	guOrtho(&dynamicp->projection,
		-(float)SCREEN_WD/2.0F, (float)SCREEN_WD/2.0F,
		-(float)SCREEN_HT/2.0F, (float)SCREEN_HT/2.0F,
		1.0F, 10.0F, 1.0F);
	guRotate(&dynamicp->modeling, theta, 0.0F, 0.0F, 1.0F);

	theta += 1.0F;

	glistp = dynamicp->glist;

	/*
	 * Tell RCP where each segment is
	 */
	gSPSegment(glistp++, 0, 0x0);	/* Physical address segment */
	gSPSegment(glistp++, CFB_SEGMENT, OS_K0_TO_PHYSICAL(cfb[draw_buffer]));

	/*
	 * Initialize RDP state.
	 */
	gSPDisplayList(glistp++, rdpinit_dl);

	/*
	 * Initialize RSP state.
	 */
	gSPDisplayList(glistp++, rspinit_dl);

	/*
	 * Clear color framebuffer.
	 */
	gSPDisplayList(glistp++, clearcfb_dl);

	/* Draw textured square */

	gSPDisplayList(glistp++, texsqr_dl);

	gDPFullSync(glistp++);
	gSPEndDisplayList(glistp++);

	assert((glistp-dynamicp->glist) < GLIST_LEN);

	/* 
	 * Build graphics task:
	 *
	 */
	tlistp->t.ucode_boot = (u64 *) rspbootTextStart;
	tlistp->t.ucode_boot_size = (u32)rspbootTextEnd - (u32)rspbootTextStart;

	tlistp->t.ucode = (u64 *) gspF3DEX2_xbusTextStart;
	tlistp->t.ucode_data = (u64 *) gspF3DEX2_xbusDataStart;
	
	/* initial display list: */
	tlistp->t.data_ptr = (u64 *) dynamicp->glist;
	tlistp->t.data_size = (u32)((glistp - dynamicp->glist) * sizeof(Gfx));

	/*
	 * Write back dirty cache lines that need to be read by the RCP.
	 */
	osWritebackDCache(&dynamic, sizeof(dynamic));
}
