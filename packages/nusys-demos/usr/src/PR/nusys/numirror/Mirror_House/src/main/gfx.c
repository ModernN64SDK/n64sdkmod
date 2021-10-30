/*======================================================================*/
/*		NIFFprev						*/
/*		gfx.c							*/
/*									*/
/*		Copyright (C) 1997, NINTENDO Co,Ltd.			*/
/*									*/
/*======================================================================*/
#define __GFX_C__

/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	Display List Manager 
 :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */
#include <string.h>

#include "pack.h"
#include "nusys.h"
#include "om_common.h"

/* ===========================================================================
	defines
 =========================================================================== */

/* ===========================================================================
	variables
 =========================================================================== */

#define MAX_GFXBUF 2

typedef enum{
    GFXSTAT_FREE = 0,
    GFXSTAT_BUILDING,
    GFXSTAT_DRAWING
} GFXSTAT;

typedef struct {
#if 1
    Gfx gfx[GFXLEN_GLIST*2];
    Mtx mtx[GFXLEN_MTX*2];
    Light light[GFXLEN_LIGHT*2];
#else
    Gfx gfx[GFXLEN_GLIST];
    Mtx mtx[GFXLEN_MTX];
    Light light[GFXLEN_LIGHT];
#endif
    GFXSTAT stat;
} GFXBUF;

static u16 *gfxZBUF;

static GFXBUF gfxbuf[MAX_GFXBUF];
static u32 building_gfxlen;
static Mtx *mtxp;
static Light *lightp;

u32 v_count;
int perfmonflg;

int building_gb;
int drawing_gb;

/* ===========================================================================
	functions
 =========================================================================== */

/* ---------------------------------------------------------------------------
	Callback for Frame Buffer Swap
	Note:  Called from a separate thread
 --------------------------------------------------------------------------- */
void gfxBufferSwap(void *p)
{
    /* Discard GFXBUF */
    gfxbuf[drawing_gb].stat = GFXSTAT_FREE;
    drawing_gb = -1;

    /* Swap the frame buffer */
    nuGfxSwapCfb(p);

    nuGfxDisplayOn();
}

/* ----------------------------------------------------------------------------
name:
    gfxInit
synopsis:
    void gfxInit(void);
description:
    Initializes the gfx driver 
parameter:
return:
notes:
 --------------------------------------------------------------------------- */
void gfxInit(void)
{
    int i;

    /* Initialize the graphics library */
    nuGfxInit();

    /* Set VI mode */
    osViSetMode(&osViModeTable[OS_VI_NTSC_LAF1]);
    osViSetSpecialFeatures(OS_VI_GAMMA_OFF|OS_VI_GAMMA_DITHER_ON);

    /* Display OFF */
    nuGfxDisplayOff();
    gfxZBUF = (u16*)(NU_GFX_ZBUFFER_ADDR);

    /* Initialize the Gfx & Mtx buffer */
    for(i=0; i<MAX_GFXBUF; ++i)
      gfxbuf[i].stat = GFXSTAT_FREE;

    building_gb = -1;
    drawing_gb = -1;
    v_count = 0;
    perfmonflg = 1;

    /* Set the frame buffer swap callback function */
    nuGfxSwapCfbFuncSet(gfxBufferSwap);
}

/* ----------------------------------------------------------------------------
name:
    gfxBegin
synopsis:
    Gfx *gfxBegin( void );
description:
    Begins creating the rendering DL.
parameter:
return:
    Gfx *;
	Display list pointer. NULL indicates rendering buffer could not be provided.
notes:
    	The size of the Gfx buffer is determined beforehand.  It may not be possible to reserve the buffer, so also set the appropriate buffer size.
 --------------------------------------------------------------------------- */
Gfx *gfxBegin( void )
{
    Gfx *gp;
    int i;

    /* Use the buffer area available */
    for(i = 0; i < MAX_GFXBUF; ++i)
      if(gfxbuf[i].stat == GFXSTAT_FREE)
	break;

    if(i<MAX_GFXBUF){

	/* Use the buffer  */
	gfxbuf[i].stat = GFXSTAT_BUILDING;
	building_gb = i;

	gp = gfxbuf[i].gfx;
	mtxp = gfxbuf[i].mtx;
	lightp = &gfxbuf[i].light[0];

	/* Allocate the addresses of the frame buffer and z-buffer to segments */
	gSPSegment(gp++, PHISICAL_SEGMENT, 0);
	gSPSegment(gp++, CFB_SEGMENT, nuGfxCfb_ptr);
	gSPSegment(gp++, ZBUF_SEGMENT, gfxZBUF);

	return gp;
    }

    return NULL;
}

