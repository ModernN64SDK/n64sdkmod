/*
   stage00.c 

   Copyright (C) 1997-1999, NINTENDO Co,Ltd.	
*/

#include <assert.h>
#include <nusys.h>
#include "main.h"
#include "graphic.h"
#include "segment.h"

#ifdef N_AUDIO
#include <nualstl_n.h>
#else
#include <nualstl.h>
#endif

static float theta;  /* The rotational angle of the square */
static float triPos_x; /* The display position, X */
static float triPos_y; /* The display position, Y */

static musHandle	seqHandle = 0;
static musHandle	sndHandle = 0;

u8	*song_start_list[] = {
    MUSIC1_START,
    MUSIC2_START,
    MUSIC3_START,
};

u8	*song_end_list[] = {
    MUSIC1_END,
    MUSIC2_END,
    MUSIC3_END,
};

/* Declaration of the prototype */
void shadetri(Dynamic* dynamicp);
void soundCheck(void);
void Rom2Ram(void *, void *, s32);

/* The initialization of stage 0 */
void initStage00(void)
{
  triPos_x = 0.0;
  triPos_y = 0.0;
  theta = 0.0;

  /* Read and register the sample bank. */
  Rom2Ram((void *)PBANK_START, (void *)ptr_buf, PBANK_END-PBANK_START);
  MusPtrBankInitialize(ptr_buf, WBANK_START);

  /* Read and register the sound effects. */
  Rom2Ram((void *)SFX_START, (void *)sfx_buf, SFX_END-SFX_START);
  MusFxBankInitialize(sfx_buf);

}

/* Make the display list and activate the task */
void makeDL00(void)
{
  Dynamic* dynamicp;
  char conbuf[20]; 

  /* Specify the display list buffer */
  dynamicp = &gfx_dynamic[gfx_gtask_no];
  glistp = &gfx_glist[gfx_gtask_no][0];

  /*  The initialization of RCP */
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

  /* Draw a square */
  shadetri(dynamicp);

  gDPFullSync(glistp++);
  gSPEndDisplayList(glistp++);

  assert((glistp - gfx_glist[gfx_gtask_no]) < GFX_GLIST_LEN);

  /* Activate the task and
     switch display buffers */
  nuGfxTaskStart(&gfx_glist[gfx_gtask_no][0],
		 (s32)(glistp - gfx_glist[gfx_gtask_no]) * sizeof (Gfx),
		 NU_GFX_UCODE_F3DEX , NU_SC_NOSWAPBUFFER);

  if(contPattern & 0x1)
    {
      /* Change the character display position. */
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

  /* Data reading of controller 1 */
  nuContDataGetEx(contdata,0);

  soundCheck();

  /* Change the display position according to stick data */
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

  if(theta>360.0)
      theta -= 360.0;
  else if (theta<0.0)
      theta += 360.0;
      
}

/* The vertex coordinate */
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


/* Provide playback and control of audio by the button of the controller */
void soundCheck(void)
{
  static int snd_no = 0;
  static int seq_no = 0;
  static int start_song_flag = 0;

  /* The sequence is actually stopped until the next frame.  After a wait of 1 frame, the sequence is loaded by DMA (overwrite).  */

  /* If the sequence was stopped in the previous frame, a new sequence is started. */
  if(start_song_flag)
    {
      /* Read the sequences. */
      Rom2Ram((void *)song_start_list[seq_no], (void *)tune_buf, song_end_list[seq_no]-song_start_list[seq_no]);
      /* Play a sequence. */
      seqHandle = MusStartSong(tune_buf);

      /* Reset the flag. */
      start_song_flag = 0;
    }

  /* Possible to play audio in order by right and left of the cross key */
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

      MusStop(MUSFLAG_SONGS,0);

      /* Set the flag so that a new sequence begins at the next frame. */
      start_song_flag = 1;
    }


  /* The order in which audio is played can be determined using the right and left sides of the cross key. */
  if((contdata[0].trigger & L_JPAD) || (contdata[0].trigger & R_JPAD))
    {
      if(snd_no)
	MusHandleStop(sndHandle,0);

      if(contdata[0].trigger & L_JPAD)
	{
	  snd_no--;
	  if(snd_no < 1) snd_no = 5;
	}
      else
	{
	  snd_no++;
	  if(snd_no > 5) snd_no = 1;
	}	  

      sndHandle = MusStartEffect(snd_no);

    }

  /* Change tempo of sequence playback by L and R buttons */
  if((contdata[0].trigger & L_TRIG) || (contdata[0].trigger & R_TRIG))
    {
      if(contdata[0].trigger & L_TRIG)
	MusHandleSetTempo(seqHandle,0x100);
      else
	MusHandleSetTempo(seqHandle,0x40);
    }

  /* Stop sequence playback by the Z button */
  if( contdata[0].trigger & Z_TRIG )
    MusHandleStop(seqHandle,200);
}


/*----------------------------------------------------------------------*/
/*	Rom2Ram - Reads data from ROM into RAM			*/
/*	IN:	from_addr	The source address of the data in ROM		*/
/*		to_addr		The destination address of the data in RAM */
/*		seq_size	      	Size of the data to read			*/
/*	RET:	Nothing							*/
/*----------------------------------------------------------------------*/
void Rom2Ram(void *from_addr, void *to_addr, s32 seq_size)
{
    /* Cannot transfer if size is an odd number, so make it an even number. */
    if(seq_size & 0x00000001) seq_size++;

    nuPiReadRom((u32)from_addr, to_addr, seq_size);
}
