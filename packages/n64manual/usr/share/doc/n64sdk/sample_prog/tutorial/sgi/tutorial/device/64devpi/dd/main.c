/*******************************************************************
 *
 *  main.c
 *
 *******************************************************************/

#include  <nusys.h>
#include  <mfs.h>

#include  "define.h"
#include  "ddmenu.h"

static void   vsyncCallback(s32 pendingTaskCount);

/* Main functions */
void  mainproc(void *dummy)
{
  extern MfsFileHandle  fileHandle[MFS_HANDLES];
  extern u8             failed;
  s32                   errorCode;
  
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
  
  /*
   *  Initialize MFS
   */
  if((errorCode = mfsHInitDiskBoot(fileHandle,
                                   MFS_HANDLES,
                                  "64",
                                  "TEST",
                                  MFS_DESTINATION_JAPAN)) != MFS_ERR_NO)
  {
    failed = 1;
    /* Error process */
    osSyncPrintf("mfsHInitDiskBoot : ");
    HandleMfsError(errorCode);
  }
  
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
    DdMenu();
}
