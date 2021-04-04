/*
   main.c

   NuSYSTEM sample nu5

   Copyright (C) 1997-1999, NINTENDO Co,Ltd.
*/

#include <nusys.h>
#include <nualsgi_n.h>
#include "main.h"
#include "segment.h"

/* Stage number */
volatile int stage;

/* Declare proto types */
void stage00(int);
void stage01(int);
void setAudioData(void);

/* Declare external functions */
void initStage00(void);
void makeDL00(void);
void updateGame00(void);

void initStage01(void);
void makeDL01(void);
void updateGame01(void);

NUContData	contdata[1]; /* Data to be read from 1 controller */

/* Whether the microcode uses XBUS or not */
volatile int now_xbusflag = 0;
volatile int next_xbusflag = 0;
/* FIFO size after modification (The initial value's been set as minimum) */
volatile int now_fifosize = NU_GFX_RDP_OUTPUTBUFF_SIZE;
volatile int next_fifosize = NU_GFX_RDP_OUTPUTBUFF_SIZE>>4;

/* Number of frames per second */
int dspcount = 0;
/* Buffer for console display */
char conbuf[40];

/*------------------------
	Main
--------------------------*/
void mainproc(void)
{
  /* Initialize graphics (with the special function) */
  appGfxInit();

  /* Initialize controller manager */
  nuContInit();
	
  /* Initialize audio */
  nuAuInit();
  /* Register the audio data in ROM */
  setAudioData();

  nuAuSeqPlayerSetNo(0,0);
  nuAuSeqPlayerPlay(0);

  /* Set 0 as the stage number */
  stage = 0;

  while(1)
    {
      switch(stage)
	{
	  /* 
	     Register appropriate callback funstions, according to
		 the stage numbers. The callback functions set values
		 to the stage when other callback functions needs
		 to be registered and modified.
	     */
	case 0:
	  /* Set the stage to -1 to wait for the callback
		 function to set values. */
	  stage = -1;
	  /* Initialize stage0 */
	  initStage00();
	  /* Register callback */
	  nuGfxFuncSet((NUGfxFunc)stage00);
	  /* Start displaying */
	  nuGfxDisplayOn();
	  break;
	default:
	  break;
	}
      
      /* Wait for the callback function to change the value (change the scene) */
      while(stage == -1)
	;
      /* Turn off displaying */
      nuGfxDisplayOff();
    }
}

/* Set audio data */
void setAudioData(void)
{
  /* Register sequence player to the bank */
  nuAuSeqPlayerBankSet(_midibankSegmentRomStart,
		       _midibankSegmentRomEnd - _midibankSegmentRomStart,
		       _miditableSegmentRomStart);
  /* Register MIDI sequence data to the sequence player */
  nuAuSeqPlayerSeqSet(_seqSegmentRomStart);
  /* Register bank to the sound player */
  nuAuSndPlayerBankSet(_sfxbankSegmentRomStart,
		       _sfxbankSegmentRomEnd - _sfxbankSegmentRomStart,
		       _sfxtableSegmentRomStart);
}


/*-----------------------------------------------------------------------------
  Callback function

  pendingGfx passed from Nusystem as a parameter for the callback function, 
  is the total number of RCP tasks that are currently processing or pending.
-----------------------------------------------------------------------------*/
/* This function returns the number of frames displayed in the last one second */
int calcFrame(void)
{
  static char frame[60];
  static int ptr = 0;
  static s64 prev = 0,now;
  int i,ctr;

  now = nuScRetraceCounter;
  if(prev >= now)
    now += 0x100000000LL;
  if(now - prev > 60)
    prev = now;
  while(prev < now-1)
    {
      frame[ptr++] = 0;
      ptr = ptr % 60;
      prev++;
    }
  frame[ptr++] = 1;
  ptr = ptr % 60;
  prev = now & 0xFFFFFFFF;
  ctr = 0;
  for(i = 0; i < 60; i++)
    ctr += frame[i];
  return(ctr);
}

/*
  This function changes FIFO buffer size
*/
void changeUcodeFifo(void)
{
    if(!next_xbusflag)
      nuGfxSetUcodeFifo(nuRDPOutputBuf, next_fifosize);
    /* Update completes */
    now_xbusflag = next_xbusflag;
    now_fifosize = next_fifosize;
}

/* Stage0 */
void stage00(int pendingGfx)
{
  /* If necessary, it changes the number of frame buffers */
  if( now_xbusflag != next_xbusflag || now_fifosize != next_fifosize )
  {
    if(pendingGfx == 0) /* No pending display list */
      changeUcodeFifo();
    return; /* Do not launch a new task when this routine is in use */
  }

  /* Display process's performed, depending on the number of processing/pending RCP tasks */
  if(pendingGfx < 4) /* The number of Dls used in stage00 is 3 kinds x 2 buffers */
    {
      dspcount = calcFrame();
      makeDL00();
    }

  /* Game status process */
  updateGame00(); 
}
