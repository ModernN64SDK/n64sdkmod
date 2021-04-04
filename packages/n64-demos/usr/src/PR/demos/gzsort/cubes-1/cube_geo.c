/*---------------------------------------------------------------------*
	Copyright (C) 1997, Nintendo.
	
	File		cube_geo.c
	Coded    by	Yoshitaka Yasumoto.	Sep 12, 1997.
	Modified by	
	
	$Id: cube_geo.c,v 1.5 1997/10/08 10:06:32 yasu Exp $
 *---------------------------------------------------------------------*/
#include	<ultra64.h>
#include	<PR/gzsort.h>
#include	"system.h"
#include	"cube.h"

/*--- View port parameter ---*/
Vp	viewport = {
  SCREEN_WD*2, -SCREEN_HT*2, G_MAXZ/2, GZ_VIEWPORT_FOG_S(FOG_IN, FOG_OUT),
  SCREEN_WD*2,  SCREEN_HT*2, G_MAXZ/2, GZ_VIEWPORT_FOG_T(FOG_IN, FOG_OUT),
};

/*--- Light parameter ---*/
Lights1	scene_light =
gdSPDefLights1( 0x20,0x20,0x20,			/* Ambient  */
	        0xe0,0xe0,0xe0,  0,40,80 );	/* Defuse 0 */

/*--- Macro to set render mode ---*/
#define	OTHERMODE_A(cyc)	(G_CYC_##cyc|G_PM_NPRIMITIVE|G_TP_PERSP|\
				 G_TD_CLAMP|G_TL_TILE|G_TT_NONE|G_TF_BILERP|\
				 G_TC_FILT|G_CK_NONE|G_CD_MAGICSQ|G_AD_DISABLE)
#define	OTHERMODE_B(rm1,rm2)	(G_AC_NONE|G_ZS_PRIM|G_RM_##rm1|G_RM_##rm2)

/*--- RDPCMD to switch into Shade Triangle Mode ---*/
Gfx	rdpcmdShPoly[] = {
  gsDPPipeSync(),
#ifdef	FOG
  gsDPSetOtherMode(OTHERMODE_A(1CYCLE), OTHERMODE_B(FOG_SHADE_A,OPA_SURF2)),
  gsDPSetFogColor(64,64,255,255),
#else
  gsDPSetOtherMode(OTHERMODE_A(1CYCLE), OTHERMODE_B(OPA_SURF,OPA_SURF2)),
#endif
  gsDPSetCombineMode(G_CC_SHADE, G_CC_SHADE),
  gsSPEndDisplayList(),
};

/*--- RDPCMD to switch into Textured Triangle Mode ---*/
#include "brick.h"

Gfx	rdpcmdTxPoly1[] = {
  gsDPPipeSync(),
#ifdef	FOG
  gsDPSetOtherMode(OTHERMODE_A(1CYCLE), OTHERMODE_B(FOG_SHADE_A,OPA_SURF2)),
  gsDPSetFogColor(67,67,255,255),
#else
  gsDPSetOtherMode(OTHERMODE_A(1CYCLE), OTHERMODE_B(OPA_SURF,OPA_SURF2)),
#endif
  gsDPSetCombineMode(G_CC_MODULATERGB, G_CC_MODULATERGB),
  gsDPSetTextureFilter(G_TF_BILERP),
  gsSPEndDisplayList(),
};

Gfx	rdpcmdTxPoly2[] = {
  gsDPPipeSync(),
  gsDPLoadTextureBlock(brick, G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 32, 0,
		       G_TX_WRAP | G_TX_MIRROR, G_TX_WRAP | G_TX_MIRROR,
		       5, 5, G_TX_NOLOD, G_TX_NOLOD),
  gsSPEndDisplayList(),
};

/*--- RDPCMD to switch into Textured Triangle Mode (Reflection) ---*/
#include "reflect.h"

Gfx	rdpcmdTxPoly2Reflect[] = {
  gsDPPipeSync(),
  gsDPLoadTextureBlock(tex_reflect, G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 32, 0,
		       G_TX_WRAP | G_TX_MIRROR, G_TX_WRAP | G_TX_MIRROR,
		       5, 5, G_TX_NOLOD, G_TX_NOLOD),
  gsSPEndDisplayList(),
};

/*--- RDPCMD array ---*/
Gfx	*cube_gfx[] = {
  NULL,			/*  0 */
  rdpcmdShPoly,		/*  1 */
  rdpcmdTxPoly1,	/*  2 */
  rdpcmdTxPoly2,	/*  3 */
  rdpcmdTxPoly2Reflect,	/*  4 */
  NULL,			/* Padding -- Dummy */
};

