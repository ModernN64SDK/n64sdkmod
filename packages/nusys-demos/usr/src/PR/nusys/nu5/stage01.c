/*
   stage01.c 

   Copyright (C) 1997-1999, NINTENDO Co,Ltd.			
*/

#include <assert.h>
#include <nusys.h>
#include "main.h"
#include "graphic.h"

static float triPos_x; /* The display position, X */
static float triPos_y; /* The display position, Y */
static float theta;  /* The rotational angle of the square */

void shadetri(Dynamic* dynamicp);


/* The initialization of stage 1 */
void initStage01(void)
{
  triPos_x = 0.0;
  triPos_y = 0.0;
  theta = 0.0;
}


/* Make the display list for stage 1 and activate the task */
void makeDL01(void)
{
  Dynamic* dynamicp;

  glistp = &gfx_clear_glist[gfx_gtask_no][0];

  /* Initialize RCP */
  gfxRCPInit();

  /* Clear the frame and Z-buffer */
  gfxClearCfb();

  assert((glistp - gfx_clear_glist[gfx_gtask_no]) < GFX_CLEAR_GLIST_LEN);

  gDPFullSync(glistp++);
  gSPEndDisplayList(glistp++);

  /* Activate the task and 
     switch display buffers. */
  nuGfxTaskStart(&gfx_clear_glist[gfx_gtask_no][0],
		 (s32)(glistp - gfx_clear_glist[gfx_gtask_no]) * sizeof (Gfx),
		 NU_GFX_UCODE_F3DEX , NU_SC_NOSWAPBUFFER);

  /* Specify the display list buffer */
  dynamicp = &gfx_dynamic[gfx_gtask_no];
  glistp = &gfx_glist[gfx_gtask_no][0];

  /* The initialization of RCP */
  gfxRCPInit();

  /* projection,modeling matrix set */
  guOrtho(&dynamicp->projection,
	  -(float)SCREEN_WD/2.0F, (float)SCREEN_WD/2.0F,
	  -(float)SCREEN_HT/2.0F, (float)SCREEN_HT/2.0F,
	  1.0F, 10.0F, 1.0F);
  guTranslate(&dynamicp->translate, triPos_x, triPos_y, 0.0F);
  guRotate(&dynamicp->modeling, theta, 0.0F, 0.0F, 1.0F);

  /* Draw a square */
  shadetri(dynamicp);

  gDPFullSync(glistp++);
  gSPEndDisplayList(glistp++);

  assert((glistp - gfx_glist[gfx_gtask_no]) < GFX_GLIST_LEN);

  /* Activate the task and 
     switch display buffers. */
  nuGfxTaskStart(&gfx_glist[gfx_gtask_no][0],
		 (s32)(glistp - gfx_glist[gfx_gtask_no]) * sizeof (Gfx),
		 NU_GFX_UCODE_F3DEX , NU_SC_NOSWAPBUFFER);


  nuDebTaskPerfBar0(1,200,NU_SC_NOSWAPBUFFER);

  /* Display the drawing state  */
  nuDebConTextPos(0,3,3);
  nuDebConCPuts(0, "Stage 01");
  nuDebConTextPos(0,3,4);
  if(pendflag == 0)
    nuDebConCPuts(0, "2 Frame Buffer");
  else
    nuDebConCPuts(0, "3 Frame Buffer");
  nuDebConTextPos(0,3,5);
  sprintf(conbuf,"%2d draw/sec",dspcount);
  nuDebConCPuts(0, conbuf);

  /* Draw characters on the frame buffer */
  nuDebConDisp(NU_SC_SWAPBUFFER);

  /* Switch display list buffers */
  gfx_gtask_no ^= 1;

}


/* The game progressing process for stage 1 */
void updateGame01(void)
{  
  static float vel = 1.0;

  /* Data reading of controller 1 */
  nuContDataGetEx(contdata,0);

  /* Change the display position according to stick data */
  triPos_x = contdata->stick_x;
  triPos_y = contdata->stick_y;

  /* The reverse rotation by the A button */
  if(contdata[0].trigger & A_BUTTON)
    vel = -vel;

  /* Rotate fast while the B button is pushed */
  if(contdata[0].button & B_BUTTON)
    theta += vel * 3.0;
  else
    theta += vel;

  /* Change the pending check when the Z button is pushed */
  if(contdata[0].trigger & Z_TRIG)
    pendflag ^= 1;

  /* Move to stage 0 when the start button is pushed */
  if(contdata[0].trigger & START_BUTTON)
    {
      /* Remove the call-back function.*/
      nuGfxFuncRemove();
      /* Specify next stage to main */
      stage = 0;
    }
}
