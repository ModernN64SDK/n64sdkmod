/*============================================================================
  main.c
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
#include "audio.h"
#include "graphic.h"


void InitMain( void );

/*------------------------
  work
  --------------------------*/
NNSched sched;
int main_no;

/* Declare external function */
extern int main00(NNSched* );
/*------------------------
 Main process
  --------------------------*/
void mainproc(void* arg)
{
    /*Initialize various kinds*/
    InitMain();
    
    /*main loop */
    while(1){
	switch(main_no){
	  case GM_MAIN:
	    main_no = main00(&sched);
	    break;
	}
    }
}

void InitMain( void )
{
    /* Initialize audio player */
    auAudioInit();

    /* Initialize sequence player */
    auSeqPlayerInit(_midibankSegmentRomStart,
		    _midibankSegmentRomEnd,
		    _seqSegmentRomStart,
		    _seqSegmentRomEnd,
		    _miditableSegmentRomStart);


    /* Create scheduler thread */
    nnScCreateScheduler(&sched, OS_VI_NTSC_LAN1, 1);

    /* Create audio thread */
    auCreateAudioThread(&sched);

    /* Initialize graphic library */
    gfxInit(_gfxdlistSegmentBssStart);

    /* Create graphic thread */
    gfxCreateGraphicThread(&sched);

    /* Set VI interface */
    /* Make DITHER filter usable by specifying OS_VI_DITHER_FILTER_ON */

    osViSetSpecialFeatures(OS_VI_DITHER_FILTER_ON |
			   OS_VI_GAMMA_OFF |
			   OS_VI_GAMMA_DITHER_OFF);
    main_no = GM_MAIN;
}
