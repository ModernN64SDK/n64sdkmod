/*======================================================================*/
/*		NuSYS high resolution sample				*/
/*		graphics.c						*/
/*									*/
/*		Copyright (C) 1997, NINTENDO Co,Ltd.			*/
/* 97/12/08	Created by Kensaku Ohki(SLANP)				*/
/*======================================================================*/
#include <nusys.h>
#include <PR/gs2dex.h>
#include "localdef.h"

#define SCREEN_SIZE_LOW		0
#define SCREEN_SIZE_HIGH	1
#define GFX_LIST_NUM		4
#define GFX_LIST_SIZE		2048

NUContData contData[4];

typedef struct st_Display {
    u32	width;
    u32	high;
}  ScreenSize;

ScreenSize screen[] = {
    { 320, 240},
    { 640, 480}
};


Gfx rdpInit_dl[] = {
    gsDPPipeSync(),
    gsDPPipelineMode(G_PM_1PRIMITIVE),
    gsDPSetTextureLOD(G_TL_TILE),
    gsDPSetTextureLUT(G_TT_NONE),
    gsDPSetTextureDetail(G_TD_CLAMP),
    gsDPSetTexturePersp(G_TP_NONE),
    gsDPSetTextureFilter(G_TF_BILERP),
    gsDPSetTextureConvert(G_TC_FILT),
    gsDPSetCombineMode(G_CC_SHADE, G_CC_SHADE),
    gsDPSetCombineKey(G_CK_NONE),
    gsDPSetAlphaCompare(G_AC_NONE),
    gsDPSetColorDither(G_CD_DISABLE),
    gsDPSetScissor(G_SC_NON_INTERLACE, 0, 0,640, 480),
    gsSPEndDisplayList(),
};


Gfx	gfxListBuf[GFX_LIST_NUM][GFX_LIST_SIZE];
u32	gfxListCnt = 0;
Gfx*	gfxListPtr;
Gfx*	gfxListStartPtr;
extern u32 mainNo;


/*----------------------------------------------------------------------*/
/*	Clear of frame buffer   					*/
/*	IN:	**glist_ptr						*/
/*		screen		Screen size     			*/
/*			SCREEN_SIZE_LOW	 0				*/
/*			SCREEN_SIZE_HIGH 1				*/
/*	RET:	None							*/
/*----------------------------------------------------------------------*/
void gfxClearCfb(u32 size)
{
    ScreenSize* screenPtr;
    
    screenPtr = &screen[size];

    gSPSegment(gfxListPtr++, 0, 0x0);

    gDPSetCycleType(gfxListPtr++, G_CYC_FILL);
    
    /* Clear Z-buffer */
    gDPSetDepthImage(gfxListPtr++, OS_K0_TO_PHYSICAL(nuGfxZBuffer));
    gDPSetColorImage(gfxListPtr++, G_IM_FMT_RGBA, G_IM_SIZ_16b,
		     screenPtr->width,
		     OS_K0_TO_PHYSICAL(nuGfxZBuffer));
    gDPSetFillColor(gfxListPtr++,(GPACK_ZDZ(G_MAXFBZ,0) << 16 |
				  GPACK_ZDZ(G_MAXFBZ,0)));
    gDPFillRectangle(gfxListPtr++, 0, 0, screen->width-1, screenPtr->high-1);

    gDPPipeSync(gfxListPtr++);


    gDPSetColorImage(gfxListPtr++, G_IM_FMT_RGBA, G_IM_SIZ_16b,
		     screenPtr->width,
		     OS_K0_TO_PHYSICAL(nuGfxCfb_ptr)); 
    gDPSetFillColor(gfxListPtr++, (GPACK_RGBA5551(0, 0, 0, 1) << 16
				     | GPACK_RGBA5551(0, 0, 0, 1)));
    gDPFillRectangle(gfxListPtr++, 0, 0,
		     screenPtr->width-1, screenPtr->high - 1);
    gDPPipeSync(gfxListPtr++);

}

/*----------------------------------------------------------------------*/
/*	Change display list buffer      				*/
/*	IN:	None							*/
/*	RET:	Pointer of display list buffer  			*/
/*----------------------------------------------------------------------*/
void gfxListBufferChange(void)
{
    gfxListStartPtr = &gfxListBuf[gfxListCnt][0];
    gfxListPtr = gfxListStartPtr;
    gfxListCnt++;
    gfxListCnt %= GFX_LIST_NUM;
    return;
}
