/*
   main.c

   NuSYSTEM sample nu1

   Copyright (C) 1997-1999, NINTENDO Co,Ltd.	
   */

#include <nusys.h>
#include "main.h"

/* Definition of the prototype */
void stage00(int);

/* Definition of the external function */
void initStage00(void);
void makeDL00(void);
void updateGame00(void);
void  appGfxInit(void);

/* The global variable */
NUContData	contdata[1]; /* Read data of 1 controller */
u8 contPattern;		     /* The pattern of the connected controller */

/*------------------------
	Main
--------------------------*/
void mainproc(void)
{
  /* The initialization of graphic */
  appGfxInit();

  /* The initialization of the controller manager */
  contPattern = nuContInit();

  /* The initialization of stage00() */
  initStage00();
  /* Call-back Register */
  nuGfxFuncSet((NUGfxFunc)stage00);
  /* screen display on */
  nuGfxDisplayOn();

  while(1)
    ;
}

/*-----------------------------------------------------------------------------
  The call-back function 

  pendingGfx which is passed from Nusystem as the argument of the call-back
  function is the total number of RCP tasks that are currently proccessing and
  waiting for the process
  -----------------------------------------------------------------------------*/
void stage00(int pendingGfx)
{
  /* Provide the display porcess if 2 or less RCP tasks are processing or
	waiting for the process */
  if(pendingGfx < 3)
    makeDL00();		

  /* The process of game progress */
  updateGame00(); 
/*  osSyncPrintf("aaa\n"); */
}

