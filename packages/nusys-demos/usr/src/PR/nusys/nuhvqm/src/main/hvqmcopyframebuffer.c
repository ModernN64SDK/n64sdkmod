/*
 * hvqmcopyframebuffer.c
 */
#include <nusys.h>
#include "hvqm.h"


/*
 * Allow copying the HVQM area of the previous frame
 */
#if HVQM_PERMIT_OTHER_GRAPHICS == YES

/* Initializing RDP which is for only copy use. */
static Gfx hvqmSetupCopyState[] = {
    gsDPPipelineMode(G_PM_1PRIMITIVE),
    gsDPSetTextureLOD(G_TL_TILE),	/* Multi tile texture */
    gsDPSetTextureLUT(G_TT_NONE),	/* Texture mode */
    gsDPSetTextureDetail(G_TD_CLAMP),	/* Texture sampling type (LOD) */
    gsDPSetTexturePersp(G_TP_NONE),	/* Texture perspective */
    gsDPSetTextureFilter(G_TF_POINT),	/* Texture filter */
    gsDPSetTextureConvert(G_TC_FILT),
    gsDPSetCombineMode(G_CC_DECALRGBA, G_CC_DECALRGBA),
    gsDPSetCombineKey(G_CK_NONE),
    gsDPSetAlphaCompare(G_AC_NONE),
    gsDPSetColorDither(G_CD_DISABLE),
    gsDPSetScissor(G_SC_NON_INTERLACE, 0, 0,640, 480),
    gsDPSetRenderMode(G_RM_SPRITE, G_RM_SPRITE2),
    gsDPSetCycleType(G_CYC_COPY),
    gsSPEndDisplayList(),
};

#if HVQM_USE_SPRITE_UCODE == YES
static u32 hvqmGfxGtaskNo=0;
static Gfx hvqmGfxGlist[2][8];
static Gfx* hvqmGlistp;
static uObjBg hvqmObjBg[2];
static uObjBg* hvqmObjBgPtr;

void hvqmCopyFrameBuffer(HVQM2Header *hvqmHeader, u32 hvqmFlag)
{
    u32 hvqmCfbPrev;		/* The number for the previous frame buffer */

    /* Initialize frame buffer (find the previous buffer) */
    hvqmCfbPrev = (nuGfxCfbCounter+nuGfxCfbNum-1)%nuGfxCfbNum;

    /* Specify the strructure for copy use */
    hvqmObjBgPtr = &hvqmObjBg[hvqmGfxGtaskNo];

    /* Set the starting address of the sorce copy image (Starting address for the previous frame) */
    hvqmObjBgPtr->b.imagePtr  = (u64*)nuGfxCfb[hvqmCfbPrev];
    /* Set the size of the source image (Size of the previous frame) */
    hvqmObjBgPtr->b.imageW    = (HVQM_CFB_WIDTH*HVQM_CFB_FORMAT)<<2;
    hvqmObjBgPtr->b.imageH    = HVQM_CFB_HEIGHT<<2;
    /* Set the location of the source image (Starting offset of HVQM image in the previous frame) */
    hvqmObjBgPtr->b.imageX    = ((hvqmOffset%HVQM_CFB_WIDTH*HVQM_CFB_FORMAT))<<5;
    hvqmObjBgPtr->b.imageY    = (hvqmOffset/HVQM_CFB_WIDTH)<<5;
    /* Set the size of the destination image (Size of HVQM image in the current frame) */
    hvqmObjBgPtr->b.frameW    = (hvqmHeader->width*HVQM_CFB_FORMAT)<<2;
    hvqmObjBgPtr->b.frameH    = hvqmHeader->height<<2;
    /* Set the location of the destination image (Starting offset of HVQM image in the current frame) */
    hvqmObjBgPtr->b.frameX    = ((hvqmOffset%HVQM_CFB_WIDTH)*HVQM_CFB_FORMAT)<<2;
    hvqmObjBgPtr->b.frameY    = (hvqmOffset/HVQM_CFB_WIDTH)<<2;

    /* Other settings */
    hvqmObjBgPtr->b.imageLoad = G_BGLT_LOADTILE;
    hvqmObjBgPtr->b.imageFmt  = G_IM_FMT_RGBA;
    /* Image format (16bit RGBA fixed) */
    hvqmObjBgPtr->b.imageSiz  = G_IM_SIZ_16b;

    hvqmObjBgPtr->b.imagePal  = 0;
    hvqmObjBgPtr->b.imageFlip = 0;

    guS2DInitBg(hvqmObjBgPtr);


    /* Set display list buffer */
    hvqmGlistp = hvqmGfxGlist[hvqmGfxGtaskNo];

    /* Set to initialize RSP segment register */
    gSPSegment(hvqmGlistp++, 0, 0x0);  /* for CPU virtual address */
    /* Initialization settings */
    gSPDisplayList(hvqmGlistp++, OS_K0_TO_PHYSICAL(hvqmSetupCopyState));
    /* !! In COPY mode, 32bit RGBA cannot be used !! */
    /* When 32bit RGBA is used, regarded as 16bit + 16bit, so the width doubles */
    gDPSetColorImage(hvqmGlistp++, G_IM_FMT_RGBA, G_IM_SIZ_16b, HVQM_CFB_WIDTH*HVQM_CFB_FORMAT, osVirtualToPhysical(nuGfxCfb_ptr));
    /* Start copying */
    gSPBgRectCopy(hvqmGlistp++, hvqmObjBgPtr);

    gDPFullSync(hvqmGlistp++);
    gSPEndDisplayList(hvqmGlistp++);

    nuGfxTaskStart(hvqmGfxGlist[hvqmGfxGtaskNo],
		   (s32)(hvqmGlistp-hvqmGfxGlist[hvqmGfxGtaskNo])*sizeof(Gfx),
		   NU_GFX_UCODE_S2DEX, hvqmFlag);

    hvqmGfxGtaskNo ^= 1;
}
#else /* HVQM_USE_SPRITE_UCODE */

