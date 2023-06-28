/*============================================================================

   NINTENDO64 TEST PROGRAM 2 by K.Ohki

   graphic_01.c
============================================================================*/
#include <ultra64.h>
#include <PR/ramrom.h>
#include <assert.h>

#include "def.h"
#include "vram.h"
#include "segment.h"
#include "message.h"
#include "nnsched.h"
#include "controller.h"
#include "graphic.h"
#include "main01.h"
#include "audio.h"


/*
 Define blender's rendering mode.
Semi-transparent line, no anti-alias, do not use Z-Buffer mode
*/
#define RM_XLU_LINE(clk)                                     \
        IM_RD | CVG_DST_CLAMP | CVG_X_ALPHA |           \
        ALPHA_CVG_SEL | FORCE_BL | ZMODE_OPA |                  \
        GBL_c##clk(G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_1MA)
/* Semi-transparent line, no anti-alias, use Z-Buffer mode */
#define RM_ZB_XLU_LINE(clk)                                     \
        Z_CMP | IM_RD | CVG_DST_CLAMP | CVG_X_ALPHA |           \
        ALPHA_CVG_SEL | FORCE_BL | ZMODE_OPA |                  \
        GBL_c##clk(G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_1MA)

#define G_RM_XLU_LINE     RM_XLU_LINE(1)
#define G_RM_XLU_LINE2    RM_XLU_LINE(2)
#define G_RM_ZB_XLU_LINE  RM_ZB_XLU_LINE(1)
#define G_RM_ZB_XLU_LINE2 RM_ZB_XLU_LINE(2)

/* vertices of coordinate axes lines */
Vtx line_vtx[] = {
  { -20,    0,    0,   0,   0,   0, 0xff, 0x00, 0x00, 0xff},
  {  20,    0,    0,   0,   0,   0, 0xff, 0x00, 0x00, 0xff},
  {    0,  20,    0,   0,   0,   0, 0x00, 0xff, 0x00, 0xff},
  {    0, -20,    0,   0,   0,   0, 0x00, 0xff, 0x00, 0xff},
  {    0,   0,   20,   0,   0,   0, 0x00, 0x00, 0xff, 0xff},
  {    0,   0,  -20,   0,   0,   0, 0x00, 0x00, 0xff, 0xff},
};

/* display list of coordinate axes lines */
Gfx line_dl[] = {
  gsDPPipeSync(),
  gsDPSetScissor(G_SC_NON_INTERLACE, 0,0, 319,239),
  gsDPSetCycleType(G_CYC_1CYCLE),
  gsSPClearGeometryMode((G_SHADE|G_SHADING_SMOOTH|G_LIGHTING|G_TEXTURE_GEN|G_TEXTURE_GEN_LINEAR|G_CULL_BOTH|G_FOG)),
  gsDPSetRenderMode(G_RM_ZB_XLU_LINE, G_RM_ZB_XLU_LINE2),
  gsSPSetGeometryMode( G_ZBUFFER | G_SHADE),
  gsSPClipRatio(FRUSTRATIO_6),
  gsDPSetCombineMode(G_CC_SHADE, G_CC_SHADE),
  gsSPVertex(&line_vtx, 6, 0),
  gsSPLineW3D(0, 1, 0, 0),
  gsSPLineW3D(2, 3, 0, 0),
  gsSPLineW3D(4, 5, 0, 0),
  gsSPEndDisplayList(),
};

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


  
void Model_Fiter(CFiter*);
void View_Matrix(Dynamic *);
void Model_bullet(CFiter *);
void Perf_line(void);


extern Gfx bullet_dl[];
extern Gfx fiter_1_dl[];
extern CFiter fiter[];
extern u32 threadpri_flag;