#define	RDPCMD_NULL		0
#define	RDPCMD_SHPOLY		1
#define	RDPCMD_TXPOLY1		2
#define	RDPCMD_TXPOLY2		3
#define	RDPCMD_TXPOLY2_REFLECT	4

/*--- Cube vertices positions ---*/
zVtxSrc	cube_vtx[] = {		/* CHECK_ALIGN(cube_vtx, 8) */
/* X     Y     Z  */
{  32,  32,  32  },		/* Vtx 0 */
{ -32,  32,  32  },		/* Vtx 1 */
{ -32, -32,  32  },		/* Vtx 2 */
{  32, -32,  32  },		/* Vtx 3 */
{  32,  32, -32  },		/* Vtx 4 */
{ -32,  32, -32  },		/* Vtx 5 */
{ -32, -32, -32  },		/* Vtx 6 */
{  32, -32, -32  },		/* Vtx 7 */
};

zVtxSrc	cube_vtx_reflect[] = {	/* CHECK_ALIGN(cube_vtx, 8) */
/* X     Y     Z  */
{  32,  32,  32  },		/* Vtx 0 */
{ -32,  32,  32  },		/* Vtx 1 */
{ -32, -32,  32  },		/* Vtx 2 */
{  32, -32,  32  },		/* Vtx 3 */
{  32,  32, -32  },		/* Vtx 4 */
{ -32,  32, -32  },		/* Vtx 5 */
{ -32, -32, -32  },		/* Vtx 6 */
{  32, -32, -32  },		/* Vtx 7 */
{  32,  32,  32  },		/* Vtx 8 */
{ -32,  32,  32  },		/* Vtx 9 */
{ -32, -32,  32  },		/* Vtx10 */
{  32, -32,  32  },		/* Vtx11 */
{  32,  32, -32  },		/* Vtx12 */
{ -32,  32, -32  },		/* Vtx13 */
{ -32, -32, -32  },		/* Vtx14 */
{  32, -32, -32  },		/* Vtx15 */

{  32,  32,  32  },		/* Vtx16 */
{ -32,  32,  32  },		/* Vtx17 */
{ -32, -32,  32  },		/* Vtx18 */
{  32, -32,  32  },		/* Vtx19 */
{  32,  32, -32  },		/* Vtx20 */
{ -32,  32, -32  },		/* Vtx21 */
{ -32, -32, -32  },		/* Vtx22 */
{  32, -32, -32  },		/* Vtx23 */
};

/*--- Cube vertices colors ---*/
zColor	cube_color[] = {	//=== ALIGN 8
{ 255,   0,   0, 255 },		/* Vtx 0 */
{   0, 255,   0, 255 },		/* Vtx 1 */
{   0,   0, 255, 255 },		/* Vtx 2 */
{   0, 255, 255, 255 },		/* Vtx 3 */
{ 255,   0, 255, 255 },		/* Vtx 4 */
{ 255, 255,   0, 255 },		/* Vtx 5 */
{ 255, 255, 255, 255 },		/* Vtx 6 */
{   0,   0,   0, 255 },		/* Vtx 7 */
};

zColor	cube_color_reflect[] = {//=== ALIGN 8
{ 255, 128, 128, 255 },		/* Vtx 0 */
{ 128, 255, 128, 255 },		/* Vtx 1 */
{ 128, 128, 255, 255 },		/* Vtx 2 */
{ 128, 255, 255, 255 },		/* Vtx 3 */
{ 255, 128, 255, 255 },		/* Vtx 4 */
{ 255, 255, 128, 255 },		/* Vtx 5 */
};

/*--- Cube vertices normals ---*/
zNorm	cube_norm_flat[] = {	//=== ALIGN 8
{    0,   0, 120  },		/* Face 0 */
{    0, 120,   0  },		/* Face 1 */
{ -120,   0,   0  },		/* Face 2 */
{    0,-120,   0  },		/* Face 3 */
{  120,   0,   0  },		/* Face 4 */
{    0,   0,-120  },		/* Face 5 */
};

u8	cube_norm_padding[4] = {0};
zNorm	cube_norm_smooth[] = {	//=== ALIGN 8
{   70,  70,  70  },		/* Vtx 0 */
{  -70,  70,  70  },		/* Vtx 1 */
{  -70, -70,  70  },		/* Vtx 2 */
{   70, -70,  70  },		/* Vtx 3 */
{   70,  70, -70  },		/* Vtx 4 */
{  -70,  70, -70  },		/* Vtx 5 */
{  -70, -70, -70  },		/* Vtx 6 */
{   70, -70, -70  },		/* Vtx 7 */
};