/* ----------------------------------------------------------------------------
name:
    gfxEnd
synopsis:
    void gfxEnd(Gfx *gfx);
description:
    Ends creation of  the rendering DL
parameter:
    Gfx *;
	Display list pointer
return:
notes:
    Loads the two Gfx functions gDPFullSync() and gSPEndDisplayList() to finish building.
 --------------------------------------------------------------------------- */
void gfxEnd(Gfx *gfx)
{
    void *vp;
    u32 len;

    gDPFullSync(gfx++);
    gSPEndDisplayList(gfx++);

    vp = gfxbuf[building_gb].gfx;
    building_gfxlen = len = (u32)gfx - (u32)vp;

    osWritebackDCache(vp, len);
#if 0
    osWritebackDCacheAll();
#endif
}

/* ----------------------------------------------------------------------------
name:
    gfxFlush
synopsis:
    void gfxFlush(u32 flag, u32 ucode);
description:
    Executes the rendering task.
parameter:
return:
notes:
    Executes by taking as an RCP task the display list finished immediately beforehand by gfxEnd().  A performance meter currently appears above the model display for use in debugging.
 --------------------------------------------------------------------------- */
void gfxFlush(u32 flag, u32 ucode)
{
    int i;

    /* Enter the pending buffer as a task */
    if(nuGfxTaskSpool == 0){
	drawing_gb = i = building_gb;
	gfxbuf[i].stat = GFXSTAT_DRAWING;
	if(!perfmonflg){
	    /* Performance meter present */
	    nuGfxTaskStart(gfxbuf[i].gfx, building_gfxlen, ucode, flag);
/*	    nuDebTaskPerfBar1( 3, 224, NU_SC_NOSWAPBUFFER ); */
	}else{
	    /* Model display only */
	    nuGfxTaskStart(gfxbuf[i].gfx, building_gfxlen, ucode, flag);
	}
    }else{
	/* Discard the previous rendering if it is unfinished, discarding even from the buffer just created. */
	gfxbuf[building_gb].stat = GFXSTAT_FREE;
    }

    if (flag == NU_SC_NOSWAPBUFFER) {
        nuDebConDisp(NU_SC_SWAPBUFFER);
    }
}

/* ----------------------------------------------------------------------------
name:
    gfxWaitSync
synopsis:
    void gfxWaitSync(void);
description:
    Waits for a retrace.
parameter:
return:
notes:
 --------------------------------------------------------------------------- */
void gfxWaitSync(void)
{
    /* ++V counter */
    ++v_count;
}

/* ----------------------------------------------------------------------------
name:
    gfxMatrix
synopsis:
    Gfx *gfxMatrix( Gfx *gp, Mtx *m, u32 flag );
description:
    Operates on a matrix.
parameter:
    Gfx *gp;
	Display list pointer 
    Mtx *m;
	Matrix structure for RSP
    u32 flag;
	Setup flag
return:
notes:
    The parameter m is copied and entered in Gfx. The Mtx corresponding to the copy destination is joined with the Gfx buffer and managed so that it is not overwritten until rendering is finished.
 --------------------------------------------------------------------------- */
Gfx *gfxMatrix( Gfx *gp, Mtx *m, u32 flag )
{
    memcpy(mtxp, m, sizeof(Mtx));
    gSPMatrix(gp++, OS_K0_TO_PHYSICAL(mtxp), flag);

    mtxp++;

    return gp;
}

/* ---------------------------------------------------------------------------
	Assign a Projection Transformation Matrix
 --------------------------------------------------------------------------- */
Gfx *gfxPerspective( Gfx *gp, float fovy, float aspect, float near, float far, float scale )
{
    Mtx m;
    u16 perspNorm;

    guPerspective( &m, &perspNorm, fovy, aspect, near, far, scale);
    gSPPerspNormalize(gp++, perspNorm);
    return gfxMatrix( gp, &m, G_MTX_PROJECTION|G_MTX_LOAD|G_MTX_NOPUSH);
}

/* ---------------------------------------------------------------------------
	Assign a xxSingle-Level Multiplicative Projection Transformation Matrixxx
 --------------------------------------------------------------------------- */
Gfx *gfxMulPerspective( Gfx *gp, float fovy, float aspect, float near, float far, float scale, float mul[4][4] )
{
    Mtx m;

    float mf[4][4];
    float ans[4][4];
    u16 perspNorm;

    guPerspectiveF( mf, &perspNorm, fovy, aspect, near, far, scale );
    guMtxCatF( mul, mf, ans );
    guMtxF2L( ans, &m );
    gSPPerspNormalize( gp++, perspNorm );
    return gfxMatrix( gp, &m, G_MTX_PROJECTION|G_MTX_LOAD|G_MTX_NOPUSH );
}

/* ---------------------------------------------------------------------------
	Assign a Parallel Projection Matrix
 --------------------------------------------------------------------------- */
