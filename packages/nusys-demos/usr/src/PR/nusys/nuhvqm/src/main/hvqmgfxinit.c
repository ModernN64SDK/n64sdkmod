/*
 * hvqmGfxInit.c
 *     This file is a copy of nugfxinit_ex2.c with several changes.
 */
/* Should already be defined, so corrected so as not to redefine */
#ifndef F3DEX_GBI_2
#define	F3DEX_GBI_2
#endif /* F3DEX_GBI_2 */
#include <nusys.h>
/* Changed to include hvqm.h */
#include "hvqm.h"
/* Added hvqm microcode external references */
#if HVQM_CFB_FORMAT == 1
extern  u8  hvqm2sp1TextStart[], hvqm2sp1TextEnd[];
extern  u8  hvqm2sp1DataStart[], hvqm2sp1DataEnd[];
#else /* HVQM_CFB_FORMAT */
extern  u8  hvqm2sp2TextStart[], hvqm2sp2TextEnd[];
extern  u8  hvqm2sp2DataStart[], hvqm2sp2DataEnd[];
#endif /* HVQM_CFB_FORMAT */

/*------------------------------------------------------*/
/*  Graphic microcode to be registered.			*/
/*  If you modify the microcode, please make     	*/
/*  additions and changes here.		        	*/
/*------------------------------------------------------*/    
static NUUcode nugfx_ucode[] = {
    {(u64*)gspF3DEX2_fifoTextStart, (u64*)gspF3DEX2_fifoDataStart},	/*0*/
    {(u64*)gspF3DEX2_NoN_fifoTextStart, (u64*)gspF3DEX2_NoN_fifoDataStart},/*1*/
    {(u64*)gspF3DEX2_Rej_fifoTextStart, (u64*)gspF3DEX2_Rej_fifoDataStart},/*2*/
    {(u64*)gspF3DLX2_Rej_fifoTextStart, (u64*)gspF3DLX2_Rej_fifoDataStart},/*3*/
    {(u64*)gspL3DEX2_fifoTextStart, (u64*)gspL3DEX2_fifoDataStart},	/*4*/
    {(u64*)gspS2DEX2_fifoTextStart, (u64*)gspS2DEX2_fifoDataStart},	/*5*/
    /* Added the next line */
#if HVQM_CFB_FORMAT == 1
    {(u64*)hvqm2sp1TextStart, (u64*)hvqm2sp1DataStart},	/*6*/
#else /* HVQM_CFB_FORMAT */
    {(u64*)hvqm2sp2TextStart, (u64*)hvqm2sp2DataStart},	/*6*/
#endif /* HVQM_CFB_FORMAT */
};

/* Changed so that the size of frame buffer will be reserved according to HVQM_CFB_FORMAT */
#define HVQM_GFX_FRAMEBUFFER_ADDR	(0x80400000-HVQM_CFB_WIDTH*HVQM_CFB_HEIGHT*HVQM_CFB_FORMAT*2*NU_GFX_FRAMEBUFFER_NUM)
#define HVQM_GFX_FRAMEBUFFER0_ADDR	(HVQM_GFX_FRAMEBUFFER_ADDR)
#define HVQM_GFX_FRAMEBUFFER1_ADDR	(HVQM_GFX_FRAMEBUFFER0_ADDR+HVQM_CFB_WIDTH*HVQM_CFB_HEIGHT*HVQM_CFB_FORMAT*2)
#define HVQM_GFX_FRAMEBUFFER2_ADDR	(HVQM_GFX_FRAMEBUFFER1_ADDR+HVQM_CFB_WIDTH*HVQM_CFB_HEIGHT*HVQM_CFB_FORMAT*2)

#if HVQM_CFB_FORMAT == 1
static u16*	FrameBuf[3] = {
    (u16*)HVQM_GFX_FRAMEBUFFER0_ADDR,
    (u16*)HVQM_GFX_FRAMEBUFFER1_ADDR,
    (u16*)HVQM_GFX_FRAMEBUFFER2_ADDR,
};
#else /* HVQM_CFB_FORMAT */
static u32*	FrameBuf[3] = {
    (u32*)HVQM_GFX_FRAMEBUFFER0_ADDR,
    (u32*)HVQM_GFX_FRAMEBUFFER1_ADDR,
    (u32*)HVQM_GFX_FRAMEBUFFER2_ADDR,
};
#endif /* HVQM_CFB_FORMAT */

