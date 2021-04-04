/*======================================================================*/
/*		NuSYS high resolution sample			              	*/
/*		graphics.c					                    	*/
/*								                   	*/
/*		Copyright (C) 1997, NINTENDO Co,Ltd.		      	*/
/*          97/12/08	Created by Kensaku Ohki(SLANP)			*/
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

uObjBg	objBgH = {
    0, 640<<2, 0, 640<<2,
    0, 480<<2, 0, 480<<2,
    (u64*)NULL,
    G_BGLT_LOADTILE,              /* imageLoad                      */
    G_IM_FMT_RGBA,                /* imageFmt                       */
    G_IM_SIZ_16b,                 /* imageSiz                       */
    0,                            /* imagePal                       */
    0                             /* imageFlip                      */
};
uObjBg	objBgL = {
    0, 320<<2, 0, 320<<2,
    0, 240<<2, 0, 240<<2,
    (u64*)NULL,
    G_BGLT_LOADTILE,              /* imageLoad                      */
    G_IM_FMT_RGBA,                /* imageFmt                       */
    G_IM_SIZ_16b,                 /* imageSiz                       */
    0,                            /* imagePal                       */
    0                             /* imageFlip                      */
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

Gfx spriteInit_dl[] = {
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

Gfx	gfxListBuf[GFX_LIST_NUM][GFX_LIST_SIZE];
u32	gfxListCnt = 0;
Gfx*	gfxListPtr;
Gfx*	gfxListStartPtr;
u32 	hireso;
extern u32 mainNo;


/*----------------------------------------------------------------------*/
/*	Clear the frame buffer. 					            */
/*	IN:	**glist_ptr						                  */
/*		screen		Screen size 		              	*/
/*			SCREEN_SIZE_LOW	 0				            */
/*			SCREEN_SIZE_HIGH 1		              		*/
/*	RET:	None						                   	*/
/*----------------------------------------------------------------------*/
void gfxClearCfb(u32 size)
{
    ScreenSize* screenPtr;
    
    screenPtr = &screen[size];

    gSPSegment(gfxListPtr++, 0, 0x0);

    gDPSetCycleType(gfxListPtr++, G_CYC_FILL);
    
    /* Clear the Z buffer. */
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
/*	Change the display list buffer. 		            		*/
/*	IN:	None						                    	*/
/*	RET:	Pointer to the display list buffer.           			*/
/*----------------------------------------------------------------------*/
void gfxListBufferChange(void)
{
    gfxListStartPtr = &gfxListBuf[gfxListCnt][0];
    gfxListPtr = gfxListStartPtr;
    gfxListCnt++;
    gfxListCnt %= GFX_LIST_NUM;
    return;
}



/*----------------------------------------------------------------------*/
/*	High resolution display						      	*/
/*	IN:	taskNum	Number of tasks remaining				*/
/*	RET:	None						                  	*/
/*----------------------------------------------------------------------*/
void graphic(u32 taskNum)
{

    uObjBg*	objBgPtr;
    
    /* Do not process if the tasks are not finished. */ 
    if(nuGfxTaskSpool) return;
    
    /* Set the display list buffer. */
    gfxListBufferChange();
    gSPDisplayList(gfxListPtr++, rdpInit_dl);
    /* Clear the screen. */
    gfxClearCfb(hireso);

    /* end top-level display list */
    gDPFullSync(gfxListPtr++);
    gSPEndDisplayList(gfxListPtr++);
    
    nuGfxTaskStart(gfxListStartPtr, gfxListPtr - gfxListStartPtr,
		   NU_GFX_UCODE_S2DEX,NU_SC_NOSWAPBUFFER);

    gfxListBufferChange();
    
    gSPDisplayList(gfxListPtr++, rdpInit_dl);
    
    gSPDisplayList(gfxListPtr++, spriteInit_dl);
    
    gDPSetRenderMode(gfxListPtr++, G_RM_SPRITE, G_RM_SPRITE2);
    gDPSetCycleType(gfxListPtr++, G_CYC_COPY);
    gDPSetTextureFilter(gfxListPtr++, G_TF_POINT);
    
    if(hireso == 0){
	objBgPtr = &objBgL;

    } else {
	objBgPtr = &objBgH;
    }
    objBgPtr->b.imagePtr = (u64*)_codeSegmentBssEnd;
    guS2DInitBg(objBgPtr);

    objBgPtr->s.frameX = 1<<2;
    objBgPtr->s.frameY = 1<<2;
    gSPBgRectCopy(gfxListPtr++, objBgPtr);
    
    /* end top-level display list */
    gDPFullSync(gfxListPtr++);
    gSPEndDisplayList(gfxListPtr++);
    
    nuGfxTaskStart(gfxListStartPtr, gfxListPtr - gfxListStartPtr,
		   NU_GFX_UCODE_S2DEX,NU_SC_NOSWAPBUFFER);

    gfxListBufferChange();

    nuDebConDisp(NU_SC_NOSWAPBUFFER);
    nuDebTaskPerfBar1(1, 210*(hireso+1), NU_SC_SWAPBUFFER);
    
    /* Read the controller data. */
    nuContDataGetExAll(contData);
    if(contData[0].trigger & A_BUTTON){

	nuGfxFuncRemove();
		    
	if(hireso == 0){
	    mainNo = 0;
	} else {
	    mainNo = 1;
	}
    }

}
