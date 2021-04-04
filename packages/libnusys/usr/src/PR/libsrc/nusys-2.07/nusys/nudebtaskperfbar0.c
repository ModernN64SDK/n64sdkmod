/*======================================================================*/
/*		NuSYS							*/
/*		nudebtaskperfbar0.c					*/
/*									*/
/*		Copyright (C) 1997, NINTENDO Co,Ltd.			*/
/*									*/
/*----------------------------------------------------------------------*/    
/* Ver 1.0	97/10/9		Created by Kensaku Ohki(SLANP)		*/
/*======================================================================*/
/* $Id: nudebtaskperfbar0.c,v 1.6 1999/07/09 06:17:10 ohki Exp $	*/
/*======================================================================*/
#ifdef NU_DEBUG
#ifndef	F3DEX_GBI_2
#define	F3DEX_GBI
#endif	/* F3DEX_GBI_2*/

#include <nusys.h>

#define	VFRAME		140
#define	BAR_WIDTH	3

#define X0		1
#define Y0		1

#define X1		(X0+VFRAME*1)
#define X2		(X0+VFRAME*2)
#define X3		(X0+VFRAME*3)
#define X4		(X0+VFRAME*4)
#define X5		(X0+VFRAME*5)
#define X6		(X0+VFRAME*6)
#define X7		(X0+VFRAME*7)


#define Y1		(Y0+4*1)
#define	Y2		(Y0+4*2)
#define	Y3		(Y0+4*3+2)


#define	FRAME_X0	(X0-1)
#define	FRAME_X1	(X7+1)
#define	FRAME_Y0	(Y0-1)
#define	FRAME_Y1	(Y3+1)

#define MARK_SIZE_X	8
#define MARK_SIZE_Y	12



extern unsigned char nuDebPerfMarker[];
extern unsigned short nuDebPerfMarkertlut[];

/* The vertex list of frames of the performance bar */
static Vtx perfFrameVtx[] = {
    { FRAME_X0, FRAME_Y0,   0,   0,   0,   0, 0xff, 0xff, 0xff, 0x80},
    { FRAME_X1, FRAME_Y0,   0,   0,   0,   0, 0xff, 0xff, 0xff, 0x80},
    { FRAME_X1, FRAME_Y1,   0,   0,   0,   0, 0xff, 0xff, 0xff, 0x80},
    { FRAME_X0, FRAME_Y1,   0,   0,   0,   0, 0xff, 0xff, 0xff, 0x80},
    
    {   X0, Y0,   0,   0,   0,   0, 0x00, 0x00, 0x40, 0x80},
    {   X1, Y0,   0,   0,   0,   0, 0x00, 0x00, 0x40, 0x80},
    {   X1, Y3,   0,   0,   0,   0, 0x00, 0x00, 0x40, 0x80},
    {   X0, Y3,   0,   0,   0,   0, 0x00, 0x00, 0x40, 0x80},
    
    {   X1, Y0,   0,   0,   0,   0, 0x00, 0x00, 0x00, 0xc0},
    {   X2, Y0,   0,   0,   0,   0, 0x00, 0x00, 0x00, 0xc0},
    {   X2, Y3,   0,   0,   0,   0, 0x00, 0x00, 0x00, 0xc0},
    {   X1, Y3,   0,   0,   0,   0, 0x00, 0x00, 0x00, 0xc0},
    
    {   X2, Y0,   0,   0,   0,   0, 0x00, 0x00, 0x00, 0x80},
    {   X3, Y0,   0,   0,   0,   0, 0x00, 0x00, 0x00, 0x80},
    {   X3, Y3,   0,   0,   0,   0, 0x00, 0x00, 0x00, 0x80},
    {   X2, Y3,   0,   0,   0,   0, 0x00, 0x00, 0x00, 0x80},
    
    {   X3, Y0,   0,   0,   0,   0, 0x00, 0x00, 0x00, 0xc0},
    {   X4, Y0,   0,   0,   0,   0, 0x00, 0x00, 0x00, 0xc0},
    {   X4, Y3,   0,   0,   0,   0, 0x00, 0x00, 0x00, 0xc0},
    {   X3, Y3,   0,   0,   0,   0, 0x00, 0x00, 0x00, 0xc0},
    
    {   X4, Y0,   0,   0,   0,   0, 0x00, 0x00, 0x00, 0x80},
    {   X5, Y0,   0,   0,   0,   0, 0x00, 0x00, 0x00, 0x80},
    {   X5, Y3,   0,   0,   0,   0, 0x00, 0x00, 0x00, 0x80},
    {   X4, Y3,   0,   0,   0,   0, 0x00, 0x00, 0x00, 0x80},
    
    {   X5, Y0,   0,   0,   0,   0, 0x00, 0x00, 0x00, 0xc0},
    {   X6, Y0,   0,   0,   0,   0, 0x00, 0x00, 0x00, 0xc0},
    {   X6, Y3,   0,   0,   0,   0, 0x00, 0x00, 0x00, 0xc0},
    {   X5, Y3,   0,   0,   0,   0, 0x00, 0x00, 0x00, 0xc0},
    
    {   X6, Y0,   0,   0,   0,   0, 0x00, 0x00, 0x00, 0x80},
    {   X7, Y0,   0,   0,   0,   0, 0x00, 0x00, 0x00, 0x80},
    {   X7, Y3,   0,   0,   0,   0, 0x00, 0x00, 0x00, 0x80},
    {   X6, Y3,   0,   0,   0,   0, 0x00, 0x00, 0x00, 0x80},
};

