/*============================================================================

		NINTENDO64 TECHNICAL SUPPORT CENTER 
		
		    NINTENDO64 SAMPLE PROGRAM 2

		Copyright (C) 1997, NINTENDO Co., Ltd.

============================================================================*/

#include <ultra64.h>
#include <PR/ramrom.h>
#include <assert.h>

#include "def.h"
#include "vram.h"
#include "segment.h"
#include "message.h"
#include "nnsched.h"
#include "main.h"
#include "controller.h"
#include "audio.h"
#include "graphic.h"


NNSched sched;
int main_no;

/* exxternal function declaration */
extern int main00(NNSched* );

/*------------------------
	mainproc
--------------------------*/
void mainproc(void* arg)
{

  /* initialize audio */
  auAudioInit();

  /* initialize sequence player */
  auSeqPlayerInit(_midibankSegmentRomStart, _midibankSegmentRomEnd,
		  _seqSegmentRomStart, _seqSegmentRomEnd,
		  _miditableSegmentRomStart);

  /* initialize sound player */
  auSndPlayerInit(_sfxbankSegmentRomStart, _sfxbankSegmentRomEnd,
		  _sfxtableSegmentRomStart);


  /* create scheduler thread */
  nnScCreateScheduler(&sched, OS_VI_NTSC_LAN1, 1);

  /* create audio thread */
  auCreateAudioThread(&sched);

  /* initialize graphic library */
  gfxInit(_gfxdlistSegmentBssStart);

  /* create graphic thread */
  gfxCreateGraphicThread(&sched);

  /*
     Set VI interface.
     Specify OS_VI_DITHER_FILTER_ON in order to use DITHER filter
  */
  osViSetSpecialFeatures(OS_VI_DITHER_FILTER_ON | OS_VI_GAMMA_OFF
				 | OS_VI_GAMMA_DITHER_OFF);

  /* initialize controller (use max 4 controller)*/
  controllerInit(4);
  controllerThreadMake(&sched);

  main_no = MAIN_00;

  /* game main loop */
  while(1){
    switch(main_no){
    case MAIN_00:   /* main 00 */
      main_no = main00(&sched);
      break;
    case MAIN_01:   /* main 01 */
      break;
    case MAIN_02:
	break;
    case MAIN_03:
	break;
    case MAIN_04:
      break;
   }
  }  /* end of while */
}


