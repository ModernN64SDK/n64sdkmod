/*
   main.c

   NuSYSTEM sample nuhvqm1
   */

#include <nusys.h>
#include "main.h"
#include "hvqm.h"

/* Stage number */
volatile int stage;

/* Prototype declarations */
void stage00(int);
void stage01(int);

/* External function declarations */
void initStage00(void);
void initStage01(void);
void makeDL00(void);
void makeDL01(void);
void updateGame00(void);
void updateGame01(void);

/* Global variables */
NUContData	contdata[1]; /* Data read from 1 Controller */
u8 contPattern;		     /* Pattern of connected Controllers */

/*------------------------
	Main
--------------------------*/
void mainproc(void)
{
  /* Initialize graphic */
  hvqmGfxInit();

  /* Initialize Controller manager */
  contPattern = nuContInit();

  /* Initialize for stage00() */
  initStage00();
  /* Register callback function */
  nuGfxFuncSet((NUGfxFunc)stage00);
  /* Turn screen display ON */
  nuGfxDisplayOn();

  /* Initialize for HVQM */
  hvqmInit();

  stage = 0;

  while(1)
  {
      switch(stage)
      {
      case 0:
	  stage = -1;

	  /* Initialization for stage00() */
	  initStage00();
	  /* Reset frame buffer */
	  osViSetMode(&osViModeNtscLan1);
	  /* Turn OFF again because executing osViSetMode
	     enables View Screen. */
	  nuGfxDisplayOff();
	  /* Needs to reset other settings */
	  osViSetSpecialFeatures(OS_VI_DITHER_FILTER_ON
				 | OS_VI_GAMMA_OFF
				 | OS_VI_GAMMA_DITHER_OFF
				 | OS_VI_DIVOT_ON);
	  /* Register callback function */
	  nuGfxFuncSet((NUGfxFunc)stage00);
	  /* Turn screen dislpay ON*/
	  nuGfxDisplayOn();
	  break;
      case 1:
	  stage = -1;

	  initStage01();
	  /* Added for frame buffer resetting */
#if HVQM_CFB_FORMAT == 1
	  osViSetMode(&osViModeNtscLan1);
#else /* HVQM_CFB_FORMAT */
	  osViSetMode(&osViModeNtscLan2);
#endif /* HVQM_CFB_FORMAT */
	  /* Turn OFF again because executing osViSetMode
	     enables View Screen. */
	  nuGfxDisplayOff();
	  /* Needs to reset other settings */
	  osViSetSpecialFeatures(OS_VI_DITHER_FILTER_ON
				 | OS_VI_GAMMA_OFF
				 | OS_VI_GAMMA_DITHER_OFF
				 | OS_VI_DIVOT_ON);
	  /* Register callback */
	  nuGfxFuncSet((NUGfxFunc)stage01);
	  nuGfxDisplayOn();
	  break;
      default:
	  break;
      }

      while(stage == -1);

      nuGfxDisplayOff();
  }
}

/*-----------------------------------------------------------------------------
  Callback function

 pendingGfx, which is passed from Nusystem as the argument of the callback
 function, is the sum total of RCP tasks currently processing and 
 waiting to be processed.  
  
-----------------------------------------------------------------------------*/
void stage00(int pendingGfx)
{
  /* Perform display processing if total number of RCP tasks currently
     being processed and waiting to be processed is less than 3 */
  if(pendingGfx < 3)
    makeDL00();		

  /* Game advance process */
  updateGame00(); 
}

void stage01(int pendingGfx)
{
    if(!pendingGfx)
	makeDL01();

    updateGame01();
}
