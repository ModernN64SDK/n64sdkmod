/*
   main.c

   NuSYSTEM sample nu0

   Copyright (C) 1997-1999, NINTENDO Co,Ltd.
   */

#include <nusys.h>
#include "nupakmenu.h"

/* Prototype declaration */
void stage00(int);
void makeDL00(void);

/*------------------------
	Main
--------------------------*/
void mainproc(void)
{
    s32 rtn;
  /* Initialize graphics. */
  nuGfxInit();
  nuContInit();

  rtn = nuPakMenu((void*)0x80200000, (void*)0x80300000);
    osSyncPrintf("mainproc: nuPakMenu rtn = %d\n", rtn);
  

  /* Callback registration */
  nuGfxFuncSet((NUGfxFunc)stage00);
  /* Screen display ON */
  nuGfxDisplayOn();

  while(1)
    ;
}

/*-----------------------------------------------------------------------------
  Callback function

pendingGfx is passed from Nusystem as a callback function argument and represents the total number of RCP tasks being processed or waiting to be processed.
-----------------------------------------------------------------------------*/
void stage00(int pendingGfx)
{
  /* Perform display processing if there are no RCP tasks being processed. */
  if(pendingGfx < 1)
    makeDL00();		
}

