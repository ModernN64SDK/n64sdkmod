/*
   stage00.c 

   Copyright (C) 1997-1999, NINTENDO Co,Ltd.			
*/

#include <assert.h>
#include <nusys.h>
#include "main.h"
#include "graphic.h"

static float triPos_x; /* Display position X */
static float triPos_y; /* Display position Y */
static float theta;  /* Rotation angle for a square */

void shadetri(Dynamic* dynamicp);


/* Initialize stage0 */
void initStage00(void)
{
  triPos_x = 0.0;
  triPos_y = 0.0;
  theta = 0.0;
}

/* Create a display list and launch task */
void makeDL00(void)
{
  Dynamic* dynamicp;

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
  guTranslate(&dynamicp->translate, triPos_x, triPos_y, 0.0F);
  guRotate(&dynamicp->modeling, theta, 0.0F, 0.0F, 1.0F);

  /* Render a square */
  shadetri(dynamicp);

  gDPFullSync(glistp++);
  gSPEndDisplayList(glistp++);

  assert((glistp - gfx_glist[gfx_gtask_no]) < GFX_GLIST_LEN);

  /* Launch the task and change display buffer */
  if(now_xbusflag)
    nuGfxTaskStart(&gfx_glist[gfx_gtask_no][0],
		   (s32)(glistp - gfx_glist[gfx_gtask_no]) * sizeof (Gfx),
		   GFX_UCODE_F3DEX_XBUS , NU_SC_NOSWAPBUFFER|NU_SC_UCODE_XBUS);
  else
    nuGfxTaskStart(&gfx_glist[gfx_gtask_no][0],
		   (s32)(glistp - gfx_glist[gfx_gtask_no]) * sizeof (Gfx),
		   NU_GFX_UCODE_F3DEX , NU_SC_NOSWAPBUFFER);

  nuDebTaskPerfBar1(1,200,NU_SC_NOSWAPBUFFER);

  /* Display drawing status */
  nuDebConClear(0);
  nuDebConTextPos(0,3,4);
  if(now_xbusflag)
    nuDebConCPuts(0, "XBUS Mode");
  else
  {
    sprintf(conbuf, "FIFO Mode (0x%Xbytes)", now_fifosize);
    nuDebConCPuts(0, conbuf);
  }
  nuDebConTextPos(0,3,5);
  sprintf(conbuf,"%2d draw/sec", dspcount);
  nuDebConCPuts(0, conbuf);

  /* Draw strings in the frame buffer */
  nuDebConDisp(NU_SC_SWAPBUFFER);

  /* Change the display list buffer */
  gfx_gtask_no ^= 1;
}


/* Game status process for the stage0 */
void updateGame00(void)
{  
  static float vel = 1.0;

  /* Read data from controller 1 */
  nuContDataGetEx(contdata,0);

  /* Change the display position obtained from the stick data */
  triPos_x = contdata->stick_x;
  triPos_y = contdata->stick_y;

  /* A button to reverse rotate */
  if(contdata[0].trigger & A_BUTTON)
    vel = -vel;

  /* Fast rotation while B button is pressed */
  if(contdata[0].button & B_BUTTON)
    theta += vel * 3.0;
  else
    theta += vel;

  /* Z button to change between XBUS and FIFO */
  if(contdata[0].trigger & Z_TRIG)
    next_xbusflag ^= 1;

  if(!now_xbusflag)
  {
    /* Up/right C buttons to change FIFO buffer size (16 byte unit) */
    if(contdata[0].button & U_CBUTTONS)
      next_fifosize += 0x10;
    if(contdata[0].button & R_CBUTTONS)
      next_fifosize -= 0x10;
    /* Left/down C buttons to change FIFO buffer size (256 byte unit) */
    if(contdata[0].button & L_CBUTTONS)
      next_fifosize += 0x100;
    if(contdata[0].button & D_CBUTTONS)
      next_fifosize -= 0x100;
    /* Adjust FIFO buffer range */
    /* Required minimum values for each microcode
       F3DEX2 F3DEX2.Non	0x410,
       F3DEX2.Rej F3DLX2.Rej	0x600,
       L3DEX2			0x540,
       S2DEX2			0x800 */
    if( next_fifosize < 0x410 )
      next_fifosize = 0x410;
    if( next_fifosize > NU_GFX_RDP_OUTPUTBUFF_SIZE )
      next_fifosize = NU_GFX_RDP_OUTPUTBUFF_SIZE;
  }
}

/* Coordinates of vertex */
static Vtx shade_vtx[] =  {
        {        -64,  64, -5, 0, 0, 0, 0, 0xff, 0, 0xff	},
        {         64,  64, -5, 0, 0, 0, 0, 0, 0, 0xff	},
        {         64, -64, -5, 0, 0, 0, 0, 0, 0xff, 0xff	},
        {        -64, -64, -5, 0, 0, 0, 0xff, 0, 0, 0xff	},
};

/* Draws a square */
void shadetri(Dynamic* dynamicp)
{
  int i;

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

  for(i = 0; i < 25; i++)
    gSP2Triangles(glistp++,0,1,2,0,0,2,3,0);
}