zNorm	cube_norm_reflect[] = {	//=== ALIGN 8
#define	A	16
#define	B	125		/* SQRT(127*127-2*A*A) */
{   A,  A,  B  },		/* Vtx 0-0 */
{  -A,  B,  A  },		/* Vtx 1-1 */
{  -B, -A,  A  },		/* Vtx 2-2 */
{   A, -B,  A  },		/* Vtx 1-3 */
{   B,  A, -A  },		/* Vtx 2-4 */
{  -A,  A, -B  },		/* Vtx 0-5 */
{  -A, -A, -B  },		/* Vtx 0-6 */
{   A, -A, -B  },		/* Vtx 0-7 */
{   A,  B,  A  },		/* Vtx 1-0 */
{  -A,  A,  B  },		/* Vtx 0-1 */
{  -A, -B,  A  },		/* Vtx 1-2 */
{   A, -A,  B  },		/* Vtx 0-3 */
{   A,  B, -A  },		/* Vtx 1-4 */
{  -A,  B, -A  },		/* Vtx 1-5 */
{  -A, -B, -A  },		/* Vtx 1-6 */
{   A, -B, -A  },		/* Vtx 1-7 */
{   B,  A,  A  },		/* Vtx 2-0 */
{  -B,  A,  A  },		/* Vtx 2-1 */
{  -A, -A,  B  },		/* Vtx 0-2 */
{   B, -A,  A  },		/* Vtx 2-3 */
{   A,  A, -B  },		/* Vtx 0-4 */
{  -B,  A, -A  },		/* Vtx 2-5 */
{  -B, -A, -A  },		/* Vtx 2-6 */
{   B, -A, -A  },		/* Vtx 2-7 */
#undef	A
#undef	B
};

/*--- Cube vertices texture coordinates ---*/
zTxtr   cube_txtr[] = {
{  0<<5,  0<<5 },		/* Vtx 0 */
{ 32<<5,  0<<5 },		/* Vtx 1 */
{ 32<<5, 32<<5 },		/* Vtx 2 */
{  0<<5, 32<<5 },		/* Vtx 3 */
{  0<<5, 32<<5 },		/* Vtx 4 */
{ 32<<5, 32<<5 },		/* Vtx 5 */
{ 32<<5,  0<<5 },		/* Vtx 6 */
{  0<<5,  0<<5 },		/* Vtx 7 */
};

zFace	cube_face_sh_flat[] = {
  zsPoly(ZPM_FLAT|ZPM_CULLBACK, RDPCMD_SHPOLY),
  zsQuadrangleD(0, 1, 2, 3),	/* Face 0 */
  zsQuadrangleD(1, 0, 4, 5),	/* Face 1 */
  zsQuadrangleD(2, 1, 5, 6),	/* Face 2 */
  zsQuadrangleD(3, 2, 6, 7),	/* Face 3 */
  zsQuadrangleD(4, 0, 3, 7),	/* Face 4 */
  zsQuadrangleD(5, 4, 7, 6),	/* Face 5 */
  zsQuadrangleD(4, 0, 3, 7),	/* Face 4 */
  zsEnd(),
};

zFace	cube_face_sh_smooth[] = {
  zsPoly(ZPM_SMOOTH|ZPM_CULLBACK, RDPCMD_SHPOLY),
#ifdef	xDEBUG
  zsTriangle(1, 0, 4),	/* Face 0 */
  zsEnd(),
#endif
  zsQuadrangleD(0, 1, 2, 3),	/* Face 0 */
  zsQuadrangleD(1, 0, 4, 5),	/* Face 1 */
  zsQuadrangleD(2, 1, 5, 6),	/* Face 2 */
  zsQuadrangleD(3, 2, 6, 7),	/* Face 3 */
  zsQuadrangleD(4, 0, 3, 7),	/* Face 4 */
  zsQuadrangleD(5, 4, 7, 6),	/* Face 5 */
  zsEnd(),
};

