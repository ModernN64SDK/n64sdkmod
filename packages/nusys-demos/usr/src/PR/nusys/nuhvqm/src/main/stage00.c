/*
   stage00.c 
*/

#include <assert.h>
#include <nusys.h>
#include "main.h"
#include "hvqm.h"
#include "graphic.h"

static float theta;  /* Rotation angle of square */
static float triPos_x; /* Display position X */
static float triPos_y; /* Display position Y */

void shadetri(Dynamic* dynamicp);

/* Initialize stage 0 */
void initStage00(void)
{
  triPos_x = 0.0;
  triPos_y = 0.0;
  theta = 0.0;

  hvqmDelete(); /* Not a stage used by hvqm so delete extra threads, etc. */
}

/* Make display list and start task */
void makeDL00(void)
{
  Dynamic* dynamicp;
  char conbuf[20]; 

  /* Specify display list buffer */
  dynamicp = &gfx_dynamic[gfx_gtask_no];
  glistp = &gfx_glist[gfx_gtask_no][0];

  /*  Initialize RCP */
  gfxRCPInit();

  /* Clear frame buffer and Z buffer */
  gfxClearCfb();

  /* projection,modeling matrix set */
  guOrtho(&dynamicp->projection,
	  -(float)SCREEN_WD/2.0F, (float)SCREEN_WD/2.0F,
	  -(float)SCREEN_HT/2.0F, (float)SCREEN_HT/2.0F,
	  1.0F, 10.0F, 1.0F);
  guRotate(&dynamicp->modeling, theta, 0.0F, 0.0F, 1.0F);
  guTranslate(&dynamicp->translate, triPos_x, triPos_y, 0.0F);

  /* Draw square */
  shadetri(dynamicp);

  gDPFullSync(glistp++);
  gSPEndDisplayList(glistp++);

  assert((glistp - gfx_glist[gfx_gtask_no]) < GFX_GLIST_LEN);

  /* Start task and swap display buffer */
  nuGfxTaskStart(&gfx_glist[gfx_gtask_no][0],
		 (s32)(glistp - gfx_glist[gfx_gtask_no]) * sizeof (Gfx),
		 NU_GFX_UCODE_F3DEX , NU_SC_NOSWAPBUFFER);

  if(contPattern & 0x1)
    {
      /* Change position of text display */
      nuDebConTextPos(0,12,23);
      sprintf(conbuf,"triPos_x=%5.1f",triPos_x);
      nuDebConCPuts(0, conbuf);

      nuDebConTextPos(0,12,24);
      sprintf(conbuf,"triPos_y=%5.1f",triPos_y);
      nuDebConCPuts(0, conbuf);
    }
  else
    {
      nuDebConTextPos(0,9,24);
      nuDebConCPuts(0, "Controller1 not connect");
    }
    
  /* Render text into frame buffer */
  nuDebConDisp(NU_SC_SWAPBUFFER);

  /* Swap display list buffer */
  gfx_gtask_no ^= 1;
}


/* Game update process for stage 0 */
void updateGame00(void)
{  
  static float vel = 1.0;

  /* Read data from Controller 1 */
  nuContDataGetEx(contdata,0);

  /* Change display position based on stick data */
  triPos_x = contdata->stick_x;
  triPos_y = contdata->stick_y;

  /* Rotate backwards using A button */
  if(contdata[0].trigger & A_BUTTON)
    {
      vel = -vel;
      osSyncPrintf("A button Push\n");
    }

  /* Rotate fast while pressing B button */
  if(contdata[0].button & B_BUTTON)
    theta += vel * 3.0;
  else
    theta += vel;

  if(theta>360.0)
      theta-=360.0;
  else if (theta<0.0)
      theta+=360.0;

  /* Change stage with START button */
  if(contdata[0].trigger & START_BUTTON)
      stage = 1;
}

/* Vertex coordinates */
static Vtx shade_vtx[] =  {
        {        -64,  64, -5, 0, 0, 0, 0, 0xff, 0, 0xff	},
        {         64,  64, -5, 0, 0, 0, 0, 0, 0, 0xff	},
        {         64, -64, -5, 0, 0, 0, 0, 0, 0xff, 0xff	},
        {        -64, -64, -5, 0, 0, 0, 0xff, 0, 0, 0xff	},
};

/* Draw a square */
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