static u32 hvqmGfxGtaskNo=0;
static Gfx hvqmGfxGlist[2][1024];
static Gfx* hvqmGlistp;

void hvqmCopyFrameBuffer(HVQM2Header *hvqmHeader, u32 hvqmFlag)
{
    u32 hvqmCfbPrev;	/* Number for the previous frame buffer */
    u32 hvqmLoadNum;	/* Number of times copied */
    u32 hvqmLoadLines;	/* Number of lines to copy at one time */
    u32 hvqmLoadStart;	/* The first line to copy */
    u32 hvqmLoadEnd;	/* The last line to copy */

    /* Initialize frame buffer (Find the previous buffer) */
    hvqmCfbPrev = (nuGfxCfbCounter+nuGfxCfbNum-1)%nuGfxCfbNum;

    /* Calculate the size (number of lines) that can be read to TMEM at one time */
    hvqmLoadLines = 4*1024/(hvqmHeader->width*HVQM_CFB_FORMAT*2);

    /* Set display list buffer */
    hvqmGlistp = hvqmGfxGlist[hvqmGfxGtaskNo];

    /* Initialize RSP segment register */
    gSPSegment(hvqmGlistp++, 0, 0x0);  /* for CPU virtual address */
    /* Initialization settings */
    gSPDisplayList(hvqmGlistp++, OS_K0_TO_PHYSICAL(hvqmSetupCopyState));

    /* !! In COPY mode, 32bit RGBA cannot be used !! */
    /* When 32bit RGBA is used, regarded as 16bit + 16bit, so the width doubles */
    gDPSetColorImage(hvqmGlistp++, G_IM_FMT_RGBA, G_IM_SIZ_16b, HVQM_CFB_WIDTH*HVQM_CFB_FORMAT, osVirtualToPhysical(nuGfxCfb_ptr));

    for(hvqmLoadNum=0, hvqmLoadEnd=0; hvqmLoadEnd != hvqmHeader->height; hvqmLoadNum++)
    {
	/* Set the starting line and the last line to copy */
	hvqmLoadStart = hvqmLoadNum*hvqmLoadLines;
	if( hvqmLoadStart + hvqmLoadLines < hvqmHeader->height )
	    hvqmLoadEnd = hvqmLoadStart+hvqmLoadLines;
	else
	    hvqmLoadEnd = hvqmHeader->height;

	gDPLoadTextureTile( hvqmGlistp++,
		/* Pointer to the previous screen */
		(u32*)nuGfxCfb[hvqmCfbPrev],
		/* Image format (16bit RGBA fixed) */
		G_IM_FMT_RGBA, G_IM_SIZ_16b,
		/* Size of the previous screen */
		HVQM_CFB_WIDTH*HVQM_CFB_FORMAT,
		HVQM_CFB_HEIGHT,
		/* Offset of the previous screen (upper left) */
		(hvqmOffset%HVQM_CFB_WIDTH)*HVQM_CFB_FORMAT,
		hvqmOffset/HVQM_CFB_WIDTH + hvqmLoadStart,
		/* Offset of the previous screen (lower right) */
		(hvqmOffset%HVQM_CFB_WIDTH + hvqmHeader->width - 1)*HVQM_CFB_FORMAT,
		hvqmOffset/HVQM_CFB_WIDTH + hvqmLoadEnd - 1,
		0, G_TX_WRAP, G_TX_WRAP,
		0, 0, G_TX_NOLOD, G_TX_NOLOD );

	gSPTextureRectangle( hvqmGlistp++,
		/* Offset of the current screen (upper left) */
		((hvqmOffset%HVQM_CFB_WIDTH)*HVQM_CFB_FORMAT)<<2,
		(hvqmOffset/HVQM_CFB_WIDTH + hvqmLoadStart)<<2,
		/* Offset of the current screen (lower right) */
		((hvqmOffset%HVQM_CFB_WIDTH + hvqmHeader->width - 1)*HVQM_CFB_FORMAT)<<2,
		(hvqmOffset/HVQM_CFB_WIDTH + hvqmLoadEnd - 1)<<2,
		/* Index of tile descriptor */
		G_TX_RENDERTILE,
		/* Offset of the previous screen (upper left) */
		((hvqmOffset%HVQM_CFB_WIDTH)*HVQM_CFB_FORMAT)<<5,
		(hvqmOffset/HVQM_CFB_WIDTH + hvqmLoadStart)<<5,
		/* scaling factor(COPY mode) */
		4<<10, 1<<10 );
    }

    gDPFullSync(hvqmGlistp++);
    gSPEndDisplayList(hvqmGlistp++);

    nuGfxTaskStart(hvqmGfxGlist[hvqmGfxGtaskNo],
		   (s32)(hvqmGlistp-hvqmGfxGlist[hvqmGfxGtaskNo])*sizeof(Gfx),
		   NU_GFX_UCODE_F3DEX, hvqmFlag);

    hvqmGfxGtaskNo ^= 1;
}
#endif /* HVQM_USE_SPRITE_UCODE */
#endif /* HVQM_PERMIT_OTHER_GRAPHICS */
