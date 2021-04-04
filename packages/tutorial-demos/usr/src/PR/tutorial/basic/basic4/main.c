/*******************************************************************
 *
 *  main.c
 *
 *******************************************************************/

#include  <nusys.h>

static void   vsyncCallback(int pendingTaskCount);
extern void   Draw(void);

/* So-called "main" functions */
void  mainproc(void * dummy)
{
  u8          pattern;
  extern u8   IsPlugged;
  
  /* Initialize graphics */
  nuGfxInit();
  
  /* Initialize controller and discover connection status of port 0 */
  pattern = nuContInit();
  IsPlugged = pattern & 1;
  
  /* Register vertical retrace callback */
  nuGfxFuncSet((NUGfxFunc)vsyncCallback);
  
  /* Video display ON */
  nuGfxDisplayOn();
  
  /* Since vsyncCallback will hereafter perform drawing processing on every vertical 
     retrace, mainproc is put into an infinte loop */
  while(1);
}

void  vsyncCallback(int pendingTaskCount)
{
  /* If there are no more tasks for which processing has not been completed, register
the next graphics task */
  if(pendingTaskCount < 1)
    Draw();
}
