/*============================================================================

		NINTENDO64 TECHNICAL SUPPORT CENTER 
		
		    NINTENDO64 SAMPLE PROGRAM 1

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
#include "controller.h"

extern Gfx cont_finish_dl[];
void ModelController(Dynamic*,ModelMtx *);
extern ModelMtx mtx_cont[];



/*-----------------------------------------------------------------------------
  graphic00

-----------------------------------------------------------------------------*/
void graphic_00(void)
{
  Dynamic* dynamic_ptr;
  NNScTask *t;
  static LookAt lookat;
  u16   perspNorm;
  u32   cnt;

  /* 
 specify display list buffer/task buffer
  */
  dynamic_ptr = &gfx_dynamic[gfx_gtask_no];
  glist_ptr = &gfx_glist[gfx_gtask_no][0];
  t = &gfx_task[gfx_gtask_no];

  /* set RSP and RDP */
  gfxRCPInit();

  /* clear frame buffer and Z buffer */
  gfxClearCfb();

  /* calculate and set projection matrix */
  guPerspective(&dynamic_ptr->projection, &perspNorm,
                33, 320.0/240.0, 10, 2000, 1.0);
  guLookAtReflect(&dynamic_ptr->viewing, &lookat,
                  0, 50, -600 ,
                  0, 0, 0,
                  0, 1, 0);
  gSPPerspNormalize(glist_ptr++, perspNorm);
  gSPLookAt(glist_ptr++, &lookat);

  gSPMatrix(glist_ptr++, &(dynamic_ptr->projection), 
            G_MTX_PROJECTION | G_MTX_LOAD | G_MTX_NOPUSH);
  gSPMatrix(glist_ptr++, &(dynamic_ptr->viewing), 
            G_MTX_PROJECTION | G_MTX_MUL | G_MTX_NOPUSH);
  
  /* draw model */
  for(cnt = 0; cnt < numControllers; cnt++){
    ModelController(dynamic_ptr,&mtx_cont[cnt]);
  }
  /* end top-level display list */
  gDPFullSync(glist_ptr++);
  gSPEndDisplayList(glist_ptr++);

  /* Flush the dynamic segment */
  osWritebackDCacheAll();
  
  gfxTaskStart(t, gfx_glist[gfx_gtask_no],
	       (s32)(glist_ptr - gfx_glist[gfx_gtask_no]) * sizeof (Gfx),
	        GFX_GSPCODE_F3DEX2, NN_SC_SWAPBUFFER);
}


/*-----------------------------------------------------------------------------
 Display controller model
-----------------------------------------------------------------------------*/
void ModelController(Dynamic* dynamic_ptr,ModelMtx *model_mtx)
{

  Mtx rotate_x;
  Mtx rotate_y;
  Mtx rotate_z;

  /* calculate model coordinates */
  guTranslate(&(model_mtx->trans), model_mtx->pos_x, model_mtx->pos_y, model_mtx->pos_z);
  guScale(&(model_mtx->scale), model_mtx->size, model_mtx->size, model_mtx->size);
  guRotate(&rotate_x, model_mtx->rot_x, 1.0, 0.0, 0.0);
  guRotate(&rotate_y, model_mtx->rot_y, 0.0, 1.0, 0.0);
  guRotate(&rotate_z, model_mtx->rot_z, 0.0, 0.0, 1.0);
  guMtxCatL(&rotate_x,&rotate_y, &model_mtx->rotate);
  guMtxCatL(&rotate_z,&model_mtx->rotate,&model_mtx->rotate);

  /* set model matrix */

  gSPMatrix(glist_ptr++, &(model_mtx->trans), 
	    G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);


  gSPMatrix(glist_ptr++, &(model_mtx->rotate), 
	    G_MTX_MODELVIEW | G_MTX_MUL | G_MTX_NOPUSH);

  gSPMatrix(glist_ptr++, &(model_mtx->scale), 
	    G_MTX_MODELVIEW | G_MTX_MUL | G_MTX_NOPUSH);

  gSPSegment(glist_ptr++, STATIC_SEGMENT, osVirtualToPhysical(gfx_dlist_buf[0]));
  /* model display list */
  gSPDisplayList(glist_ptr++, cont_finish_dl);


}


