/* The vertex list of audio and task bar */
static Vtx auTaskVtx[] = {
    { 0, Y2+BAR_WIDTH,  -1,   0,   0,   0, 0xff, 0xff, 0xff, 0xff},
    { 0, Y2+BAR_WIDTH,  -1,   0,   0,   0, 0xff, 0xff, 0xff, 0xff},
    { 0, Y2          ,  -1,   0,   0,   0, 0xff, 0xff, 0xff, 0xff},
    { 0, Y2          ,  -1,   0,   0,   0, 0xff, 0xff, 0xff, 0xff},
    { 0, Y2+BAR_WIDTH,  -1,   0,   0,   0, 0xff, 0xff, 0xff, 0xff},
    { 0, Y2+BAR_WIDTH,  -1,   0,   0,   0, 0xff, 0xff, 0xff, 0xff},
    { 0, Y2          ,  -1,   0,   0,   0, 0xff, 0xff, 0xff, 0xff},
    { 0, Y2          ,  -1,   0,   0,   0, 0xff, 0xff, 0xff, 0xff},
    { 0, Y2+BAR_WIDTH,  -1,   0,   0,   0, 0xff, 0xff, 0xff, 0xff},
    { 0, Y2+BAR_WIDTH,  -1,   0,   0,   0, 0xff, 0xff, 0xff, 0xff},
    { 0, Y2          ,  -1,   0,   0,   0, 0xff, 0xff, 0xff, 0xff},
    { 0, Y2          ,  -1,   0,   0,   0, 0xff, 0xff, 0xff, 0xff},
    { 0, Y2+BAR_WIDTH,  -1,   0,   0,   0, 0xff, 0xff, 0xff, 0xff},
    { 0, Y2+BAR_WIDTH,  -1,   0,   0,   0, 0xff, 0xff, 0xff, 0xff},
    { 0, Y2          ,  -1,   0,   0,   0, 0xff, 0xff, 0xff, 0xff},
    { 0, Y2          ,  -1,   0,   0,   0, 0xff, 0xff, 0xff, 0xff},
};

