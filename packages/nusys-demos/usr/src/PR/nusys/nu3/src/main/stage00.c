/*
   stage00.c 

   Copyright (C) 1997-1999, NINTENDO Co,Ltd.
*/

#include <assert.h>
#include <nusys.h>
#include "main.h"
#include "graphic.h"

#ifdef N_AUDIO
#include <nualsgi_n.h>
#else
#include <nualsgi.h>
#endif

static float theta;  /* The rotational angle of the square */
static float triPos_x; /* The display position, X */
static float triPos_y; /* The display position, Y */

/* Declaration of the prototype */
void shadetri(Dynamic* dynamicp);
void soundCheck(void);



/* The initialization of stage 0 */
void initStage00(void)
{
  triPos_x = 0.0;
  triPos_y = 0.0;
  theta = 0.0;
}

/* Make the display list and activate the task */
void makeDL00(void)
{
  Dynamic* dynamicp;
  char conbuf[20]; 

  /* Specify the display list buffer */
  dynamicp = &gfx_dynamic[gfx_gtask_no];
  glistp = &gfx_glist[gfx_gtask_no][0];

  /* The initialization of RCP */
  gfxRCPInit();

  /* Clear the frame and Z-buffer */
  gfxClearCfb();

  /* projection,modeling matrix set */
  guOrtho(&dynamicp->projection,
	  -(float)SCREEN_WD/2.0F, (float)SCREEN_WD/2.0F,
	  -(float)SCREEN_HT/2.0F, (float)SCREEN_HT/2.0F,
	  1.0F, 10.0F, 1.0F);
  guRotate(&dynamicp->modeling, theta, 0.0F, 0.0F, 1.0F);
  guTranslate(&dynamicp->translate, triPos_x, triPos_y, 0.0F);

  /*  Draw a square */
  shadetri(dynamicp);

  gDPFullSync(glistp++);
  gSPEndDisplayList(glistp++);

  assert((glistp - gfx_glist[gfx_gtask_no]) < GFX_GLIST_LEN);

  /* Activate the task and 
     switch display buffers. */
  nuGfxTaskStart(&gfx_glist[gfx_gtask_no][0],
		 (s32)(glistp - gfx_glist[gfx_gtask_no]) * sizeof (Gfx),
		 NU_GFX_UCODE_F3DEX , NU_SC_NOSWAPBUFFER);

  if(contPattern & 0x1)
    {
      /* Change character representation positions */
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
    
  /* Draw characters on the frame buffer */
  nuDebConDisp(NU_SC_SWAPBUFFER);

  /* Switch display list buffers */
  gfx_gtask_no ^= 1;
}


/* The game progressing process for stage 0 */
void updateGame00(void)
{  
  static float vel = 1.0;

  /* The game progressing process for stage 0 */
  nuContDataGetEx(contdata,0);

  soundCheck();

  /* Change the display position by stick data */
  triPos_x = contdata->stick_x;
  triPos_y = contdata->stick_y;

  /* The reverse rotation by the A button */
  if(contdata[0].trigger & A_BUTTON)
    {
      vel = -vel;
      osSyncPrintf("A button Push\n");
    }

  /* Rotate fast while the B button is pushed */
  if(contdata[0].button & B_BUTTON)
    theta += vel * 3.0;
  else
    theta += vel;
}

/* The vertex coordinate */
static Vtx shade_vtx[] =  {
        {        -64,  64, -5, 0, 0, 0, 0, 0xff, 0, 0xff	},
        {         64,  64, -5, 0, 0, 0, 0, 0, 0, 0xff	},
        {         64, -64, -5, 0, 0, 0, 0, 0, 0xff, 0xff	},
        {        -64, -64, -5, 0, 0, 0, 0xff, 0, 0, 0xff	},
};

/* Drew a square */
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


/* Provide playback and control of audio by the button of the controller */
void soundCheck(void)
{
  static int snd_no = 0;
  static int seq_no = 0;

  /* Change music of sequence playback depending on the top and bottom of 
  the cross key */
  if((contdata[0].trigger & U_JPAD) || (contdata[0].trigger & D_JPAD))
    {
      if(contdata[0].trigger & U_JPAD)
	{
	  seq_no--;
	  if(seq_no < 0) seq_no = 2;
	}
      else
	{
	  seq_no++;
	  if(seq_no > 2) seq_no = 0;
	}	  

      nuAuSeqPlayerStop(0);
      nuAuSeqPlayerSetNo(0,seq_no);
      nuAuSeqPlayerPlay(0);
    }

  /* Possible to play audio in order by right and left of the cross key */
  if((contdata[0].trigger & L_JPAD) || (contdata[0].trigger & R_JPAD))
    {
      if(contdata[0].trigger & L_JPAD)
	{
	  snd_no--;
	  if(snd_no < 0) snd_no = 10;
	}
      else
	{
	  snd_no++;
	  if(snd_no > 10) snd_no = 0;
	}	  

      /* Eleven sounds (sound data items) are provided.  Of these, the first 10 are sampled at 44 KHz and the 11th at 24 KHz. */
      nuAuSndPlayerPlay(snd_no); 
      if(snd_no < 10)
	nuAuSndPlayerSetPitch(44100.0/32000);
      else
	nuAuSndPlayerSetPitch(24000.0/32000);
    }

  /* Change tempo of sequence playback by the L and R buttons */
  if((contdata[0].trigger & L_TRIG) || (contdata[0].trigger & R_TRIG))
    {
      s32 tmp;
      tmp = nuAuSeqPlayerGetTempo(0);

      if(contdata[0].trigger & L_TRIG)
	{
	  tmp /= 10;
	  tmp *= 8;
	}
      else
	{
	  tmp /= 10;
	  tmp *= 12;
	}
      nuAuSeqPlayerSetTempo(0,tmp);
    }

  /* Fade out sound by pushing the Z button */
  if(contdata[0].trigger & Z_TRIG)
    {
      nuAuSeqPlayerFadeOut(0,200);
    }
}
