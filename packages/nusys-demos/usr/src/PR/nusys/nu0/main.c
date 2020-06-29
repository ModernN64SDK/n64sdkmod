/*
   main.c

   NuSYSTEM sample nu0

   Copyright (C) 1997-1999, NINTENDO Co,Ltd.	
   */

#include <nusys.h>

/* Declaration of the prototype */
void stage00(int);
void makeDL00(void);

/*------------------------
	Main
--------------------------*/
void mainproc(void)
{
  /* The initialization of graphic  */
  nuGfxInit();

  /* Register call-back  */
  nuGfxFuncSet((NUGfxFunc)stage00);
  /* The screen display ON */
  nuGfxDisplayOn();

  while(1)
    ;
}

/*-----------------------------------------------------------------------------
  The call-back function 

  pendingGfx which is passed from Nusystem as the argument of the call-back 
  function is the total number of RCP tasks that are currently processing 
  and waiting for the process. 
-----------------------------------------------------------------------------*/
void stage00(int pendingGfx)
{
  /* It provides the display process if there is no RCP task that is processing. */
  if(pendingGfx < 1)
    makeDL00();		
}