/* The vertex list of the graphic, RSP and task bar */
static Vtx gfxRspTaskVtx[] = {
    {    0, Y1+BAR_WIDTH,   0,   0,   0,   0, 0xff, 0xff, 0x00, 0xff},
    {    0, Y1+BAR_WIDTH,   0,   0,   0,   0, 0xff, 0x00, 0x00, 0xff},
    {    0, Y1          ,   0,   0,   0,   0, 0xff, 0x00, 0x00, 0xff},
    {    0, Y1          ,   0,   0,   0,   0, 0xff, 0xff, 0x00, 0xff},
    {    0, Y1+BAR_WIDTH,   0,   0,   0,   0, 0xff, 0xff, 0x00, 0xff},
    {    0, Y1+BAR_WIDTH,   0,   0,   0,   0, 0xff, 0x00, 0x00, 0xff},
    {    0, Y1          ,   0,   0,   0,   0, 0xff, 0x00, 0x00, 0xff},
    {    0, Y1          ,   0,   0,   0,   0, 0xff, 0xff, 0x00, 0xff},
    {    0, Y1+BAR_WIDTH,   0,   0,   0,   0, 0xff, 0xff, 0x00, 0xff},
    {    0, Y1+BAR_WIDTH,   0,   0,   0,   0, 0xff, 0x00, 0x00, 0xff},
    {    0, Y1          ,   0,   0,   0,   0, 0xff, 0x00, 0x00, 0xff},
    {    0, Y1          ,   0,   0,   0,   0, 0xff, 0xff, 0x00, 0xff},
    {    0, Y1+BAR_WIDTH,   0,   0,   0,   0, 0xff, 0xff, 0x00, 0xff},
    {    0, Y1+BAR_WIDTH,   0,   0,   0,   0, 0xff, 0x00, 0x00, 0xff},
    {    0, Y1          ,   0,   0,   0,   0, 0xff, 0x00, 0x00, 0xff},
    {    0, Y1          ,   0,   0,   0,   0, 0xff, 0xff, 0x00, 0xff},
    {    0, Y1+BAR_WIDTH,   0,   0,   0,   0, 0xff, 0xff, 0x00, 0xff},
    {    0, Y1+BAR_WIDTH,   0,   0,   0,   0, 0xff, 0x00, 0x00, 0xff},
    {    0, Y1          ,   0,   0,   0,   0, 0xff, 0x00, 0x00, 0xff},
    {    0, Y1          ,   0,   0,   0,   0, 0xff, 0xff, 0x00, 0xff},
    {    0, Y1+BAR_WIDTH,   0,   0,   0,   0, 0xff, 0xff, 0x00, 0xff},
    {    0, Y1+BAR_WIDTH,   0,   0,   0,   0, 0xff, 0x00, 0x00, 0xff},
    {    0, Y1          ,   0,   0,   0,   0, 0xff, 0x00, 0x00, 0xff},
    {    0, Y1          ,   0,   0,   0,   0, 0xff, 0xff, 0x00, 0xff},
    {    0, Y1+BAR_WIDTH,   0,   0,   0,   0, 0xff, 0xff, 0x00, 0xff},
    {    0, Y1+BAR_WIDTH,   0,   0,   0,   0, 0xff, 0x00, 0x00, 0xff},
    {    0, Y1          ,   0,   0,   0,   0, 0xff, 0x00, 0x00, 0xff},
    {    0, Y1          ,   0,   0,   0,   0, 0xff, 0xff, 0x00, 0xff},
    {    0, Y1+BAR_WIDTH,   0,   0,   0,   0, 0xff, 0xff, 0x00, 0xff},
    {    0, Y1+BAR_WIDTH,   0,   0,   0,   0, 0xff, 0x00, 0x00, 0xff},
    {    0, Y1          ,   0,   0,   0,   0, 0xff, 0x00, 0x00, 0xff},
    {    0, Y1          ,   0,   0,   0,   0, 0xff, 0xff, 0x00, 0xff},

};

