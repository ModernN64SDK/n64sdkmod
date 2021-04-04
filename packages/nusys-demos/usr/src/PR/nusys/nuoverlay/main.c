/*
   main.c

   NuSYSTEM sample nuoverlay

   Copyright (C) 1997-1999, NINTENDO Co.,Ltd.
*/

#include <nusys.h>
#include <nualsgi_n.h>
#include "graphic.h"
#include "main.h"
#include "segment.h"

/* Stage number  */
volatile int stage;

/* Prototype declarations */
void stage00(int);
void stage01(int);
void setAudioData(void);
void overlay(int);

/* External function declarations  */
void initStage00(void);
void makeDL00(void);
void updateGame00(void);

void initStage01(void);
void makeDL01(void);
void updateGame01(void);

NUContData	contdata[1]; /* Data read from 1 Controller */
/* Value used for pending callback function check -- 0 or 1   */
int pendflag = 0;	
/* Number of frames displayed per second  */
int dspcount = 0;
/* Console display buffer  */
char conbuf[40];

/* for stage00 and stage01 */
float triPos_x; /* Display coordinate X */
float triPos_y; /* Display coordinate Y */
float theta;  /* Angle of rotation of square */

/*------------------------
	Main
--------------------------*/
void mainproc(void)
{
  /* Initialize graphic */
  nuGfxInit();

  /* Initialize Controller manager */
  nuContInit();
	
  /* Initialize audio */
  nuAuInit();
  /* Register the audio data in ROM */
  setAudioData();

  nuAuSeqPlayerSetNo(0,0);
  nuAuSeqPlayerPlay(0);

  /* Set the stage number to 0 */
  stage = 0;

  while(1)
    {
      /* Load the specified stage */
      overlay(stage);

      switch(stage)
	{
	  /* 
	     Register the callback function corresponding to that stage number.
	     This callback function sets a value in stage if a change in 
             registration is necessary in another callback function.
	  */
	case 0:
	  /* Waiting for the callback function to set a value in stage,
             so set the value of stage to -1  */
	  stage = -1;

	  /* Initialize stage 0 */
	  initStage00();
	  /* Register callback */
	  nuGfxFuncSet((NUGfxFunc)stage00);
	  /* Begin display */
	  nuGfxDisplayOn();
	  break;
	case 1:
	  stage = -1;
	  initStage01();
	  nuGfxFuncSet((NUGfxFunc)stage01);
	  nuGfxDisplayOn();
	  break;
	default:
	  break;
	}
      
      /* Wait for callback function to switch value (to switch stage) */
      while(stage == -1)
	;
      /* Turn off display */
      nuGfxDisplayOff();
    }
}

/* Set audio data */
void setAudioData(void)
{
  /* Register bank in sequence player */
  nuAuSeqPlayerBankSet(_midibankSegmentRomStart,
		       _midibankSegmentRomEnd - _midibankSegmentRomStart,
		       _miditableSegmentRomStart);
  /* Register MIDI sequence data in sequence player */
  nuAuSeqPlayerSeqSet(_seqSegmentRomStart);
  /* Register bank in sound player */
  nuAuSndPlayerBankSet(_sfxbankSegmentRomStart,
		       _sfxbankSegmentRomEnd - _sfxbankSegmentRomStart,
		       _sfxtableSegmentRomStart);
}


/*-----------------------------------------------------------------------------
Callback function

 pendingGfx, which is passed from Nusystem as the argument of a callback
 function, is the sum total of RCP tasks currently processing and 
 waiting to be processed.
-----------------------------------------------------------------------------*/

/* Function that returns how many frames were displayed during 
   the past 1 second. */
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

/* Stage 0  */
void stage00(int pendingGfx)
{
  /* Perform display processing based on number of RCP tasks currently
     being processed and waiting to be processed */
  if(pendingGfx < 3+pendflag)
    {
      dspcount = calcFrame();
      makeDL00();
    }

  /* Advance game */
  updateGame00(); 
}

/* Stage 1 */
void stage01(int pendingGfx)
{
  /* Perform display processing based on number of RCP tasks currently
     being processed and waiting to be processed */
  if(pendingGfx < 4+pendflag)
    {
      dspcount = calcFrame();
      makeDL01();
    }

  /* Advance game */
  updateGame01();
}

/*
 * overlay
 *   Overlay with the specified stage.
 * (Note): Code must not be executing when overwritten.
 *         In this sample, the callback function is included
 *      in the contents of the overlay process, so nuGfxFuncRemove()
 *      is called within the callback function before the stage is
 *      switched.
 *
 */
void overlay(int stage)
{
    NUPiOverlaySegment	segment;

    /* Load the specified stage */
    switch(stage)
    {
    case 0:
	segment.romStart	= _stage00SegmentRomStart;
	segment.romEnd		= _stage00SegmentRomEnd;
	segment.ramStart	= _stage00SegmentStart;
	segment.textStart	= _stage00SegmentTextStart;
	segment.textEnd		= _stage00SegmentTextEnd;
	segment.dataStart	= _stage00SegmentDataStart;
	segment.dataEnd		= _stage00SegmentDataEnd;
	segment.bssStart	= _stage00SegmentBssStart;
	segment.bssEnd		= _stage00SegmentBssEnd;
	break;
    case 1:
	segment.romStart	= _stage01SegmentRomStart;
	segment.romEnd		= _stage01SegmentRomEnd;
	segment.ramStart	= _stage01SegmentStart;
	segment.textStart	= _stage01SegmentTextStart;
	segment.textEnd		= _stage01SegmentTextEnd;
	segment.dataStart	= _stage01SegmentDataStart;
	segment.dataEnd		= _stage01SegmentDataEnd;
	segment.bssStart	= _stage01SegmentBssStart;
	segment.bssEnd		= _stage01SegmentBssEnd;
	break;
    default:
	return;
    }

    nuPiReadRomOverlay(&segment);
}
