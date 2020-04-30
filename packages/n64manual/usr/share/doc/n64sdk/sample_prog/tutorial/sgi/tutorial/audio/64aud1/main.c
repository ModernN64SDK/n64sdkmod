/*******************************************************************
 *
 *  main.c
 *
 *******************************************************************/

#include  <nusys.h>

static void   vsyncCallback(s32 pendingTaskCount);
extern void   Menu(void);

/* Main functions */
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
  
  /* Register vertical retrace callback function */
  nuGfxFuncSet((NUGfxFunc)vsyncCallback);
  
  /* Video output ON */
  nuGfxDisplayOn();
  
  while(1);
}

void  vsyncCallback(s32 pendingTaskCount)
{
  /* If there are no tasks for which processing is not completed, register the next graphics task */
  if(pendingTaskCount < 1)
    Menu();
}
