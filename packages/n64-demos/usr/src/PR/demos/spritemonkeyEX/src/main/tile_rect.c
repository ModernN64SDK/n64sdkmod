/*
  tile_rect.c
    Copyright (C) 1999, NINTENDO Co,Ltd., MONEGI CORPORATION.
    Jan. 28, 1999.
*/

#include <ultra64.h>
#include <PR/gs2dex.h>

#include "Tile_Rect.h"

static Gfx bg_setup_RCP_dl[] =
{
    gsDPPipeSync(),
    gsDPSetTexturePersp(G_TP_NONE),
    gsDPSetTextureLOD(G_TL_TILE),
    gsDPSetTextureLUT(G_TT_NONE),
    gsDPSetTextureConvert(G_TC_FILT),
    gsDPSetAlphaCompare(G_AC_THRESHOLD),
    gsDPSetBlendColor(0, 0, 0, 0x01),
    gsDPSetCombineMode(G_CC_DECALRGBA, G_CC_DECALRGBA),
    gsSPEndDisplayList(),
};

Gfx *setup_rsprdp( Gfx *glistp )
{
   gSPDisplayList( glistp++ ,bg_setup_RCP_dl );
   gDPPipeSync(glistp++);
    return glistp;
}

Gfx *tile_rectangle( Gfx *glistp, Rectangle *rect )
{
	static uObjBg	bg = {
		0, 320<<2, 0<<2, 320<<2,	/* imageX, imageW, frameX, frameW */
		0, 240<<2, 0<<2, 240<<2,	/* imageY, imageH, frameY, frameH */
		0,							/* imagePtr                       */
		G_BGLT_LOADTILE,			/* imageLoad                      */
		G_IM_FMT_RGBA,				/* imageFmt                       */
		G_IM_SIZ_16b,				/* imageSiz                       */
		0,							/* imagePal                       */
		0							/* imageFlip                      */
	};

	/* use guS2DEmuBgRect1Cyc for G_IM_SIZ_32b */
	if((G_IM_SIZ_32b == rect->size) && (0 == giDrawMode)) {
		giDrawMode = 1;
#ifdef	_DEBUG
		osSyncPrintf("use guS2DEmuBgRect1Cyc for G_IM_SIZ_32b\n");
#endif	/* _DEBUG */
	}
    gDPSetRenderMode(glistp++, G_RM_SPRITE, G_RM_SPRITE2);
    gDPSetCycleType(glistp++, G_CYC_1CYCLE);
    gDPSetTextureFilter(glistp++, G_TF_POINT);

	bg.s.imageX = (u16)((rect->scrollx) << 5);
	bg.s.imageY = (u16)((rect->scrolly) << 5);
	bg.s.imageW = (u16)((rect->width) << 2);
	bg.s.imageH = (u16)((rect->height) << 2);

	bg.s.frameX = (s16)((rect->sclx) << 2);
	bg.s.frameY = (s16)((rect->scly) << 2);
	bg.s.frameW = (u16)(((u16)rect->sc_width) << 2);
	bg.s.frameH = (u16)(((u16)rect->sc_height) << 2);

    bg.s.imagePtr = (u64 *)rect->timg;
	bg.s.imageFmt = rect->fmt;
	bg.s.imageSiz = rect->size;

    if(0 != rect->tlut) {
		gDPSetTextureLUT( glistp++ , G_TT_RGBA16 );
		gDPLoadSync( glistp++ );
		gSPObjLoadTxtr(glistp ++, rect->tlut);
    	gDPPipeSync( glistp++ );
    }

	bg.s.imageFlip = (unsigned short)((0 != rect->flipx) ? G_BG_FLAG_FLIPS : 0);
	bg.s.scaleW = (unsigned short)(4096.0f / rect->scalex);
	bg.s.scaleH = (unsigned short)(4096.0f / rect->scaley);
	bg.s.imageYorig = bg.s.imageY;
	if(0 == giDrawMode) {
	    gSPBgRect1Cyc(glistp++, &bg);
	}
	else {
		guS2DEmuSetScissor(0, 0, 320, 240, 1);
		guS2DEmuBgRect1Cyc(&glistp,&bg);
	}
    gDPPipeSync( glistp++ );
    return glistp;
}
