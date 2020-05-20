/*
   stage00.c 

   Copyright (C) 1997-1999, NINTENDO Co,Ltd.
*/

#include <assert.h>
#include <nusys.h>
#include "main.h"
#include "graphic.h"

static float triPos_x; /* The display position, X */
static float triPos_y; /* The display position, Y */

void shadetri(Dynamic* dynamicp);

/* The initialization of stage 0  */
void initStage00(void)
{
  triPos_x = 0.0;
  triPos_y = 0.0;
}

/* Make the display list and activate the task  */
void makeDL00(void)
{
  Dynamic* dynamicp;

  /* Specify the display list buffer  */
  dynamicp = &gfx_dynamic[gfx_gtask_no];
  glistp = &gfx_glist[gfx_gtask_no][0];

  /*  The initialization of RCP  */
  gfxRCPInit();

  /* Clear the frame and Z-buffer  */
  gfxClearCfb();

  /* projection,modeling matrix set */
  guOrtho(&dynamicp->projection,
	  -(float)SCREEN_WD/2.0F, (float)SCREEN_WD/2.0F,
	  -(float)SCREEN_HT/2.0F, (float)SCREEN_HT/2.0F,
	  1.0F, 10.0F, 1.0F);
  guRotate(&dynamicp->modeling, 0.0F, 0.0F, 0.0F, 1.0F);
  guTranslate(&dynamicp->translate, triPos_x, triPos_y, 0.0F);

  /* Draw a square  */
  shadetri(dynamicp);

  gDPFullSync(glistp++);
  gSPEndDisplayList(glistp++);

  assert((glistp - gfx_glist[gfx_gtask_no]) < GFX_GLIST_LEN);

  /* Activate the task and 
     switch display buffers.  */
  nuGfxTaskStart(&gfx_glist[gfx_gtask_no][0],
		 (s32)(glistp - gfx_glist[gfx_gtask_no]) * sizeof (Gfx),
		 NU_GFX_UCODE_F3DEX , NU_SC_NOSWAPBUFFER);

  /* Change character representation positions  */
  nuDebConTextPos(0,11,5);
  /* Display characters  */
  nuDebConCPuts(0, "Stage 0 : Stick");
  /* Write characters on the frame buffer  */
  nuDebConDisp(NU_SC_SWAPBUFFER);

  /* Switch display list buffers  */
  gfx_gtask_no ^= 1;
}


/* The game progressing process for stage 0  */
void updateGame00(void)
{  
  /* Data reading of controller 1  */
  nuContDataGetEx(contdata,0);

  /* Change the display position by stick data  */
  triPos_x = contdata->stick_x;
  triPos_y = contdata->stick_y;

  /* Convert to stage 1 when the start button is pushed  */
  if(contdata[0].trigger & START_BUTTON){
    /* Remove the call-back function.  */
    nuGfxFuncRemove();
    /* Specify next stage to main  */
    stage = 1;
  }
}

/* The vertex coordinate  */
static Vtx shade_vtx[] =  {
        {        -64,  64, -5, 0, 0, 0, 0, 0xff, 0, 0xff	},
        {         64,  64, -5, 0, 0, 0, 0, 0, 0, 0xff	},
        {         64, -64, -5, 0, 0, 0, 0, 0, 0xff, 0xff	},
        {        -64, -64, -5, 0, 0, 0, 0xff, 0, 0, 0xff	},
};

/* Draw a square  */
void shadetri(Dynamic* dynamicp)
{
  gSPMatrix(glistp++,OS_K0_TO_PHYSICAL(&(dynamicp->projection)),
		G_MTX_PROJECTION|G_MTX_LOAD|G_MTX_NOPUSH);
  gSPMatrix(glistp++,OS_K0_TO_PHYSICAL(&(dynamicp->translate)),
		G_MTX_MODELVIEW|G_MTX_LOAD|G_MTX_NOPUSH);
  gSPMatrix(glistp++,OS_K0_TO_PHYSICAL(&(dynamicp->modeling)),
		G_MTX_MODELVIEW|G_MTX_MUL|G_MTX_NOPUSH);

  gSPVertex(glistp++,&(shade_vtx[0]),4, 0);

  gDPPipeSync(glistp++);
  gDPSetCycleType(glistp++,G_CYC_1CYCLE);
  gDPSetRenderMode(glistp++,G_RM_AA_OPA_SURF, G_RM_AA_OPA_SURF2);
  gSPClearGeometryMode(glistp++,0xFFFFFFFF);
  gSPSetGeometryMode(glistp++,G_SHADE| G_SHADING_SMOOTH);

  gSP2Triangles(glistp++,0,1,2,0,0,2,3,0);
}