Gfx *gfxOrtho( Gfx *gp, float left, float right, float bottom, float top, float near, float far, float scale )
{
    Mtx m;

    guOrtho( &m, left, right, bottom, top, near, far, scale);
    return gfxMatrix( gp, &m, G_MTX_PROJECTION|G_MTX_LOAD|G_MTX_NOPUSH);
}

/* ---------------------------------------------------------------------------
Create and Assign a Model Matrix Based on Three Values:  xxViewpoint, Focal Point, and Up Directionxx
 --------------------------------------------------------------------------- */
Gfx *gfxLookAtReflect( Gfx *gp, s8 *right, s8 *upper)
{
    Light *lkp;
    static s8 _s8_a[]={0,0,0,0};
    static s8 _s8_b[]={0,0x80,0,0};

    lkp = lightp;

    memcpy(&lightp->l.col[0], _s8_a, 4);
    memcpy(&lightp->l.colc[0], _s8_a, 4);
    memcpy(&lightp->l.dir[0], right, 3);
    ++lightp;
    memcpy(&lightp->l.col[0], _s8_b, 4);
    memcpy(&lightp->l.colc[0], _s8_b, 4);
    memcpy(&lightp->l.dir[0], upper, 3);
    ++lightp;
    gSPLookAt(gp++, lkp);

    return gp;
}

/* ---------------------------------------------------------------------------
	Set to Write to the Model Matrix
 --------------------------------------------------------------------------- */
Gfx *gfxLoadMatrix( Gfx *gp, Mtx *m )
{
    return gfxMatrix( gp, m, G_MTX_MODELVIEW|G_MTX_LOAD|G_MTX_NOPUSH);
}

/* ---------------------------------------------------------------------------
	Multiply the Model Matrix 
 --------------------------------------------------------------------------- */
Gfx *gfxMultiMatrix( Gfx *gp, Mtx *m )
{
    return gfxMatrix( gp, m, G_MTX_MODELVIEW|G_MTX_MUL|G_MTX_NOPUSH);
}

/* ---------------------------------------------------------------------------
	Multiply the Model Matrix while Pushing xxOne Levelxx
 --------------------------------------------------------------------------- */
Gfx *gfxPushMatrix( Gfx *gp, Mtx *m )
{
    return gfxMatrix( gp, m, G_MTX_MODELVIEW|G_MTX_MUL|G_MTX_PUSH);
}

/* ---------------------------------------------------------------------------
	Pop the Model Matrix xxOne Levelxx
 --------------------------------------------------------------------------- */
Gfx *gfxPopMatrix( Gfx *gp )
{
    gSPPopMatrix(gp++, G_MTX_MODELVIEW);
    return gp;
}

/* ---------------------------------------------------------------------------
	Absorb Macro
 --------------------------------------------------------------------------- */
Gfx *ndgSPLightColor(Gfx *gp, int lnum, u32 color)
{
    switch(lnum){
      case 0: {gSPLightColor( gp++, LIGHT_1, color );} break;
      case 1: {gSPLightColor( gp++, LIGHT_2, color );} break;
      case 2: {gSPLightColor( gp++, LIGHT_3, color );} break;
      case 3: {gSPLightColor( gp++, LIGHT_4, color );} break;
      case 4: {gSPLightColor( gp++, LIGHT_5, color );} break;
      case 5: {gSPLightColor( gp++, LIGHT_6, color );} break;
      case 6: {gSPLightColor( gp++, LIGHT_7, color );} break;
      case 7: {gSPLightColor( gp++, LIGHT_8, color );} break;
    }
    return gp;
}

/* ---------------------------------------------------------------------------
	Transmit a Light 
 --------------------------------------------------------------------------- */
Gfx *gfxSetLight( Gfx *gp, int num, u32 color, s8 *normal )
{
    if(normal){
	/* Define using the heap if a normal vector is specified */
	memcpy(&lightp->l.col[0], &color, 3);
	memcpy(&lightp->l.colc[0], &color, 3);
	lightp->l.dir[0] = normal[0];
	lightp->l.dir[1] = normal[1];
	lightp->l.dir[2] = normal[2];
	gSPLight(gp++, lightp++, num);
    }else{
	/* If no normal vector specified, xxMODIFY transmitxx only the color value  */
	gp = ndgSPLightColor( gp, num, color );
    }

    return gp;
}

/* ---------------------------------------------------------------------------
	Transmit the Ambient Light 
 --------------------------------------------------------------------------- */
Gfx *gfxSetAmbient( Gfx *gp, int num, u32 color )
{
    static s8 dummy_normal[]={ 0, 0, 127 };

    return gfxSetLight( gp, num, color, dummy_normal );
}


