/*============================================================================

		NINTENDO64 TECHNICAL SUPPORT CENTER 
		
		    NINTENDO64 SAMPLE PROGRAM 2

		Copyright (C) 1997, NINTENDO Co., Ltd.

============================================================================*/
#include <ultra64.h>
#include <PR/ramrom.h>
#include <assert.h>

#include "def.h"
#include "vram.h"
#include "segment.h"
#include "message.h"
#include "nnsched.h"
#include "graphic.h"


/*
 Define blender's rendering mode.
Semitransparent line, no anti-alias, do not use Z-Buffer mode.
*/
#define RM_XLU_LINE(clk)                                     \
        IM_RD | CVG_DST_CLAMP | CVG_X_ALPHA |           \
        ALPHA_CVG_SEL | FORCE_BL | ZMODE_OPA |                  \
        GBL_c##clk(G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_1MA)
/* Semitransparent line, no anti-alias, use Z-Buffer mode */
#define RM_ZB_XLU_LINE(clk)                                     \
        Z_CMP | IM_RD | CVG_DST_CLAMP | CVG_X_ALPHA |           \
        ALPHA_CVG_SEL | FORCE_BL | ZMODE_OPA |                  \
        GBL_c##clk(G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_1MA)

#define G_RM_XLU_LINE     RM_XLU_LINE(1)
#define G_RM_XLU_LINE2    RM_XLU_LINE(2)
#define G_RM_ZB_XLU_LINE  RM_ZB_XLU_LINE(1)
#define G_RM_ZB_XLU_LINE2 RM_ZB_XLU_LINE(2)

/* vertices of performance meter frame */
Vtx perfline_vtx[] = {
  {   40,  20,   0,   0,   0,   0, 0xff, 0xff, 0xff, 0x80},
  {   40,  40,   0,   0,   0,   0, 0xff, 0xff, 0xff, 0x80},
  {  120,  20,   0,   0,   0,   0, 0xff, 0xff, 0xff, 0x80},
  {  120,  40,   0,   0,   0,   0, 0xff, 0xff, 0xff, 0x80},
  {  200,  20,   0,   0,   0,   0, 0xff, 0xff, 0xff, 0x80},
  {  200,  40,   0,   0,   0,   0, 0xff, 0xff, 0xff, 0x80},
  {  280,  20,   0,   0,   0,   0, 0xff, 0xff, 0xff, 0x80},
  {  280,  40,   0,   0,   0,   0, 0xff, 0xff, 0xff, 0x80},
  {   40,  40,   0,   0,   0,   0, 0xff, 0xff, 0xff, 0x80},
  {  280,  40,   0,   0,   0,   0, 0xff, 0xff, 0xff, 0x80},
  {   40,  20,   0,   0,   0,   0, 0xff, 0xff, 0xff, 0x80},
  {  280,  20,   0,   0,   0,   0, 0xff, 0xff, 0xff, 0x80},
};

/* performance meter vertex */
Vtx meter_vtx[60];

/* drawing of performance meter frame */
Gfx perfline_dl[] = {
  gsDPPipeSync(),
  gsDPSetScissor(G_SC_NON_INTERLACE, 0,0, 319,239),
  gsDPSetCycleType(G_CYC_1CYCLE),
  gsSPClearGeometryMode((G_SHADE|G_SHADING_SMOOTH|G_LIGHTING|G_TEXTURE_GEN|G_TEXTURE_GEN_LINEAR|G_CULL_BOTH|G_FOG)),
  gsDPSetRenderMode(G_RM_XLU_LINE, G_RM_XLU_LINE2),
  gsSPSetGeometryMode( G_SHADE),
  gsDPSetCombineMode(G_CC_SHADE, G_CC_SHADE),
  gsSPVertex(&perfline_vtx, 12, 0),
  gsSPLine3D( 0, 1, 0),
  gsSPLine3D( 2, 3, 0),
  gsSPLine3D( 4, 5, 0),
  gsSPLine3D( 6, 7, 0),
  gsSPLine3D( 8, 9, 0),
  gsSPLine3D(10,11, 0),
  gsSPEndDisplayList(),
};