/* The vertex list of the graphic, RDP and task bar */
static Vtx gfxRdpTaskVtx[] = {
    {    0, Y0+BAR_WIDTH,   0,   0,   0,   0, 0x00, 0xff, 0x00, 0xff},
    {    0, Y0+BAR_WIDTH,   0,   0,   0,   0, 0x00, 0xff, 0x00, 0xff},
    {    0, Y0          ,   0,   0,   0,   0, 0x00, 0xff, 0xff, 0xff},
    {    0, Y0          ,   0,   0,   0,   0, 0x00, 0xff, 0xff, 0xff},
    {    0, Y0+BAR_WIDTH,   0,   0,   0,   0, 0x00, 0xff, 0x00, 0xff},
    {    0, Y0+BAR_WIDTH,   0,   0,   0,   0, 0x00, 0xff, 0x00, 0xff},
    {    0, Y0          ,   0,   0,   0,   0, 0x00, 0xff, 0xff, 0xff},
    {    0, Y0          ,   0,   0,   0,   0, 0x00, 0xff, 0xff, 0xff},
    {    0, Y0+BAR_WIDTH,   0,   0,   0,   0, 0x00, 0xff, 0x00, 0xff},
    {    0, Y0+BAR_WIDTH,   0,   0,   0,   0, 0x00, 0xff, 0x00, 0xff},
    {    0, Y0          ,   0,   0,   0,   0, 0x00, 0xff, 0xff, 0xff},
    {    0, Y0          ,   0,   0,   0,   0, 0x00, 0xff, 0xff, 0xff},
    {    0, Y0+BAR_WIDTH,   0,   0,   0,   0, 0x00, 0xff, 0x00, 0xff},
    {    0, Y0+BAR_WIDTH,   0,   0,   0,   0, 0x00, 0xff, 0x00, 0xff},
    {    0, Y0          ,   0,   0,   0,   0, 0x00, 0xff, 0xff, 0xff},
    {    0, Y0          ,   0,   0,   0,   0, 0x00, 0xff, 0xff, 0xff},
    {    0, Y0+BAR_WIDTH,   0,   0,   0,   0, 0x00, 0xff, 0x00, 0xff},
    {    0, Y0+BAR_WIDTH,   0,   0,   0,   0, 0x00, 0xff, 0x00, 0xff},
    {    0, Y0          ,   0,   0,   0,   0, 0x00, 0xff, 0xff, 0xff},
    {    0, Y0          ,   0,   0,   0,   0, 0x00, 0xff, 0xff, 0xff},
    {    0, Y0+BAR_WIDTH,   0,   0,   0,   0, 0x00, 0xff, 0x00, 0xff},
    {    0, Y0+BAR_WIDTH,   0,   0,   0,   0, 0x00, 0xff, 0x00, 0xff},
    {    0, Y0          ,   0,   0,   0,   0, 0x00, 0xff, 0xff, 0xff},
    {    0, Y0          ,   0,   0,   0,   0, 0x00, 0xff, 0xff, 0xff},
    {    0, Y0+BAR_WIDTH,   0,   0,   0,   0, 0x00, 0xff, 0x00, 0xff},
    {    0, Y0+BAR_WIDTH,   0,   0,   0,   0, 0x00, 0xff, 0x00, 0xff},
    {    0, Y0          ,   0,   0,   0,   0, 0x00, 0xff, 0xff, 0xff},
    {    0, Y0          ,   0,   0,   0,   0, 0x00, 0xff, 0xff, 0xff},
    {    0, Y0+BAR_WIDTH,   0,   0,   0,   0, 0x00, 0xff, 0x00, 0xff},
    {    0, Y0+BAR_WIDTH,   0,   0,   0,   0, 0x00, 0xff, 0x00, 0xff},
    {    0, Y0          ,   0,   0,   0,   0, 0x00, 0xff, 0xff, 0xff},
    {    0, Y0          ,   0,   0,   0,   0, 0x00, 0xff, 0xff, 0xff},
};


static Vp vp = {
    320*2, 240*2, G_MAXZ/2, 0,	/* scale */
    320*2, 240*2, G_MAXZ/2, 0,	/* translate */
};
static Gfx initRcpDl[] = {
    gsSPSegment(0x0, 0x0),
    gsDPSetScissor(G_SC_NON_INTERLACE, 0, 0, NU_GFX_INIT_SCREEN_WD * 2, NU_GFX_INIT_SCREEN_HT * 2),
    gsSPClearGeometryMode(0xffffffff),
    gsSPSetGeometryMode(G_SHADE | G_SHADING_SMOOTH),
    gsSPTexture(0, 0, 0, 0, G_OFF),
    gsDPSetCycleType(G_CYC_1CYCLE),
    gsDPPipelineMode(G_PM_NPRIMITIVE),
    gsDPSetColorDither(G_CD_BAYER),
    gsDPSetRenderMode(G_RM_XLU_SURF, G_RM_XLU_SURF),
    gsDPSetCombineMode(G_CC_SHADE, G_CC_SHADE),
    gsDPPipeSync(),
    gsSPEndDisplayList(),
};
#define	G_CC_TEXT	0, 0, 0, PRIMITIVE, 0, 0, 0, TEXEL0

