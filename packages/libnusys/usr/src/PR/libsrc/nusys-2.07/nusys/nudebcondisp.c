/*======================================================================*/
/*		NuSYS							*/
/*		nudebconsole_ex2.c					*/
/*									*/
/*		Copyright (C) 1997, NINTENDO Co,Ltd.			*/
/*									*/
/*----------------------------------------------------------------------*/
/*      $Id: nudebcondisp.c,v 1.8 1999/07/09 06:16:57 ohki Exp $	*/
/*======================================================================*/
#define	F3DEX_GBI
#include <nusys.h>

#define RESOLUTION_LOW	0
#define RESOLUTION_HIGH	1

extern unsigned char nuFont[];
extern unsigned char nuFont2[];

//* Display list that sets the RDP for character rendering */
/* The size of the display list buffer for console display is */
/* the maximum number of characters displayed in a window  */		
/* * the number of windows * 3.  This is because gSPTextureRectangle */
/* requires 3 buffers for each window. */

static Gfx	conGlistBuf[2][NU_DEB_CON_TEXT_SIZE*NU_DEB_CON_WINDOW_NUM*3];
static u32	conGlistCnt = 0;

#define	G_CC_TEXT	0, 0, 0, PRIMITIVE, 0, 0, 0, TEXEL0

static Gfx consoleInitDL[] = {
    gsSPSegment(0x0, 0x0),
    gsDPSetCycleType(G_CYC_1CYCLE),
    gsDPSetScissor(G_SC_NON_INTERLACE, 0, 0, NU_GFX_INIT_SCREEN_WD * 2, NU_GFX_INIT_SCREEN_HT * 2),
    gsDPSetTextureFilter(G_TF_POINT),
    gsDPSetCombineMode(G_CC_TEXT, G_CC_TEXT),
    gsDPSetRenderMode(G_RM_XLU_SURF, G_RM_XLU_SURF),
    gsDPSetTexturePersp(G_TP_NONE),
    gsDPSetTextureLOD(G_TL_TILE),
    gsDPSetTextureLUT(G_TT_NONE),
    gsDPSetTextureDetail(G_TD_CLAMP),
    gsDPSetTextureConvert(G_TC_FILT),
    gsDPSetAlphaCompare(G_AC_NONE),
    gsSPTexture(0xffff, 0xffff, 0, G_TX_RENDERTILE, G_ON),
    gsDPSetPrimColor(0, 0, 255, 255, 255, 0),
    gsSPClearGeometryMode(0xffffffff),

    gsDPLoadTextureBlock_4b(nuFont, G_IM_FMT_I,
			    144, 56,			    
			    0,
			    G_TX_WRAP , G_TX_WRAP,
			    G_TX_NOMASK, G_TX_NOMASK,
			    G_TX_NOLOD, G_TX_NOLOD),
    
    gsDPPipeSync(),
    gsSPEndDisplayList(),
};

static u8 colorTbl[][4] = {
    {   0,   0,   0, 255},	/* Black	*/
    {   0,   0, 255, 255},	/* Blue 	*/
    {   0, 255,   0, 255},	/* Green	*/
    {   0, 255, 255, 255},	/* Cyan*/
    { 255,   0,   0, 255},	/* Red	*/
    { 255,   0, 255, 255},	/* Magenta */
    { 255, 255,   0, 255},	/* Yellow 	*/
    { 255, 255, 255, 255},	/* White 	*/
    { 128, 128, 128, 255},	/* Black	*/
    { 128, 128, 255, 255},	/* Blue 	*/
    { 128, 255, 128, 255},	/* Green	*/
    { 128, 255, 255, 255},	/* Cyan*/
    { 255, 128, 128, 255},	/* Red	*/
    { 255, 128, 255, 255},	/* Magenta */
    { 255, 255, 128, 255},	/* Yellow 	*/
    { 255, 255, 255, 255},	/* White 	*/
};