/*-----------------------------------------------------------------------------
  graphic01

-----------------------------------------------------------------------------*/
void graphic_01(void)
{

  static Mtx line_mtx;
  Dynamic* dynamic_ptr;
  NNScTask *t;


  /* 
     Specify display list buffer and task buffer
  */
  dynamic_ptr = &gfx_dynamic[gfx_gtask_no];
  glist_ptr = &gfx_glist[gfx_gtask_no][0];
  t = &gfx_task[gfx_gtask_no];

  /* set RSP and RDP*/
  gfxRCPInit();

  /* clear frame buffer and Z buffer*/
  gfxClearCfb();

  /* end top-level display list */
  gDPFullSync(glist_ptr++);
  gSPEndDisplayList(glist_ptr++);

  /* Flush the dynamic segment */
  osWritebackDCacheAll();

  /* start graphic task */
  gfxTaskStart(t, &gfx_glist[gfx_gtask_no][0],
	       (s32)(glist_ptr - gfx_glist[gfx_gtask_no]) * sizeof (Gfx),
	       GFX_GSPCODE_F3DEX2, 0);

/*
     Here the graphic thread priority is lowered.
     It is normally GFX_THREAD_PRI so this has no effect.

      While the Z trigger is being pressed the priority rises and the system waits for a retrace message.  
   If a retrace message comes,  the system creates a graphic task , moves RSP-RDP and clears the Z-Buffer, FrameBuffer.  After that the priority declines for creation of an audio task.  At this point you can use the time the CPU takes to create  the audio task.

 -Buffer,FrameBuffer
*/
  osSetThreadPri(NULL, GRAPHIC_THREAD_PRI);


  /* 
     Specify display list buffer and task buffer 
  */
  dynamic_ptr = &gfx_dynamic[gfx_gtask_no];
  glist_ptr = &gfx_glist[gfx_gtask_no][0];
  t = &gfx_task[gfx_gtask_no];

  /* set RSP and RDP */
  gfxRCPInit();

    /* calculate and set projection matrix */
  View_Matrix(dynamic_ptr);

  /* set fog */
  gSPFogPosition(glist_ptr++,500,1000);
  gDPSetFogColor(glist_ptr++, 0, 0, 0, 255);

  /*  model  drawing */
  Model_Fiter(&fiter[0]);  /* 1rqrA(c)v_ */
  Model_Fiter(&fiter[1]);  /* 2rqrA(c)v_ */
  Model_bullet(&fiter[0]); /* 1rqrA'e */
  Model_bullet(&fiter[1]); /* 2rqrA'e */

  /* end top-level display list */
  gDPFullSync(glist_ptr++);
  gSPEndDisplayList(glist_ptr++);

  /* Flush the dynamic segmnet */
  osWritebackDCacheAll();

  /* start graphic task */
  gfxTaskStart(t, &gfx_glist[gfx_gtask_no][0],
	       (s32)(glist_ptr - gfx_glist[gfx_gtask_no]) * sizeof (Gfx),
	       GFX_GSPCODE_F3DEX2, 0);


  /* 
     specify display list buffer and task buffer 
  */
  dynamic_ptr = &gfx_dynamic[gfx_gtask_no];
  glist_ptr = &gfx_glist[gfx_gtask_no][0];
  t = &gfx_task[gfx_gtask_no];
 
  /* set RSP and RDP */
  gfxRCPInit();

  /* calculate and set projection matrix */
  View_Matrix(dynamic_ptr);

  /* model drawing */
  guMtxIdent(&dynamic_ptr->modeling);
  gSPMatrix(glist_ptr++, OS_K0_TO_PHYSICAL(&dynamic_ptr->modeling),
	    G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);

  /* display X,Y,Z axes */
  gSPDisplayList(glist_ptr++, OS_K0_TO_PHYSICAL(line_dl));

  /* display performance meter */
  Perf_line();

  /* end top-level display list */
  gDPFullSync(glist_ptr++);
  gSPEndDisplayList(glist_ptr++);

  /* Flush the dynamic segmnet */
  osWritebackDCacheAll();

  /* start graphic task */
  gfxTaskStart(t, gfx_glist[gfx_gtask_no],
	       (s32)(glist_ptr - gfx_glist[gfx_gtask_no]) * sizeof (Gfx),
	       GFX_GSPCODE_L3DEX2, NN_SC_SWAPBUFFER);

  /*
     While the Z trigger is being pressed this thread has higher priority than the audio thread.   This is so the graphic thread is moved first in the next retrace.
  */
  if(threadpri_flag){
    osSetThreadPri(NULL, AUDIO_THREAD_PRI+5);
  }
}


