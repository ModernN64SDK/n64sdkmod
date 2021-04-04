/*---------------------------------------------------------------------
	Copyright (C) 1997, Nintendo.
	
	File		sprite.c
	Coded    by	Yoshitaka Yasumoto.	Mar 19, 1997.
	Modified by	
	Comments	
	
	$Id: sprite.c,v 1.11 1997/08/07 03:24:20 yasu Exp $
  ---------------------------------------------------------------------*/
#include	<ultra64.h>
#include	<PR/gs2dex.h>
#include	"texture.h"

extern	u8	ut_gorogoro[];
extern	u16	utPal_gorogoro[];

/*---------------------------------------------------------------------------*
 *	The Background structure
 *---------------------------------------------------------------------------*/
uObjBg	objBg = {
  0, 328<<2, 0<<2, 320<<2,	/* imageX, imageW, frameX, frameW */
  0, 240<<2, 0<<2, 240<<2,	/* imageY, imageH, frameY, frameH */
  0,				/* imagePtr                       */
  G_BGLT_LOADBLOCK,		/* imageLoad                      */
  G_IM_FMT_RGBA,		/* imageFmt                       */
  G_IM_SIZ_16b,			/* imageSiz                       */
  0,				/* imagePal                       */
  0				/* imageFlip                      */
};

/* Load CI8 TLUT */
uObjTxtr objBgTLUT = {
  G_OBJLT_TLUT,				/* type    */
  (u64 *)L2_TV_pal,			/* image   */
  GS_PAL_HEAD(1),			/* phead   */
  GS_PAL_NUM(255),			/* pnum    */
  0,					/* zero    */
  0,					/* sid     */
  -1,					/* flag    */
  0					/* mask    */
};

/*---------------------------------------------------------------------------*
 *	The 2D Object structure
 *---------------------------------------------------------------------------*/

/* The grid */
uObjSprite	objRect[] = {{
  0<<2, 1<<10, 32<<5, 0,		/* objX, scaleX, imageW, paddingX */
  0<<2, 1<<10, 32<<5, 0,		/* objY, scaleY, imageH, paddingY */
  GS_PIX2TMEM(32, G_IM_SIZ_16b),	/* imageStride */
  GS_PIX2TMEM(0, G_IM_SIZ_16b),		/* imageAdrs   */
  G_IM_FMT_RGBA,			/* imageFmt */
  G_IM_SIZ_16b,				/* imageSiz */
  0,					/* imagePal */
  0					/* imageFlags */
},{
  0<<2, 1<<10, 32<<5, 0,		/* objX, scaleX, imageW, paddingX */
  0<<2, 1<<10, 32<<5, 0,		/* objY, scaleY, imageH, paddingY */
  GS_PIX2TMEM(32, G_IM_SIZ_16b),	/* imageStride */
  GS_PIX2TMEM( 0, G_IM_SIZ_16b),	/* imageAdrs   */
  G_IM_FMT_RGBA,			/* imageFmt */
  G_IM_SIZ_16b,				/* imageSiz */
  0,					/* imagePal */
  0					/* imageFlags */
}};

/*---------------------------------------------------------------------------*
 *	The 2D Matrix structure
 *---------------------------------------------------------------------------*/
uObjMtx	objMtx[3] = {
{ 
  0x10000,  0,			/* A,B */
  0,        0x10000,		/* C,D */
  0,        0,			/* X,Y */
  1<<10,    1<<10		/* BaseScaleX, BaseScaleY */
}, { 
  0x10000,  0,			/* A,B */
  0,        0x10000,		/* C,D */
  0,        0,			/* X,Y */
  1<<10,    1<<10		/* BaseScaleX, BaseScaleY */
}, { 
  0x10000,  0,			/* A,B */
  0,        0x10000,		/* C,D */
  0,        0,			/* X,Y */
  1<<10,    1<<10		/* BaseScaleX, BaseScaleY */
}
};

/*---------------------------------------------------------------------------*
 *	The Texture load structure
 *---------------------------------------------------------------------------*/
/* The RGBA16 Texture load by LoadBlock */
uObjTxtr objTxtrBlock_RGBA16 = {
  G_OBJLT_TXTRBLOCK,			/* type    */
  (u64 *)textureRGBA16,			/* image   */
  GS_PIX2TMEM(0,     G_IM_SIZ_16b),	/* tmem    */
  GS_TB_TSIZE(32*32, G_IM_SIZ_16b),	/* tsize   */
  GS_TB_TLINE(32,    G_IM_SIZ_16b),	/* tline   */
  0,					/* sid     */
  -1,					/* flag    */
  0 					/* mask    */
};

/* The RGBA16 Texture load by LoadTile */
uObjTxtr objTxtrTile_RGBA16 = {
  G_OBJLT_TXTRTILE,			/* type    */
  (u64 *)textureRGBA16,			/* image   */
  GS_PIX2TMEM(0,  G_IM_SIZ_16b),	/* tmem    */
  GS_TT_TWIDTH(32, G_IM_SIZ_16b),	/* twidth  */
  GS_TT_THEIGHT(32, G_IM_SIZ_16b),	/* theight */
  0,					/* sid     */
  -1,					/* flag    */
  0					/* mask    */
};