/*----------------------------------------------------------------------*/
/*	conWindowDisp - Console display				*/
/* 	Creates the display list that displays the console.		*/
/*	IN:	glist_ptr		Display list buffer 		*/
/*		conWin		Console window structure		*/
/*		resolution	Resolution			*/
/*	RET:	nothing						*/
/*----------------------------------------------------------------------*/
static void conWindowDisp(Gfx** glistP, NUDebConWindow* conWin, u32 resolution)
{
    u32		indexY;
    u32		cntX, cntY;
    u32		color = 0xFF;
    u32		atrCol;
    u32		textX, textY;
    u32		textIdx;
    u32		sl, tl;
    u16*	text;
    u16		code;
    u16		attr;
    u8*		fontPtr;
    u8*		loadFont = NULL;
    
/* Taking into account the scroll value, calculate the starting position of the display characters. */
    text = conWin->text;
    textIdx = (conWin->scroll * conWin->winW);
    indexY = conWin->scroll;
    
 /* Create the display list that displays the console window. */
    for(cntY = 0; cntY < conWin->winH; cntY++){
	for(cntX = 0; cntX < conWin->winW; cntX++){
	    
	    code = (text[textIdx + cntX] & 0x00ff);	/* Character code */
	    
	    if(code){
		atrCol = (text[textIdx + cntX] >> 8) & 0x0f;	/* Color code */
		attr   = text[textIdx + cntX] >> 12;		/* Attribute */

		/* Blink */
		if((attr & NU_DEB_CON_ATTR_BLINK)
		   && (nuScRetraceCounter & 0x20)){
		    continue;
		}
		    
		/* Highlight */
		if(attr & NU_DEB_CON_ATTR_REVERSE){
		    fontPtr = nuFont2;
		} else {
		    fontPtr = nuFont;
		}
		if(loadFont != fontPtr){
		    gDPLoadTextureBlock_4b((*glistP)++,
					   fontPtr, G_IM_FMT_I,
					   144, 56,    
					   0,
					   G_TX_WRAP , G_TX_WRAP,
					   G_TX_NOMASK, G_TX_NOMASK,
					   G_TX_NOLOD, G_TX_NOLOD);
		    loadFont = fontPtr;
		}
		
		/* Change colors when not the current character color. */
		if(color != atrCol){
		    color = atrCol;
		    gDPSetPrimColor((*glistP)++, 0, 0,
				    colorTbl[color][0],
				    colorTbl[color][1],
				    colorTbl[color][2],
				    colorTbl[color][3]);
		    gDPPipeSync((*glistP)++);
		}

		tl = (((code & 0xf0) >>4 ) * 9 )<<5;
		sl = ((code & 0x0f) * 9) << 5;

		if(resolution == RESOLUTION_LOW){
		    /* High resolution */
		    textX = cntX * 8 + conWin->winX;	/* x-coordinate of character display */
		    textY = cntY * 8 + conWin->winY;	/* y-coordinate of character display */
		    gSPTextureRectangle((*glistP)++,
					textX << 2, textY << 2,
					(textX + 9) << 2, (textY + 9) << 2,
					G_TX_RENDERTILE,
					sl, tl,
					1<<10, 1<<10);

		} else {
		    /* high resolution */
		    textX = cntX * 16 + conWin->winX;/* x-coordinate of character display */
		    textY = cntY * 16 + conWin->winY;/* y-coordinate of character display */
		    gSPTextureRectangle((*glistP)++,
					textX << 2, textY << 2,
					(textX + 16) << 2, (textY + 16) << 2,
					G_TX_RENDERTILE,
					sl, tl,
					1<<9, 1<<9);
		}

	    }
	}
	indexY++;
	indexY %= conWin->winH;
	textIdx = indexY * conWin->winW;
    }
}

/*----------------------------------------------------------------------*/
/*	nuDebConDisp - Console display					*/
/* 	Displays the console.						*/
/*	IN:	glist_ptr	Display list buffer		*/
/*	RET:	nothing							*/
/*----------------------------------------------------------------------*/
void nuDebConDisp(u32 flag)
{
    u32		cnt;
    Gfx*	conGlistPtr;
    u32		viMode;
    u32		resolution;
    Gfx*	glistCheckPtr;

    /* Obtain the current VI mode.*/
    viMode = osViGetCurrentMode();

    conGlistPtr = conGlistBuf[conGlistCnt];

    /* Set RDP for character rendering. */
    gSPDisplayList(conGlistPtr++, consoleInitDL);

/* VI mode of 0-7 is low resolution; */
/* 8-13 is high resolution. 		*/
    if(viMode < 8){
	resolution = RESOLUTION_LOW;
	gDPSetColorImage(conGlistPtr++, G_IM_FMT_RGBA, G_IM_SIZ_16b,
			 NU_DEB_CON_WIDTH,
			 osVirtualToPhysical(nuGfxCfb_ptr));
    } else if(viMode < 14){
	gDPSetColorImage(conGlistPtr++, G_IM_FMT_RGBA, G_IM_SIZ_16b,
			 NU_DEB_CON_WIDTH*2,
			 osVirtualToPhysical(nuGfxCfb_ptr));
	resolution = RESOLUTION_HIGH;
    } else {
	/* Unsupported VI mode */
	return;
    }

    glistCheckPtr = conGlistPtr;
/* Display characters in windows with display set to ON. */
   
for(cnt = 0; cnt < NU_DEB_CON_WINDOW_NUM; cnt++){
	/* Display only when window display enabled. */
	if(nuDebConWin[cnt].windowFlag){
	    conWindowDisp(&conGlistPtr, &nuDebConWin[cnt], resolution);
	}
    }
    if((glistCheckPtr == conGlistPtr) && (!(flag & NU_SC_SWAPBUFFER))) return;

    gDPFullSync(conGlistPtr++);
    gSPEndDisplayList(conGlistPtr++);

#ifdef NU_DEBUG
    if((u32)(conGlistPtr - conGlistBuf[conGlistCnt])
       > (NU_DEB_CON_TEXT_SIZE * NU_DEB_CON_WINDOW_NUM * 3)){
	osSyncPrintf("nuDebConDisp: gfx list buffer over.\n");
    }
#endif /* NU_DEBUG */
    
	/* Start the graphics task. */
    nuGfxTaskStart(conGlistBuf[conGlistCnt],
		   (conGlistPtr - conGlistBuf[conGlistCnt]) * sizeof(Gfx),
		   NU_GFX_UCODE_F3DEX , flag);
    conGlistCnt++;
    conGlistCnt &= 0x01;
}
