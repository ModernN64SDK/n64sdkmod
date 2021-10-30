/*
 *  NUSYSTEM-HVQ library Sample program 
   main.c
Copyright (C) 1999 NINTENDO Co.,Ltd.
Copyright (C) 1999 MONEGI CORPORATION
*/

#include <nusys.h>
#include "main.h"
#include <nualstl_n.h>

/* Prototype declarations */
void stage00(int);
void initAudio(void);

/* External function declarations  */
void makeDL00(void);
void updateGame00(void);
void initStage00(void);
void appGfxInit(void);

/* Global variables */
NUContData	contdata[1]; /* Data read from 1 Controller */
u8 contPattern;		     /* Pattern of attached Controllers */
int useRSP=0;
int hvqno=1;

/*------------------------
	Main
--------------------------*/
void mainproc(void)
{
  /* Initialize graphics */
	appGfxInit();
  /* Initialize audio */
  /* Register the audio data in ROM */
	initAudio();
  /* Initialize for stage00() */
    initStage00();
  /* Initialize the Controller manager */
	contPattern = nuContInit();
  /* Register callback function */
	nuGfxFuncSet((NUGfxFunc)stage00);
  /* Turn screen display ON */
	nuGfxDisplayOn();
	while(1)
    ;
}
/* Initialize audio */
void initAudio(void)
{
    musConfig c;

    c.control_flag	= 0; /* To MUSCONTROL_RAM if wbk file also in RAM */
    c.channels		= NU_AU_CHANNELS; /* Maximum total number of channels */
    c.sched		= NULL; /* Please ignore this */
    c.thread_priority	= NU_AU_MGR_THREAD_PRI; /* Normally not changed */
    c.heap		= (unsigned char*)NU_AU_HEAP_ADDR; /* Normally not changed */
    c.heap_length	= NU_AU_HEAP_SIZE;
    c.ptr		= NULL; /* Can set initial ptr file */
    c.wbk		= NULL; /* Can set initial wbk file */
    c.default_fxbank	= NULL; /* Can set initial bfx file */
    c.fifo_length	= NU_AU_FIFO_LENGTH; /* Normally not changed */
    c.syn_updates	= NU_AU_SYN_UPDATE_MAX;
    c.syn_output_rate	= 44100; 
             /* NU_AU_OUTPUT_RATE usually set to 32kHz or lower */
    c.syn_rsp_cmds	= NU_AU_CLIST_LEN;
    c.syn_retraceCount	= 1; /* Normally not changed */
    c.syn_num_dma_bufs	= NU_AU_DMA_BUFFER_NUM;
    c.syn_dma_buf_size	= NU_AU_DMA_BUFFER_SIZE;

    /* Initialize audio manager */
    nuAuStlMgrInit(&c);

    /* Register PRENMI function */
    nuAuPreNMIFuncSet(nuAuPreNMIProc);

}

/*-----------------------------------------------------------------------------
  Callback function

 pendingGfx, which is passed from Nusystem as the argument of the callback
 function, is the sum total of RCP tasks currently processing and 
 waiting to be processed.  

-----------------------------------------------------------------------------*/
void stage00(int pendingGfx)
{
  /* Perform display processing if total number of RCP tasks currently
     being processed and waiting to be processed is less than 3 */
  if(pendingGfx < 3)
    makeDL00();		

  /* Game advance process  */
  updateGame00(); 
}

/* end */