/* The CI4 Texture load by LoadBlock */
uObjTxtr objTxtrBlock_CI4 = {
  G_OBJLT_TXTRBLOCK,			/* type    */
  (u64 *)textureCI4,			/* image   */
  GS_PIX2TMEM(0,     G_IM_SIZ_4b),	/* tmem    */
  GS_TB_TSIZE(32*32, G_IM_SIZ_4b),	/* tsize   */
  GS_TB_TLINE(32,    G_IM_SIZ_4b),	/* tline   */
  0,					/* sid     */
  -1,					/* flag    */
  0 					/* mask    */
};

/* The CI4 Texture load by LoadTile */
uObjTxtr objTxtrTile_CI4 = {
  G_OBJLT_TXTRTILE,			/* type    */
  (u64 *)textureCI4,			/* image   */
  GS_PIX2TMEM(0,  G_IM_SIZ_4b),	/* tmem    */
  GS_TT_TWIDTH(32, G_IM_SIZ_4b),	/* twidth  */
  GS_TT_THEIGHT(32, G_IM_SIZ_4b),	/* theight */
  0,					/* sid     */
  -1,					/* flag    */
  0					/* mask    */
};

/* The CI4 TLUT load */
uObjTxtr objTLUT_CI4 = {
  G_OBJLT_TLUT,				/* type    */
  (u64 *)textureCI4pal,			/* image   */
  GS_PAL_HEAD(0),			/* phead   */
  GS_PAL_NUM(5),			/* pnum    */
  0,					/* zero    */
  0,					/* sid     */
  -1,					/* flag    */
  0					/* mask    */
};

/*---------------------------------------------------------------------------*
 *	The TxSprite structure
 *---------------------------------------------------------------------------*/
uObjTxSprite	objBall[] = {
  /* 1st part of Ball */
  {{  G_OBJLT_TXTRBLOCK,		/* type    */
      (u64 *)ut_gorogoro,		/* image   */
      GS_PIX2TMEM(0,     G_IM_SIZ_8b),	/* tmem    */
      GS_TB_TSIZE(64*32, G_IM_SIZ_8b),	/* tsize   */
      GS_TB_TLINE(64,    G_IM_SIZ_8b),	/* tline   */
      0,				/* sid     */
      -1,				/* flag    */
      0 				/* mask    */      },
   {   0<<2, 1<<10, 64<<5, 0,		/* objX, scaleX, imageW, paddingX */
      32<<2, 1<<10, 32<<5, 0,		/* objY, scaleY, imageH, paddingY */
      GS_PIX2TMEM(64, G_IM_SIZ_8b),	/* imageStride */
      GS_PIX2TMEM( 0, G_IM_SIZ_8b),	/* imageAdrs   */
      G_IM_FMT_CI,			/* imageFmt    */
      G_IM_SIZ_8b,			/* imageSiz    */
      0,				/* imagePal    */
      0					/* imageFlags  */  }},

  /* 2nd part of Ball for PointSample mode */
  {{  G_OBJLT_TXTRBLOCK,		/* type    */
      (u64 *)((u32)ut_gorogoro+64*32),	/* image   */
      GS_PIX2TMEM(0,     G_IM_SIZ_8b),	/* tmem    */
      GS_TB_TSIZE(64*32, G_IM_SIZ_8b),	/* tsize   */
      GS_TB_TLINE(64,    G_IM_SIZ_8b),	/* tline   */
      0,				/* sid     */
      -1,				/* flag    */
      0 				/* mask    */      },
   {   0<<2, 1<<10, 64<<5, 0,		/* objX, scaleX, imageW, paddingX */
      64<<2, 1<<10, 32<<5, 0,		/* objY, scaleY, imageH, paddingY */
      GS_PIX2TMEM(64, G_IM_SIZ_8b),	/* imageStride */
      GS_PIX2TMEM( 0, G_IM_SIZ_8b),	/* imageAdrs   */
      G_IM_FMT_CI,			/* imageFmt    */
      G_IM_SIZ_8b,			/* imageSiz    */
      0,				/* imagePal    */
      0					/* imageFlags  */  }},

  /* 3rd part of Ball for Bilerp mode */
  {{  G_OBJLT_TXTRBLOCK,		/* type    */
      (u64 *)((u32)ut_gorogoro+64*31),	/* image   */
      GS_PIX2TMEM(0,     G_IM_SIZ_8b),	/* tmem    */
      GS_TB_TSIZE(64*32, G_IM_SIZ_8b),	/* tsize   */
      GS_TB_TLINE(64,    G_IM_SIZ_8b),	/* tline   */
      0,				/* sid     */
      -1,				/* flag    */
      0 				/* mask    */      },
   {   0<<2, 1<<10, 64<<5, 0,		/* objX, scaleX, imageW, paddingX */
      63<<2, 1<<10, 32<<5, 0,		/* objY, scaleY, imageH, paddingY */
      GS_PIX2TMEM(64, G_IM_SIZ_8b),	/* imageStride */
      GS_PIX2TMEM( 0, G_IM_SIZ_8b),	/* imageAdrs   */
      G_IM_FMT_CI,			/* imageFmt    */
      G_IM_SIZ_8b,			/* imageSiz    */
      0,				/* imagePal    */
      0					/* imageFlags  */  }}
};

/* The CI8 TLUT load */
uObjTxtr objBallTLUT = {
  G_OBJLT_TLUT,				/* type    */
  (u64 *)utPal_gorogoro,		/* image   */
  GS_PAL_HEAD(0),			/* phead   */
  GS_PAL_NUM(256),			/* pnum    */
  0,					/* zero    */
  0,					/* sid     */
  -1,					/* flag    */
  0					/* mask    */
};

/*======== End of sprite.c ========*/
