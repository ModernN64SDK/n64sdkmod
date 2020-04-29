/*******************************************************************
 *
 *  main.c
 *
 *******************************************************************/

#include  <nusys.h>

static void   vsyncCallback(int pendingTaskCount);
extern void   Draw(void);

/* So-called "main" function */
void  mainproc(void * dummy)
{
  /* Initialize graphics */
  nuGfxInit();
  
  /* Register vertical retrace callback */
  nuGfxFuncSet((NUGfxFunc)vsyncCallback);
  
  /* Video output ON */
  nuGfxDisplayOn();
  
  /* Since vsyncCallback will hereafter perform drawing processing on every vertical 
     retrace, mainproc is put into an infinte loop */
  while(1);
}

void  vsyncCallback(int pendingTaskCount)
{
  Draw();
}
