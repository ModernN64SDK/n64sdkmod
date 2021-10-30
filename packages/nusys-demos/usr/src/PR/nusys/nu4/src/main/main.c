/*
   main.c

   NuSYSTEM sample nu4

   Copyright (C) 1997-1999, NINTENDO Co,Ltd.	
   */

#include <nusys.h>
#include "main.h"
#include "segment.h"

#ifdef N_AUDIO
#include <nualstl_n.h>
#else
#include <nualstl.h>
#endif

/* Definition of the prototype */
void stage00(int);
void initAudio(void);

/* Definition of the external function */
void initStage00(void);
void makeDL00(void);
void updateGame00(void);

/* The global variable */
NUContData contdata[1];		/* Read data of 1 controller */
u8 contPattern;	     /* The bit pattern of the connected controller */

int ptr_buf[NU_AU_SAMPLE_SIZE]__attribute__((aligned(16)));
int tune_buf[NU_AU_SONG_SIZE]__attribute__((aligned(16)));
int sfx_buf[NU_AU_SE_SIZE]__attribute__((aligned(16)));

/*------------------------
	Main
--------------------------*/
void mainproc(void)
{
  /* The initialization of graphic */
  nuGfxInit();

  /* The initialization of the controller manager */
  contPattern = nuContInit();

  /* The initialization of audio */
  /* Register audio data on ROM */
  initAudio();

  /* The initialization for stage00() */
  initStage00();
  /* Call-back register */
  nuGfxFuncSet((NUGfxFunc)stage00);
  /* Screen display ON */
  nuGfxDisplayOn();

  while(1)
    ;
}

/* Initialize audio. */
void initAudio(void)
{
    musConfig c;

    c.control_flag	= 0; /* Set to MUSCONTROL_RAM if wbk file also placed in RAM */
    c.channels		= NU_AU_CHANNELS; /* Maximum total number of channels */
    c.sched		= NULL; /* Ignore */
    c.thread_priority	= NU_AU_MGR_THREAD_PRI; /* Usually not changed */
    c.heap		= (unsigned char*)NU_AU_HEAP_ADDR; /* Usually not changed */
    c.heap_length	= NU_AU_HEAP_SIZE;
    c.ptr		= NULL; /* Can set the default ptr file */
    c.wbk		= NULL; /* Can set the default wbk file */
    c.default_fxbank	= NULL; /* Can set the default bfx file  */
    c.fifo_length	= NU_AU_FIFO_LENGTH; /* Usually not changed */
    c.syn_updates	= NU_AU_SYN_UPDATE_MAX;
    c.syn_output_rate	= 44100;
	/* Usually set to NU_AU_OUTPUT_RATE=32kHz or less*/
    c.syn_rsp_cmds	= NU_AU_CLIST_LEN;
    c.syn_retraceCount	= 1; /* Usually not changed */
    c.syn_num_dma_bufs	= NU_AU_DMA_BUFFER_NUM;
    c.syn_dma_buf_size	= NU_AU_DMA_BUFFER_SIZE;

    /* Initialize the Audio Manager. */
    nuAuStlMgrInit(&c);

    /* Register the PRENMI function.*/
    nuAuPreNMIFuncSet(nuAuPreNMIProc);

}

/*-----------------------------------------------------------------------------
  The call-back function

  pincingGfx which is passed from Nusystem as the argument of the call-back
  function is the total number of RCP tasks that are currently processing
  and waiting for the process.
-----------------------------------------------------------------------------*/
void stage00(int pendingGfx)
{
  /* Provide the display process if 2 or less RCP tasks are processing or
	waiting for the process */
  if(pendingGfx < 3)
    makeDL00();		

  /* The process of game progress */
  updateGame00(); 
}
