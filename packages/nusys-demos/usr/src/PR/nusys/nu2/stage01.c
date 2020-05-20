/*
   stage01.c 

   Copyright (C) 1997-1999, NINTENDO Co,Ltd.
*/

#include <assert.h>
#include <nusys.h>
#include "main.h"
#include "graphic.h"

static float theta;  /* The rotational angle of the square  */

void shadetri(Dynamic* dynamicp);


/* The initialization of stage 1  */
void initStage01(void)
{
  theta = 0.0;
}


/* Make the display list for stage 1 and activate the task  */
void makeDL01(void)
{
  Dynamic* dynamicp;

  /* Specify the display list buffer  */
  dynamicp = &gfx_dynamic[gfx_gtask_no];
  glistp = &gfx_glist[gfx_gtask_no][0];

  /*  The initialization of RCP  */
  gfxRCPInit();

  /* projection,modeling matrix set */
  guOrtho(&dynamicp->projection,
	  -(float)SCREEN_WD/2.0F, (float)SCREEN_WD/2.0F,
	  -(float)SCREEN_HT/2.0F, (float)SCREEN_HT/2.0F,
	  1.0F, 10.0F, 1.0F);
  guRotate(&dynamicp->modeling, theta, 0.0F, 0.0F, 1.0F);
  guTranslate(&dynamicp->translate, 0.0F, 0.0F, 0.0F);

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
  nuDebConCPuts(0, "Stage 1 : A & B");
  /* Draw characters on the frame buffer  */
  nuDebConDisp(NU_SC_SWAPBUFFER);

  /* Switch the display list buffer  */
  gfx_gtask_no ^= 1;

  glistp = &gfx_clear_glist[gfx_gtask_no][0];

  /*  The initialization of RCP  */
  gfxRCPInit();

  /* Clear the frame and Z-buffer  */
  gfxClearCfb();

  assert((glistp - gfx_clear_glist[gfx_gtask_no]) < GFX_CLEAR_GLIST_LEN);

  gDPFullSync(glistp++);
  gSPEndDisplayList(glistp++);

  /* Activate the task and 
     switch display buffers.  */
  nuGfxTaskStart(&gfx_clear_glist[gfx_gtask_no][0],
		 (s32)(glistp - gfx_clear_glist[gfx_gtask_no]) * sizeof (Gfx),
		 NU_GFX_UCODE_F3DEX , NU_SC_NOSWAPBUFFER);
}


/* The game progressing process for stage 1  */
void updateGame01(void)
{  
  static float vel = 1.0;

  /* Data reading of controller 1  */
  nuContDataGetEx(contdata,0);

  /* The reverse rotation by the A button  */
  if(contdata[0].trigger & A_BUTTON)
    vel = -vel;

  /* Rotate fast while the B button is pushed  */
  if(contdata[0].button & B_BUTTON)
    theta += vel * 3.0;
  else
    theta += vel;

if(theta>360.0)
      theta-=360.0;
  else if (theta<0.0)
      theta+=360.0;

  /* Convert to stage 0 when the start button is pushed  */
  if(contdata[0].trigger & START_BUTTON)
    {
      /* Remove the call-back function. */
      nuGfxFuncRemove();
      /* Specify next stage to main  */
      stage = 0;
    }
}
