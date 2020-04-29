
#include  <nusys.h>

#include  "define.h"

static void   vsyncCallback(int pendingTaskCount);
extern void   Draw(void);

/* Main function */
void  mainproc(void *dummy)
{
  /* Initialize graphics */
  nuGfxInit();
  
  /* Set special VI functions */
  osViSetSpecialFeatures(OS_VI_GAMMA_OFF |
                         OS_VI_GAMMA_DITHER_OFF |
                         OS_VI_DIVOT_OFF |
                         OS_VI_DITHER_FILTER_OFF);
  
  /* Register vertical retrace callback */
  nuGfxFuncSet((NUGfxFunc)vsyncCallback);
  
  /* Video output ON */
  nuGfxDisplayOn();
  
  while(1);
}

void  vsyncCallback(int pendingTaskCount)
{
  /* If there are no tasks for which processing is not completed, register the 
next graphics task */
  if(pendingTaskCount < 1)
    Draw();
}
