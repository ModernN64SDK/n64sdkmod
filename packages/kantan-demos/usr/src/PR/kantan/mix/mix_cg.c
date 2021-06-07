/*============================================================================
  mix_cg.c
  ============================================================================*/

#define F3DEX_GBI

#include <ultra64.h>
#include <PR/ramrom.h>
#include <assert.h>

#include "def.h"
#include "vram.h"
#include "segment.h"
#include "message.h"
#include "nnsched.h"
#include "graphic.h"

#define SQUARE_SIZE 400.0

void DrawSquare( Dynamic* );
float rotate;

BaseMtx mtx_base;

/*data of map vertex*/
Vtx base_vtx[] = {
    { -SQUARE_SIZE, -SQUARE_SIZE, 0.0,  0, 0, 0,    0, 0xff,    0, 0xff},
    {  SQUARE_SIZE, -SQUARE_SIZE, 0.0,  0, 0, 0,    0,    0,    0, 0xff},
    {  SQUARE_SIZE,  SQUARE_SIZE, 0.0,  0, 0, 0,    0,    0, 0xff, 0xff},
    { -SQUARE_SIZE,  SQUARE_SIZE, 0.0,  0, 0, 0, 0xff,    0,    0, 0xff},
};

/*-----------------------------------------------------------------------------
  graphic00
  -----------------------------------------------------------------------------*/
void
graphic_00(void)
{
    Dynamic* dynamic_ptr;
    NNScTask *t;
    static LookAt lookat;
    u16   perspNorm;

    /* 
      Specify display list buffer, task buffer
       */
    dynamic_ptr = &gfx_dynamic[gfx_gtask_no];
    glist_ptr = &gfx_glist[gfx_gtask_no][0];
    t = &gfx_task[gfx_gtask_no];

    /* Set RSP and  RDP  */
    gfxRCPInit();

    /* Clear frame buffer and Z buffer */
    gfxClearCfb();

    /* Calculate and set projection matrix */
    guPerspective(&dynamic_ptr->projection, &perspNorm,
		  33, 320.0/240.0, 10, 10000, 1.0);

    guLookAtReflect(&dynamic_ptr->viewing, &lookat,
		    0, 0, 2000,
		    0, 0, 0,
		    0, 1, 0);
    
    gSPPerspNormalize(glist_ptr++, perspNorm);
    gSPLookAt(glist_ptr++, &lookat);

    gSPMatrix(glist_ptr++, &(dynamic_ptr->projection), 
	      G_MTX_PROJECTION | G_MTX_LOAD | G_MTX_NOPUSH);
    gSPMatrix(glist_ptr++, &(dynamic_ptr->viewing), 
	      G_MTX_PROJECTION | G_MTX_MUL | G_MTX_NOPUSH);

    /*Render model */
    DrawSquare(dynamic_ptr);

    /* end top-level display list */
    gDPFullSync(glist_ptr++);
    gSPEndDisplayList(glist_ptr++);

    /* Flush the dynamic segmnet */
    osWritebackDCacheAll();
  
    gfxTaskStart(t, gfx_glist[gfx_gtask_no],
		 (s32)(glist_ptr - gfx_glist[gfx_gtask_no]) * sizeof (Gfx),
		 GFX_GSPCODE_F3DEX, NN_SC_SWAPBUFFER);
}

/*-----------------------------------------------------------------------------
  Draw map base
  -----------------------------------------------------------------------------*/
void
DrawSquare(Dynamic* dynamic_ptr)
{
    /*Calculate model coordinate system */
    guTranslate(&mtx_base.trans, 0.0, 0.0, 0.0);
    guScale(&mtx_base.scale, 1.0, 1.0, 1.0);

    guRotateRPY(&mtx_base.rotate, 0.0, 0.0, rotate);

    /* Set model matrix */
    gSPMatrix(glist_ptr++, &(mtx_base.trans), 
	      G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);

    gSPMatrix(glist_ptr++, &(mtx_base.rotate), 
	      G_MTX_MODELVIEW | G_MTX_MUL | G_MTX_NOPUSH);

    gSPMatrix(glist_ptr++, &(mtx_base.scale), 
	      G_MTX_MODELVIEW | G_MTX_MUL | G_MTX_NOPUSH);

    gDPPipeSync(glist_ptr++);
    gDPSetCycleType(glist_ptr++,G_CYC_1CYCLE);

    gDPSetRenderMode(glist_ptr++,G_RM_ZB_OPA_SURF, G_RM_ZB_OPA_SURF2);
    gSPClearGeometryMode(glist_ptr++,0xFFFFFFFF);
    gSPSetGeometryMode(glist_ptr++,G_SHADE | G_SHADING_SMOOTH);

    gSPVertex(glist_ptr++,&(base_vtx[0]), 4, 0);

    gSP2Triangles(glist_ptr++,0,1,2,0,0,2,3,0);
}
