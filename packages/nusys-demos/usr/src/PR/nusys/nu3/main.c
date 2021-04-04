/*
   main.c

   NuSYSTEM sample nu3 

   Copyright (C) 1997-1999, NINTENDO Co,Ltd.	
   */

#include <nusys.h>
#include "main.h"
#include "segment.h"

#ifdef N_AUDIO
#include <nualsgi_n.h>
#else
#include <nualsgi.h>
#endif

/* Declaration of the prototype  */
void stage00(int);
void setAudioData(void);

/* Declaration of the external function  */
void initStage00(void);
void makeDL00(void);
void updateGame00(void);

/* The global variable  */
NUContData contdata[1];		/* Read data of 1 controller  */
u8 contPattern;	     /* The bit pattern of the connected controller  */

/*------------------------
	Main
--------------------------*/
void mainproc(void)
{
  /* The initialization of graphic  */
  nuGfxInit();

  /* The initialization of the controller manager  */
  contPattern = nuContInit();

  /* The initialization of audio  */
  nuAuInit();
  /* Register audio data on ROM  */
  setAudioData();

  /* The initialization for stage00()  */
  initStage00();
  /* Call-back register  */
  nuGfxFuncSet((NUGfxFunc)stage00);
  /* Screen display ON*/
  nuGfxDisplayOn();

  while(1)
    ;
}

/* Set audio data  */
void setAudioData(void)
{
  /* Register the bank to the sequence player  */
  nuAuSeqPlayerBankSet(_midibankSegmentRomStart,
		       _midibankSegmentRomEnd - _midibankSegmentRomStart,
		       _miditableSegmentRomStart);
  /* Register MIDI sequence data to the sequence player */
  nuAuSeqPlayerSeqSet(_seqSegmentRomStart);
  /* Register the bank to the sound player  */
  nuAuSndPlayerBankSet(_sfxbankSegmentRomStart,
		       _sfxbankSegmentRomEnd - _sfxbankSegmentRomStart,
		       _sfxtableSegmentRomStart);
}

/*-----------------------------------------------------------------------------
  The call-back function 

  pendingGfx which is passed from Nusystem as the argument of the call-back 
  function is the total number of RCP tasks that are currently processing
  and waiting for the process.  
-----------------------------------------------------------------------------*/
void stage00(int pendingGfx)
{
  /* Provide the display process if 2 or less RCP tasks are processing or 
	waiting for the process.  */
  if(pendingGfx < 3)
    makeDL00();		

  /* The process of game progress */
  updateGame00(); 
}