zFace	cube_face_tx_null[] = {
  zsPolyTxtr(ZPM_NULL|ZPM_CULLBACK,
	     RDPCMD_TXPOLY1, RDPCMD_TXPOLY2, RDPCMD_NULL),
  zsQuadrangleD(0, 1, 2, 3),	/* Face 0 */
  zsQuadrangleD(1, 0, 4, 5),	/* Face 1 */
  zsQuadrangleD(2, 1, 5, 6),	/* Face 2 */
  zsQuadrangleD(3, 2, 6, 7),	/* Face 3 */
  zsQuadrangleD(4, 0, 3, 7),	/* Face 4 */
  zsQuadrangleD(5, 4, 7, 6),	/* Face 5 */
  zsEnd(),
};

zFace	cube_face_tx_flat[] = {
  zsPolyTxtr(ZPM_FLAT|ZPM_CULLBACK,
	     RDPCMD_TXPOLY1, RDPCMD_TXPOLY2, RDPCMD_NULL),
  zsQuadrangleD(0, 1, 2, 3),	/* Face 0 */
  zsQuadrangleD(1, 0, 4, 5),	/* Face 1 */
  zsQuadrangleD(2, 1, 5, 6),	/* Face 2 */
  zsQuadrangleD(3, 2, 6, 7),	/* Face 3 */
  zsQuadrangleD(4, 0, 3, 7),	/* Face 4 */
  zsQuadrangleD(5, 4, 7, 6),	/* Face 5 */
  zsEnd(),
};

zFace	cube_face_tx_smooth[] = {
  zsPolyTxtr(ZPM_SMOOTH|ZPM_CULLBACK,
	     RDPCMD_TXPOLY1, RDPCMD_TXPOLY2, RDPCMD_NULL),
  zsQuadrangleD(0, 1, 2, 3),	/* Face 0 */
  zsQuadrangleD(1, 0, 4, 5),	/* Face 1 */
  zsQuadrangleD(2, 1, 5, 6),	/* Face 2 */
  zsQuadrangleD(3, 2, 6, 7),	/* Face 3 */
  zsQuadrangleD(4, 0, 3, 7),	/* Face 4 */
  zsQuadrangleD(5, 4, 7, 6),	/* Face 5 */
  zsEnd(),
};

zFace	cube_face_reflect_null[] = {
  zsPolyTxtr(ZPM_NULL|ZPM_CULLBACK,
	     RDPCMD_TXPOLY1, RDPCMD_TXPOLY2_REFLECT, RDPCMD_NULL),
  zsQuadrangleD( 0, 9,18,11),	/* Face 0 */
  zsQuadrangleD( 1, 8,12,13),	/* Face 1 */
  zsQuadrangleD( 2,17,21,22),	/* Face 2 */
  zsQuadrangleD( 3,10,14,15),	/* Face 3 */
  zsQuadrangleD( 4,16,19,23),	/* Face 4 */
  zsQuadrangleD( 5,20, 7, 6),	/* Face 5 */
  zsEnd(),
};

zFace	cube_face_reflect_flat[] = {
  zsPolyTxtr(ZPM_FLAT|ZPM_CULLBACK,
	     RDPCMD_TXPOLY1, RDPCMD_TXPOLY2_REFLECT, RDPCMD_NULL),
  zsQuadrangleD( 0, 9,18,11),	/* Face 0 */
  zsQuadrangleD( 1, 8,12,13),	/* Face 1 */
  zsQuadrangleD( 2,17,21,22),	/* Face 2 */
  zsQuadrangleD( 3,10,14,15),	/* Face 3 */
  zsQuadrangleD( 4,16,19,23),	/* Face 4 */
  zsQuadrangleD( 5,20, 7, 6),	/* Face 5 */
  zsEnd(),
};