static Gfx markInitDl[] = {
    gsDPPipeSync(),
    gsDPSetTextureFilter(G_TF_POINT),
    gsDPSetCombineMode(G_CC_DECALRGBA, G_CC_DECALRGBA),
    gsDPSetTexturePersp(G_TP_NONE),
    gsDPSetTextureLOD(G_TL_TILE),
    gsDPSetTextureLUT(G_TT_RGBA16),
    gsDPSetTextureDetail(G_TD_CLAMP),
    gsDPSetTextureConvert(G_TC_FILT),
    gsDPSetAlphaCompare(G_AC_NONE),
    gsSPTexture(0xffff, 0xffff, 0, G_TX_RENDERTILE, G_ON),
    gsDPPipeSync(),
    gsDPLoadTLUT_pal16(0, nuDebPerfMarkertlut),
    gsDPLoadTextureBlock_4b(nuDebPerfMarker, G_IM_FMT_CI,
			   80, 12,
			   0,
			   G_TX_WRAP , G_TX_WRAP,
			   G_TX_NOMASK, G_TX_NOMASK,
			   G_TX_NOLOD, G_TX_NOLOD),
    gsDPSetRenderMode(G_RM_TEX_EDGE, G_RM_TEX_EDGE),
    gsSPClearGeometryMode(0xffffffff),
    gsDPPipeSync(),
    gsSPEndDisplayList()
};    

/* Draw the frame of the performance meter */
static Gfx perfFrameDl[] = {
    gsSPVertex(&perfFrameVtx, 32, 0),
    gsSP2Triangles(0, 1, 2, 0, 2, 3, 0, 0),
    gsSPEndDisplayList(),
};

/* The scale value in the case of changing the display frame number of the task bar */
static float frameScale[] = { 1.0, 0.6667, 0.5, 0.4, 0.3333, 0.2857};

static Mtx 	perf_mtx;
static Mtx 	view_mtx;
static Mtx 	line_mtx;
static Mtx 	scale_mtx;
static Mtx 	move_mtx;

static Gfx	glistBuf[2][256];
static u32	glistCnt = 0;

