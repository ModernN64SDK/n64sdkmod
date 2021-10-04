/*---------------------------------------------------------------------
        Copyright (C) 1998 Nintendo.
        
        File            main.c
        Coded    by     Koji Mitsunari. Apr 30, 1998.
        Modified by     Koji Mitsunari. Oct 27, 1998.
        Comments        64GB-PAK test program main
   
        $Id: 
   ---------------------------------------------------------------------*/
#include <ultra64.h>
#include "nu64sys.h"

#include "main.h"
#include "siproc.h"		/* For siproc */
#include "graph.h"		/* For screen display */

u8	gb_id[OS_GBPAK_ROM_ID_SIZE];	/* GB ROM registration area */
u8	buffer[DATA_SIZE];		/* Data loading buffer */

static OSMesg   siMsgBuf[SI_MSG_NUM];
static OSThread siThread;
static u64      siStack[STACKSIZE/sizeof(u64)];
static SiMsg 	siMsg;
OSMesgQueue     siMsgQ;

/*---------------------------------------------------------------------
                  Error Message Screen Display
  ---------------------------------------------------------------------*/
void
PrintErrorMessage(s32 errmes) {
  u16	*p = (u16 *)osViGetNextFramebuffer();
  
  switch(errmes) {
  case GBPAK_ERR_MES_NOPACK:
    printmes(p, ERR_X, ERR_Y1, WHITE, mes0);
    printmes(p, ERR_X, ERR_Y2, WHITE, mes1);
    printmes(p, ERR_X, ERR_Y3, WHITE, mes2);
    break;
  case GBPAK_ERR_MES_ERR_DEVICE:
    printmes(p, ERR_X, ERR_Y1, WHITE, mes4);
    printmes(p, ERR_X, ERR_Y2, WHITE, mes5);
    printmes(p, ERR_X, ERR_Y3, WHITE, mes6);
    break;
  case GBPAK_ERR_MES_FAIL:
    printmes(p, ERR_X, ERR_Y1, WHITE, mes7);
    printmes(p, ERR_X, ERR_Y2, WHITE, mes8);
    printmes(p, ERR_X, ERR_Y3, WHITE, mes9);
    break;
  case GBPAK_ERR_MES_NOCART:
    printmes(p, ERR_X, ERR_Y1, WHITE, mes10);
    printmes(p, ERR_X, ERR_Y2, WHITE, mes11);
    break;
  case GBPAK_ERR_MES_ANOTHER_GAME:
    printmes(p, ERR_X, ERR_Y1, WHITE, mes12);
    printmes(p, ERR_X, ERR_Y2, WHITE, mes13);
    printmes(p, ERR_X, ERR_Y3, WHITE, mes14);
    break;
  case GBPAK_ERR_MES_NOW_LOADING:
    printmes(p, ERR_X, ERR_Y1, WHITE, mes16);
    printmes(p, ERR_X, ERR_Y2, WHITE, mes17);
    printmes(p, ERR_X, ERR_Y3, WHITE, mes18);
    break;
  case GBPAK_ERR_MES_FAIL_IN_READ_ID:
    printmes(p, ERR_X, ERR_Y1, WHITE, mes20);
    printmes(p, ERR_X, ERR_Y2, WHITE, mes21);
    printmes(p, ERR_X, ERR_Y3, WHITE, mes22);
    printmes(p, ERR_X, ERR_Y4, WHITE, mes23);
    printmes(p, ERR_X, ERR_Y5, WHITE, mes24);
    break;
  default:
    break;
  }
}

/*---------------------------------------------------------------------
                  Main
  ---------------------------------------------------------------------*/
void
mainproc(void) {
  static s32	step = 0;
  u16		*p;
  RetMsg 	*msg = NULL;

  osCreateMesgQueue(&siMsgQ, siMsgBuf, SI_MSG_NUM);
  osCreateThread(&siThread, 8, (void *)siproc, 0,
		 siStack+STACKSIZE/sizeof(u64), 12);
  osStartThread(&siThread);

  while(1){
    siMsg.cmd = SI_CONT_READ;
    osSendMesg(&siMsgQ, (OSMesg *)&siMsg, OS_MESG_NOBLOCK);

    /* Wait for ending message for SI_CONT_READ */
    while(!osRecvMesg(&retMsgQ, NULL, OS_MESG_NOBLOCK));
    
    osWritebackDCache(cfb_16_a, SCREEN_WD*SCREEN_HT*2);
    while(!osRecvMesg(&n_retraceMessageQ, NULL, OS_MESG_NOBLOCK));
    osRecvMesg(&n_retraceMessageQ, NULL, OS_MESG_BLOCK);

    p = (u16 *)osViGetNextFramebuffer();

    switch (step) {
    case 0:			/* Start Gbpak processing */
      siMsg.cmd = SI_GBPAK_READ;
      siMsg.address = 0;	/* Gameboy address */
      siMsg.size = DATA_SIZE;	/* Data size (byte) */
      siMsg.buffer = buffer;	/* RDRAM buffer pointer */
      osSendMesg(&siMsgQ, (OSMesg *)&siMsg, OS_MESG_BLOCK);
      step ++;
      break;
    case 1:			/* Check completion of Gbpak processing */
      if (!MQ_IS_EMPTY(&retMsgQ)){
	gcls(p);
	osRecvMesg(&retMsgQ, (OSMesg *)&msg, OS_MESG_BLOCK);
	if (msg->proc_status == 0) { /* 64GB Pack processing completed */
	  if (msg->ret != 0){
	    if ( (msg->ret == PFS_ERR_NEW_GBCART)
		&& (msg->errmes == NULL)) {
	      step = 0;
	    } else {
	      PrintErrorMessage(msg->errmes);/* Display error message */
#if	1
	      if (msg->errmes==GBPAK_ERR_MES_FAIL_IN_READ_ID) {
		step = 3;
	      } else {
		step = 2;
	      }
#else  /*--- For debugging ---*/
	      step = 2;
#endif
	    }
	  } else {		/* 64GB Pak read/write successful! */
	    printmes(p, ERR_X, ERR_Y1, WHITE, mes19);
	    step = 3;		/* END */
	  }
	} else {		/* 64GB Pak processing not completed */
	  PrintErrorMessage(msg->errmes);
	}
      }
      break;
    case 2:			/* Wait for key input */
      printmes(p, ERR_X, ERR_YY, WHITE, mes15);
      if (conts[CONTNO].nowtrg & A_BUTTON) {
	gcls(p);
	step = 0;
      }
      break;
    }
  }
}