/*------------------------------------------------------*/
/*  Display list first time RDP is initialized		*/
/*------------------------------------------------------*/
static Gfx rdpstateinit_dl[] = {

    /* set all of the attribute registers to zero */
    gsDPSetEnvColor(0,0,0,0),
    gsDPSetPrimColor(0,0,0,0,0,0),
    gsDPSetBlendColor(0,0,0,0),
    gsDPSetFogColor(0,0,0,0),
    gsDPSetFillColor(0),
    gsDPSetPrimDepth(0,0),
    gsDPSetConvert(0,0,0,0,0,0),
    gsDPSetKeyR(0,0,0),
    gsDPSetKeyGB(0,0,0,0,0,0),

    /* set combine mode */
    gsDPSetCombineMode(G_CC_SHADE, G_CC_SHADE),

    /* initialize the scissor box */
    gsDPSetScissor(G_SC_NON_INTERLACE, 0, 0,
		   NU_GFX_INIT_SCREEN_WD, NU_GFX_INIT_SCREEN_HT),

    /* initialize all the texture tile descriptors to zero */
    gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
    gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0),
    gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0),
    gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0),
    gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0),
    gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0),
    gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0),
    gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0),

    gsDPSetTileSize(0, 0, 0, (1 - 1) << G_TEXTURE_IMAGE_FRAC, (1 - 1) <<
		G_TEXTURE_IMAGE_FRAC),
    gsDPSetTileSize(1, 0, 0, (1 - 1) << G_TEXTURE_IMAGE_FRAC, (1 - 1) <<
		G_TEXTURE_IMAGE_FRAC),
    gsDPSetTileSize(2, 0, 0, (1 - 1) << G_TEXTURE_IMAGE_FRAC, (1 - 1) <<
		G_TEXTURE_IMAGE_FRAC),
    gsDPSetTileSize(3, 0, 0, (1 - 1) << G_TEXTURE_IMAGE_FRAC, (1 - 1) <<
		G_TEXTURE_IMAGE_FRAC),
    gsDPSetTileSize(4, 0, 0, (1 - 1) << G_TEXTURE_IMAGE_FRAC, (1 - 1) <<
		G_TEXTURE_IMAGE_FRAC),
    gsDPSetTileSize(5, 0, 0, (1 - 1) << G_TEXTURE_IMAGE_FRAC, (1 - 1) <<
		G_TEXTURE_IMAGE_FRAC),
    gsDPSetTileSize(6, 0, 0, (1 - 1) << G_TEXTURE_IMAGE_FRAC, (1 - 1) <<
		G_TEXTURE_IMAGE_FRAC),
    gsDPSetTileSize(7, 0, 0, (1 - 1) << G_TEXTURE_IMAGE_FRAC, (1 - 1) <<
		G_TEXTURE_IMAGE_FRAC),
    gsDPPipeSync(),
    gsSPEndDisplayList(),
};


/* Name of function changed */
/*----------------------------------------------------------------------*/
/*	hvqmGfxInit - Initilize graphics thread, manager		*/
/*	IN:	Nothing special						*/
/*	RET:	None							*/
/*----------------------------------------------------------------------*/
void hvqmGfxInit(void)
{
    Gfx		gfxList[0x100];
    Gfx*	gfxList_ptr;
    

    /* Start graphics thread */
    nuGfxThreadStart();

    /* Cast because nuGfxSetCfb expects 16-bit frame buffer. */
    nuGfxSetCfb((u16**)FrameBuf, 3);		/* Register frame buffer */
    nuGfxSetZBuffer((u16*)NU_GFX_ZBUFFER_ADDR);

    /* Register callback function to swap buffers */
    nuGfxSwapCfbFuncSet(nuGfxSwapCfb);

    /* Set FIFO buffer for FIFO microcode     */
    nuGfxSetUcodeFifo(nuRDPOutputBuf, NU_GFX_RDP_OUTPUTBUFF_SIZE);

    /* Register graphics microcode */
    nuGfxSetUcode(nugfx_ucode);
    /* Initialize graphics task manager	*/
    nuGfxTaskMgrInit();

    /* RDP only needs to be initialized once at the start.  Here is where it is done. */
    gfxList_ptr = gfxList;
    gSPDisplayList(gfxList_ptr++, OS_K0_TO_PHYSICAL(rdpstateinit_dl));
    gDPFullSync(gfxList_ptr++);
    gSPEndDisplayList(gfxList_ptr++);
    nuGfxTaskStart(gfxList,
		   (s32)(gfxList_ptr - gfxList) * sizeof (Gfx),
		   NU_GFX_UCODE_F3DEX , 0);

    /* Wait for task to end */
    nuGfxTaskAllEndWait();
}
