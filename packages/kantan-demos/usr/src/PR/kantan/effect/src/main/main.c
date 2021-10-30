/*============================================================================
  main.c
  ============================================================================*/

#include <ultra64.h>
#include <PR/ramrom.h>
#include <assert.h>

#include "def.h"
#include "segment.h"
#include "message.h"
#include "nnsched.h"
#include "main.h"
#include "audio.h"

void InitMain( void );

/*------------------------
  work
  --------------------------*/
NNSched sched;
int main_no;

/* Declare external function */
extern int main00(NNSched* );
extern int main01(NNSched* );

/*------------------------
  Main process
  --------------------------*/
void mainproc(void* arg)
{
    /*Perform various initialization */
    InitMain();
    
    /* Main loop */
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

    /*Initialize sound player */
    auSndPlayerInit(_sfxbankSegmentRomStart,
		    _sfxbankSegmentRomEnd,
		    _sfxtableSegmentRomStart);

    /* Create scheduler thread */
    nnScCreateScheduler(&sched, OS_VI_NTSC_LAN1, 1);

    /* Create audio thread */
    auCreateAudioThread(&sched);

    main_no = GM_MAIN;
}
