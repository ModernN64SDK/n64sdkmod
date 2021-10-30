/*
   main.c

   NuSYSTEM sample nu1

   Copyright (C) 1997-1999, NINTENDO Co,Ltd.			
 */

#include <nusys.h>
#include "main.h"

/* Prototype declaration */
void stage00(int);

/* Declare external variables */
void initStage00(void);
void makeDL00(void);
void updateGame00(void);

/* Global variables */
NUContData	contdata[1]; /* Data to be read for one controller */
u8 contPattern;		     /* Pattern for the connected controllers */

/*------------------------
	Main
--------------------------*/
void mainproc(void)
{
  /* Initialize graphics. */
  nuGfxInit();

  /* Initialize controller manager */
  contPattern = nuContInit();

  /* Initialize for stage00() */
  initStage00();
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
  if(pendingGfx < 3)
    makeDL00();		

  /* Game status process */
  updateGame00(); 
}