zPoly	cube_poly[CUBE_ARRAY_MAX] = {
  {
#ifndef	xDEBUG
    /*>>>>>> Flat-Shade <<<<<<*/
    ZRM_NULL,
    sizeof(cube_vtx)/sizeof(zVtxSrc), 0,
    cube_vtx, cube_color, NULL, NULL,
    cube_face_sh_flat, cube_gfx
  },{
    /*>>>>>> Smooth-Shade <<<<<<*/
    ZRM_NULL,
    sizeof(cube_vtx)/sizeof(zVtxSrc), 0,
    cube_vtx, cube_color, NULL, NULL,
    cube_face_sh_smooth, cube_gfx
  },{
    /*>>>>>> Texture <<<<<<*/
    ZRM_NULL,
    sizeof(cube_vtx)/sizeof(zVtxSrc), 0,
    cube_vtx, NULL, NULL, cube_txtr,
    cube_face_tx_null, cube_gfx
  },{
    /*>>>>>> Texture Flat-Shade <<<<<<*/
    ZRM_NULL,
    sizeof(cube_vtx)/sizeof(zVtxSrc), 0,
    cube_vtx, cube_color, NULL, cube_txtr,
    cube_face_tx_flat, cube_gfx
  },{
    /*>>>>>> Texture Smooth-Shade <<<<<<*/
    ZRM_NULL,
    sizeof(cube_vtx)/sizeof(zVtxSrc), 0,
    cube_vtx, cube_color, NULL, cube_txtr,
    cube_face_tx_smooth, cube_gfx
  },{
    /*>>>>>> Light Flat-Shade <<<<<<*/
    ZRM_LIGHT,
    sizeof(cube_vtx)/sizeof(zVtxSrc),
    sizeof(cube_norm_flat)/sizeof(zNorm),
    cube_vtx, cube_color, cube_norm_flat, NULL,
    cube_face_sh_flat, cube_gfx
  },{
#endif
    /*>>>>>> Light Smooth-Shade <<<<<<*/
    ZRM_LIGHT,
    sizeof(cube_vtx)/sizeof(zVtxSrc),
    sizeof(cube_norm_smooth)/sizeof(zNorm),
    cube_vtx, cube_color, cube_norm_smooth, NULL,
    cube_face_sh_smooth, cube_gfx
  },{
    /*>>>>>> Light Texture Flat-Shade <<<<<<*/
    ZRM_LIGHT,
    sizeof(cube_vtx)/sizeof(zVtxSrc),
    sizeof(cube_norm_flat)/sizeof(zNorm),
    cube_vtx, cube_color, cube_norm_flat, cube_txtr,
    cube_face_tx_flat, cube_gfx
  },{
    /*>>>>>> Light Texture Smooth-Shade <<<<<<*/
    ZRM_LIGHT,
    sizeof(cube_vtx)/sizeof(zVtxSrc),
    sizeof(cube_norm_smooth)/sizeof(zNorm),
    cube_vtx, cube_color, cube_norm_smooth, cube_txtr,
    cube_face_tx_smooth, cube_gfx
  },{
    /*>>>>>> Light Material Flat-Shade <<<<<<*/
    ZRM_LIGHT|ZRM_MATERIAL,
    sizeof(cube_vtx)/sizeof(zVtxSrc),
    sizeof(cube_norm_flat)/sizeof(zNorm),
    cube_vtx, cube_color, cube_norm_flat, NULL,
    cube_face_sh_flat, cube_gfx
  },{
    /*>>>>>> Light Material Smooth-Shade <<<<<<*/
    ZRM_LIGHT|ZRM_MATERIAL,
    sizeof(cube_vtx)/sizeof(zVtxSrc),
    sizeof(cube_norm_smooth)/sizeof(zNorm),
    cube_vtx, cube_color, cube_norm_smooth, NULL,
    cube_face_sh_smooth, cube_gfx
  },{
    /*>>>>>> Light Material Texture Flat-Shade <<<<<<*/
    ZRM_LIGHT|ZRM_MATERIAL,
    sizeof(cube_vtx)/sizeof(zVtxSrc),
    sizeof(cube_norm_flat)/sizeof(zNorm),
    cube_vtx, cube_color, cube_norm_flat, cube_txtr,
    cube_face_tx_flat, cube_gfx
  },{
    /*>>>>>> Light Material Texture Smooth-Shade <<<<<<*/
    ZRM_LIGHT|ZRM_MATERIAL,
    sizeof(cube_vtx)/sizeof(zVtxSrc),
    sizeof(cube_norm_smooth)/sizeof(zNorm),
    cube_vtx, cube_color, cube_norm_smooth, cube_txtr,
    cube_face_tx_smooth, cube_gfx
  },{
    /*>>>>>> Texture generation <<<<<<*/
    ZRM_TEX_GEN,
    sizeof(cube_vtx_reflect)/sizeof(zVtxSrc),
    sizeof(cube_norm_reflect)/sizeof(zNorm),
    cube_vtx_reflect, NULL, cube_norm_reflect, NULL,
    cube_face_reflect_null, cube_gfx
  },{    
    /*>>>>>> Texture generation Flat-Shade <<<<<<*/
    ZRM_TEX_GEN,
    sizeof(cube_vtx_reflect)/sizeof(zVtxSrc),
    sizeof(cube_norm_reflect)/sizeof(zNorm),
    cube_vtx_reflect, cube_color_reflect, cube_norm_reflect, NULL,
    cube_face_reflect_flat, cube_gfx
  },
};

/*======== End of cube_geo.c ========*/