/*----------------------------------------------------------------------*/
/*	Display the performance meter (Type O)				*/
/* 	Display the performance meter					*/
/*	Type O displays to see the activation and end of RSP/RDP tasks.	*/
/*	IN:	glist_ptr	The display list buffer			*/
/*		frameNum	The display frame number (1-6)		*/
/*		y		The display position			*/
/*	RET:	None							*/
/*----------------------------------------------------------------------*/
void nuDebTaskPerfBar0(u32 frameNum, u32 y, u32 flag)
{

    u32		cnt;
    s64		retraceTime;
    s64		StartTime;
    s64		EndTime;
    short 	StartX;
    short	EndX;
    u32		color;
    u32		vtxIdx;
    float	posY;
    Gfx*	glistPtr;
    u32		viMode;
    u32		scale;

    glistPtr = glistBuf[glistCnt];    
    /* Get current VI mode */
    viMode = osViGetCurrentMode();
    if(viMode < 8){
	scale = 1;
    } else if(viMode < 14){
	scale = 2;
    }

    vp.vp.vscale[0] = vp.vp.vtrans[0] = 320*2*scale;
    vp.vp.vscale[1] = vp.vp.vtrans[1] = 240*2*scale;
    gSPViewport(glistPtr++, &vp);
	
    /* Set RSP and RDP */
    gSPDisplayList(glistPtr++, OS_K0_TO_PHYSICAL(initRcpDl));
    gDPSetColorImage(glistPtr++, G_IM_FMT_RGBA, G_IM_SIZ_16b,
		     NU_DEB_CON_WIDTH * scale,
		     osVirtualToPhysical(nuGfxCfb_ptr));
    
    /* Convert the origin of the coordinate from the lower-left to the upper-left */
    posY =(240.0 - FRAME_Y1) - y / scale;
    if(posY == 0.0) posY = 0.1;

    /* Limit the display frame number to make sure */
    if(frameNum > 5) frameNum = 5;

    guOrtho(&perf_mtx,
	    0.0F, (float)(NU_GFX_INIT_SCREEN_WD*scale),
	    0.0F, (float)(NU_GFX_INIT_SCREEN_HT*scale),
	    -10.0F, 10.0F, 1.0F);
    guLookAt(&view_mtx,
	     0.0, 0.0, 1.0,
	     0.0, 0.0, 0.0,
	     0.0, 1.0, 0.0);
    
    /* Change the display frame number by changing the scale of the X-axis direction */
    guScale(&scale_mtx, frameScale[frameNum]*(float)scale, (float)scale, 1.0);

    /* Move the position from the left-end to the 20-dot on the screen */
    guTranslate(&move_mtx, (20.0)/frameScale[frameNum], posY, 0.0);
    guMtxIdent(&line_mtx);
    
    gSPMatrix(glistPtr++, &perf_mtx, 
	    G_MTX_PROJECTION | G_MTX_LOAD | G_MTX_NOPUSH);
    gSPMatrix(glistPtr++, &view_mtx,
	      G_MTX_PROJECTION | G_MTX_MUL | G_MTX_NOPUSH);

    gSPMatrix(glistPtr++, OS_K0_TO_PHYSICAL(&line_mtx),
	      G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
    gSPMatrix(glistPtr++, OS_K0_TO_PHYSICAL(&scale_mtx),
	      G_MTX_MODELVIEW | G_MTX_MUL | G_MTX_NOPUSH);
    gSPMatrix(glistPtr++, OS_K0_TO_PHYSICAL(&move_mtx),
	      G_MTX_MODELVIEW | G_MTX_MUL | G_MTX_NOPUSH);

    

    /* Draw the outside frame of the performance bar display */
    gSPDisplayList(glistPtr++, OS_K0_TO_PHYSICAL(perfFrameDl));
    perfFrameVtx[1].v.ob[0] = VFRAME * (frameNum + 2) + 2;
    perfFrameVtx[2].v.ob[0] = VFRAME * (frameNum + 2) + 2;


    /* Draw the inside frame only for the display frame number */
    for(cnt = 0; cnt < frameNum + 2; cnt++){
	vtxIdx = cnt * 4 + 4;
	gSP2Triangles(glistPtr++, vtxIdx+ 0, vtxIdx+1, vtxIdx+2, 0,
		      vtxIdx+2, vtxIdx+3, vtxIdx+0, 0);
    }


    gDPPipeSync(glistPtr++);
    gSPVertex(glistPtr++, auTaskVtx, 16, 0);
    
    retraceTime = nuDebTaskPerfPtr->retraceTime;
    /* Display the audio and task bar */
    for(cnt = 0; cnt < nuDebTaskPerfPtr->auTaskCnt; cnt++){
	vtxIdx = cnt * 4;
	StartTime = nuDebTaskPerfPtr->auTaskTime[cnt].rspStart - retraceTime;
	EndTime =  nuDebTaskPerfPtr->auTaskTime[cnt].rspEnd - retraceTime;
	StartX = (StartTime * VFRAME)/16666 + X1;
	EndX	=  (EndTime * VFRAME)/16666 + X1;
	auTaskVtx[vtxIdx+0].v.ob[0] = StartX;
	auTaskVtx[vtxIdx+1].v.ob[0] = EndX;
	auTaskVtx[vtxIdx+2].v.ob[0] = EndX;
	auTaskVtx[vtxIdx+3].v.ob[0] = StartX;
	gSP2Triangles(glistPtr++, vtxIdx+0, vtxIdx+2, vtxIdx+1, 0,
		      vtxIdx+0, vtxIdx+3, vtxIdx+2, 0);
    }

    /* Display the graphic, RSP and task bar */
    gSPVertex(glistPtr++, gfxRspTaskVtx, 32, 0);
    for(cnt = 0; cnt < nuDebTaskPerfPtr->gfxTaskCnt; cnt++){
	vtxIdx = cnt * 4;
	StartTime = nuDebTaskPerfPtr->gfxTaskTime[cnt].rspStart - retraceTime;
	EndTime =  nuDebTaskPerfPtr->gfxTaskTime[cnt].rspEnd - retraceTime;
	StartX = (StartTime * VFRAME)/16666 + X1;
	EndX	=  (EndTime * VFRAME)/16666 + X1;
	gfxRspTaskVtx[vtxIdx+0].v.ob[0] = StartX;
	gfxRspTaskVtx[vtxIdx+1].v.ob[0] = EndX;
	gfxRspTaskVtx[vtxIdx+2].v.ob[0] = EndX;
	gfxRspTaskVtx[vtxIdx+3].v.ob[0] = StartX;
	gSP2Triangles(glistPtr++, vtxIdx+0, vtxIdx+2, vtxIdx+1, 0,
		      vtxIdx+0, vtxIdx+3, vtxIdx+2, 0);
    }
    
    /* Display the graphic, RDP and task bar */
    gSPVertex(glistPtr++, gfxRdpTaskVtx, 32, 0);
    for(cnt = 0; cnt < nuDebTaskPerfPtr->gfxTaskCnt; cnt++){
	vtxIdx = cnt * 4;
	StartTime = nuDebTaskPerfPtr->gfxTaskTime[cnt].rspStart - retraceTime;
	EndTime =  nuDebTaskPerfPtr->gfxTaskTime[cnt].rdpEnd - retraceTime;
	StartX = (StartTime * VFRAME)/16666 + X1;
	EndX	=  (EndTime * VFRAME)/16666 + X1;
	gfxRdpTaskVtx[vtxIdx+0].v.ob[0] = StartX;
	gfxRdpTaskVtx[vtxIdx+1].v.ob[0] = EndX;
	gfxRdpTaskVtx[vtxIdx+2].v.ob[0] = EndX;
	gfxRdpTaskVtx[vtxIdx+3].v.ob[0] = StartX;
	color = 255;
	gfxRdpTaskVtx[vtxIdx+1].v.cn[2] = color;
	gfxRdpTaskVtx[vtxIdx+2].v.cn[2] = color;
	gSP2Triangles(glistPtr++, vtxIdx+0, vtxIdx+2, vtxIdx+1, 0,
		      vtxIdx+0, vtxIdx+3, vtxIdx+2, 0);
    }

    gSPDisplayList(glistPtr++, markInitDl);

    for(cnt = 0; cnt < NU_DEB_MARKER_NUM; cnt ++){
	if(nuDebTaskPerfPtr->markerTime[cnt] > 0){
	    StartTime = nuDebTaskPerfPtr->markerTime[cnt] - retraceTime;
	    StartX = ((StartTime * VFRAME)/16666 + X1) * frameScale[frameNum]
		  + 20 ;
	    StartX *= scale;
	    EndX = StartX + MARK_SIZE_X * scale;
	    gSPTextureRectangle(glistPtr++,
				StartX << 2, (y  - MARK_SIZE_Y * scale) << 2,
				EndX << 2, y  << 2,
				G_TX_RENDERTILE,
				(cnt * MARK_SIZE_X) <<5, 0,
				1<<(10 - (scale - 1)), 1<<(10 - (scale - 1)));
	}
    }

    gDPFullSync(glistPtr++);
    gSPEndDisplayList(glistPtr++);

#ifdef NU_DEBUG    
    if((u32)(glistPtr - glistBuf[glistCnt]) > 256){
	osSyncPrintf("nuDebTaskPerfBar0: gfx list buffer over.\n");
    }
#endif /* NU_DEBUG */
    
    /* Start the graphic task */
    nuGfxTaskStart(glistBuf[glistCnt],
		  (s32)(glistPtr - glistBuf[glistCnt]) * sizeof(Gfx),
		   NU_GFX_UCODE_F3DEX , flag);
    glistCnt++;
    glistCnt &= 0x01;
}

#endif /* NU_DEBUG */