/*-----------------------------------------------------------------------------
Model display
-----------------------------------------------------------------------------*/
void Model_Fiter(CFiter *fiter)
{
  /* set segment registers */
  gSPSegment(glist_ptr++, STATIC_SEGMENT, osVirtualToPhysical(fiter->dlist_buf));

  /* model coordinate calculations */
  guScale(&fiter->model_size, fiter->pos.size, fiter->pos.size, fiter->pos.size);

  guTranslate(&fiter->model_trans, 0.0, fiter->pos.y, fiter->pos.z);
  
  guRotate(&fiter->model_rotate, fiter->pos.x, 0.0, 0.0, 1.0);


  /* model matrix settings */
  gSPMatrix(glist_ptr++, &fiter->model_size, 
	    G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
  gSPMatrix(glist_ptr++, &fiter->model_rotate, 
	    G_MTX_MODELVIEW | G_MTX_MUL | G_MTX_NOPUSH);
  gSPMatrix(glist_ptr++, &fiter->model_trans, 
	    G_MTX_MODELVIEW | G_MTX_MUL | G_MTX_NOPUSH);

  /* model display list */
  gSPDisplayList(glist_ptr++, fiter->dlist_ptr);
}

/*-----------------------------------------------------------------------------
  Bullet display
-----------------------------------------------------------------------------*/
void Model_bullet(CFiter *fiter)
{
  int cnt;
  CBullet* bullet_ptr;
  float size;

  size = fiter->pos.size;
  for(cnt = 0; cnt < FITER_BULLET_NUM; cnt++){
    bullet_ptr = &fiter->bullet[cnt];
    if(bullet_ptr->flag == FITER_BULLET_FLAG_FIRE){
      guScale(&bullet_ptr->model_size, size, size, size);
      guTranslate(&bullet_ptr->model_trans, 0.0,
		  bullet_ptr->pos.y, bullet_ptr->pos.z);
      guRotate(&bullet_ptr->model_rotate, bullet_ptr->pos.x, 0.0, 0.0, 1.0);

      gSPMatrix(glist_ptr++, &bullet_ptr->model_size, 
		G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
      gSPMatrix(glist_ptr++, &bullet_ptr->model_rotate, 
		G_MTX_MODELVIEW | G_MTX_MUL | G_MTX_NOPUSH);
      gSPMatrix(glist_ptr++, &bullet_ptr->model_trans, 
		G_MTX_MODELVIEW | G_MTX_MUL | G_MTX_NOPUSH);

      gSPDisplayList(glist_ptr++, bullet_dl);
    }
  }
}
/*-----------------------------------------------------------------------------
View matrix settings
-----------------------------------------------------------------------------*/
void View_Matrix(Dynamic *dynamic_ptr)
{
  float rot_pitch[4][4];
  float rot_yaw[4][4];
  u16   perspNorm;

  guRotateF(rot_pitch, lookat.pitch, 1.0, 0.0, 0.0);
  guRotateF(rot_yaw, lookat.yaw, 0.0, 1.0, 0.0);
  guMtxCatF(rot_pitch, rot_yaw, rot_yaw);
  guMtxXFMF(rot_yaw,
	    0.0, 0.0, lookat.dist,
	    &lookat.eye_x, &lookat.eye_y, &lookat.eye_z);

  guPerspective(&dynamic_ptr->projection, &perspNorm,
		33, 320.0/240.0, 20, 1000, 1.0);

  guLookAt(&dynamic_ptr->viewing,
		  lookat.eye_x, lookat.eye_y, lookat.eye_z,
		  lookat.at_x, lookat.at_y, lookat.at_z,
		  lookat.up_x, lookat.up_y, lookat.up_z);
  gSPPerspNormalize(glist_ptr++, perspNorm);

  gSPMatrix(glist_ptr++, &(dynamic_ptr->projection), 
	    G_MTX_PROJECTION | G_MTX_LOAD | G_MTX_NOPUSH);
  gSPMatrix(glist_ptr++, &(dynamic_ptr->viewing), 
	    G_MTX_PROJECTION | G_MTX_MUL | G_MTX_NOPUSH);
  gDPPipeSync(glist_ptr++);
}

/*-----------------------------------------------------------------------------
  Vertex matrix settings for performance meter
-----------------------------------------------------------------------------*/
void vtxset(Vtx* vtx_ptr, short x, short y, short z,
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

    /* RSP measurement results */
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

  /* create display list */
  for(cnt = 0; cnt < vtx_index; cnt +=2){
    /*  load vertices in vertex cache */
    if( (cnt % 16 ) == 0){ 
      /* calculate remaining number of vertices  */
      if((vtx_index-cnt) > 16 ){
	vtx_rest = 16;
      }	else {
	/* if remaining vertices greater than 16 */
	vtx_rest = vtx_index-cnt;
      }
      /* load vertex  */
      gSPVertex(glist_ptr++, &meter_vtx[cnt], vtx_rest, 0);
    }
    gSPLineW3D(glist_ptr++, cnt, cnt+1, 7,0);

  }
  /* the end */
  gDPPipeSync(glist_ptr++);
#endif /* NN_SC_PERF */
}