/*-----------------------------------------------------------------------------
 Vertex matrix settings for performance meter
-----------------------------------------------------------------------------*/
static void vtxset(Vtx* vtx_ptr, short x, short y, short z,
	    u8 red, u8 green, u8 blue, u8 alpha)
{
  vtx_ptr->v.ob[0] = x;
  vtx_ptr->v.ob[1] = y;
  vtx_ptr->v.ob[2] = z;
  vtx_ptr->v.flag  = 0;
  vtx_ptr->v.tc[0] = 0;
  vtx_ptr->v.tc[1] = 0;
  vtx_ptr->v.cn[0] = red;
  vtx_ptr->v.cn[1] = green;
  vtx_ptr->v.cn[2] = blue;
  vtx_ptr->v.cn[3] = alpha;
}

/*-----------------------------------------------------------------------------
 Performance meter drawing
-----------------------------------------------------------------------------*/
void Perf_line(void)
{
#ifdef NN_SC_PERF
  static Mtx perf_mtx;
  static Mtx line_mtx;
  static Mtx view_mtx;
  int cnt;
  int vtx_index;
  int vtx_rest;
  short x0,x1;

  /* Performance meter viewpoint settings */
  guOrtho(&perf_mtx,
	  0.0F, (float)SCREEN_WD,
	  0.0F, (float)SCREEN_HT,
	  -1.0F, 10.0F, 1.0F);

  guLookAt(&view_mtx,
	   0.0, 0.0, 1.0,
	   0.0, 0.0, 0.0,
	   0.0, 1.0, 0.0);

  gSPMatrix(glist_ptr++, &perf_mtx, 
	    G_MTX_PROJECTION | G_MTX_LOAD | G_MTX_NOPUSH);
  gSPMatrix(glist_ptr++, &view_mtx,
	    G_MTX_PROJECTION | G_MTX_MUL | G_MTX_NOPUSH);

  guMtxIdent(&line_mtx);
  gSPMatrix(glist_ptr++, OS_K0_TO_PHYSICAL(&line_mtx),
	    G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);

  vtx_index = 0;

  /* Audio task performance display */
  for(cnt = 0; cnt < nnsc_perf_ptr->autask_cnt; cnt++){

    x0 = nnsc_perf_ptr->autask_stime[cnt] * 80/16666 + 40;
    vtxset( &meter_vtx[vtx_index], x0, 36, 0, 0xff, 0x00, 0x00, 0x80);
    vtx_index++;
    x0 = nnsc_perf_ptr->autask_etime[cnt] * 80/16666 + 40;
    vtxset( &meter_vtx[vtx_index], x0, 36, 0, 0xff, 0x00, 0x00, 0x80);
    vtx_index++;

  }

  /* Graphic task performance display */
  for(cnt = 0; cnt < nnsc_perf_ptr->gtask_cnt; cnt++){

    /* RSP measurement results  */
    x0 = nnsc_perf_ptr->gtask_stime[cnt] * 80/16666 + 40;
    vtxset( &meter_vtx[vtx_index], x0, 30, 0, 0x00, 0xff, 0x00, 0x80);
    vtx_index++;

    x1 = nnsc_perf_ptr->rsp_etime[cnt] * 80/16666 + 40;
    vtxset( &meter_vtx[vtx_index], x1, 30, 0, 0x00, 0xff, 0x00, 0x80);
    vtx_index++;

    /* RDP measurement results */
    vtxset( &meter_vtx[vtx_index], x0, 24, 0, 0xff, 0xff, 0x00, 0x80);
    vtx_index++;

    x1 = nnsc_perf_ptr->rdp_etime[cnt] * 80/16666 + 40;
    vtxset( &meter_vtx[vtx_index], x1, 24, 0, 0xff, 0xff , 0x00, 0x80);
    vtx_index++;

  }

  gSPDisplayList(glist_ptr++, OS_K0_TO_PHYSICAL(perfline_dl));

  /* create display list*/
  for(cnt = 0; cnt < vtx_index; cnt +=2){
    /* load vertices in vertex cache */
    if( (cnt % 16 ) == 0){ 
      /* calculate remaining number of vertices */
      if((vtx_index-cnt) > 16 ){
	vtx_rest = 16;
      }	else {
	/* if remaining vertices greater than 16 */
	vtx_rest = vtx_index-cnt;
      }
      /* load vertex */
      gSPVertex(glist_ptr++, &meter_vtx[cnt], vtx_rest, 0);
    }
    gSPLineW3D(glist_ptr++, cnt, cnt+1, 7,0);

  }
  /* the end */
  gDPPipeSync(glist_ptr++);
#endif /* NN_SC_PERF */
}




