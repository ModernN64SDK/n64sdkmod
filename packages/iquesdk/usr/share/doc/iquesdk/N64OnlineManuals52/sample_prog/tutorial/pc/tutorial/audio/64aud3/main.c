/*******************************************************************
 *
 *  main.c
 *
 *******************************************************************/

#include  <nusys.h>

volatile s32  stage;
  
static void   vsyncCallback0(s32 pendingTaskCount);
static void   vsyncCallback1(s32 pendingTaskCount);
extern void   Menu0(void);
extern void   Menu1(void);

/* Main Function */
void  mainproc(void *dummy)
{
  /* Initialize graphics */
  nuGfxInit();
  
  /*
   *  Initialize controller
   *    Internally 
   *      nuSiMgrInit           Initialize and launch SI Manager
   *      nuContMgrInit         Initialize and register Controller Manager
   *      nuContPakMgrInit      Initialize and register Controller Pak Manager
   *      nuContRmbMgrInit      Initialize and register Rumble Pak Manager
   *    are called 
   */
  nuContInit();
  
  /* Initialize sound */
  InitializeAudio();
  
  /* Set default value for stage number to 0 */
  stage = 0;
  
  while(1)
  {
    switch(stage)
    {
    case 0:
      /* Initialize Mode 0 sounds */
      InitializeAudio0();
      /* Register vertical retrace callback */
      nuGfxFuncSet((NUGfxFunc)vsyncCallback0);
      /* Video output ON */
      nuGfxDisplayOn();
      break;
    case 1:
      /* Initialize Mode 1 sounds */
      InitializeAudio1();
      /* Register vertical retrace callback */
      nuGfxFuncSet((NUGfxFunc)vsyncCallback1);
      /* Video output ON */
      nuGfxDisplayOn();
      break;
    default:  /* This case is impossible */
      break;
    }
    
    /* Set to -1 to detect when a callback function has been changed to a variable */
    stage = -1;
    
    /* Loop until stage is changed */
    while(stage == -1);
    
    /* Completely clear the display when the stage changes */
    nuGfxDisplayOff();
  }
}

void  vsyncCallback0(s32 pendingTaskCount)
{
  /* If there are no tasks that are not completed, register the next graphics task */
  if(pendingTaskCount < 1)
    Menu0();
}

void  vsyncCallback1(s32 pendingTaskCount)
{
  /* If there are no tasks that are not completed, register the next graphics task */
  if(pendingTaskCount < 1)
    Menu1();
}
