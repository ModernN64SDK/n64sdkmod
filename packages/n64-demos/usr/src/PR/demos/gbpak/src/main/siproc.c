/*---------------------------------------------------------------------
        Copyright (C) 1998 Nintendo.
        
        File            siproc.c
        Coded    by     Koji Mitsunari. Apr 30, 1998.
        Modified by     Koji Mitsunari. May 18, 1998.
        Comments        SI Device Procedure
   
        $Id: 
   ---------------------------------------------------------------------*/
/**************************************************************************
 *
 *  $Revision: 1.8 $
 *  $Date: 1999/04/16 07:31:26 $
 *  $Source: 
 *
 **************************************************************************/

#include <ultra64.h>
#include "nu64sys.h"
#include "siproc.h"

extern s32	GbpakSramInit(OSPfs *);
extern s32	GbpakSramReadWrite(OSPfs *, u16, u32, u8 *, u32);

RetMsg		retMsg;
static OSMesg   retMsgBuf[RET_MSG_NUM];
OSMesgQueue     retMsgQ;

OSContPad	contdata[MAXCONTROLLERS];
OSContStatus	contstat[MAXCONTROLLERS];
Conts		conts[MAXCONTROLLERS];
OSPfs		pfs;

OSGbpakId	gbid;

/*---------------------------------------------------------------------
                  Initialize Controller
  ---------------------------------------------------------------------*/
static void
ContGetStatus(void)
{
  int	i;

  for (i = 0 ; i < MAXCONTROLLERS ; i ++) {
    conts[i].stat = contstat[i].status;
    if ( contstat[i].errno == 0 &&
	((contstat[i].type & CONT_TYPE_MASK) == CONT_TYPE_NORMAL)) {
      conts[i].flag |= 1;
    } else {
      conts[i].flag = 2;
    }
  }
}
/*---------------------------------------------------------------------
                  Read Controller
  ---------------------------------------------------------------------*/
static void
ReadCont(void)
{
  s32	i;

  osContGetReadData(contdata);
  for(i = 0 ; i < MAXCONTROLLERS ; i ++){
    if (!(contdata[i].errno & CONT_NO_RESPONSE_ERROR)) {
      conts[i].flag |= 1;
      conts[i].oldcon = conts[i].nowcon;
      conts[i].nowcon = (int)contdata[i].button;
      conts[i].nowtrg = conts[i].nowcon & (~conts[i].oldcon);
      conts[i].sx = contdata[i].stick_x;
      conts[i].sy = contdata[i].stick_y;
      
      conts[i].repeat = conts[i].nowcon & conts[i].oldcon;
      if (conts[i].repeat) {
	conts[i].repcnt ++;
	if (conts[i].repcnt < 32) {
	  conts[i].repeat = 0;
	} else if (conts[i].repcnt < 80) {
	  if (conts[i].repcnt & 3) {
	    conts[i].repeat = 0;
	  }
	}
      } else {
	conts[i].repcnt = 0;
      }
      conts[i].repeat |= conts[i].nowtrg;
    } else {
      conts[i].flag = 2;
    }
  }
}

/*---------------------------------------------------------------------
                  Read Data from 64GB-PAK
  ---------------------------------------------------------------------*/
static void
GbpakRamAccess(SiMsg *msg, RetMsg *rmsg, u16 flag)
{
  static s32	step = 0;

  switch(step) {
  case 0:			/*--- Initialize 64GB Pack ---*/
    rmsg->ret = osGbpakInit(&n_siMessageQ, &pfs, CONTNO);
    switch(rmsg->ret) {
    case 0:
      step ++;
      rmsg->errmes = NULL ;
      break;
    case PFS_ERR_NOPACK:
      rmsg->errmes = GBPAK_ERR_MES_NOPACK ;
      break;
    case PFS_ERR_DEVICE:
      rmsg->errmes = GBPAK_ERR_MES_ERR_DEVICE ;
      break;
    case PFS_ERR_CONTRFAIL:
      rmsg->errmes = GBPAK_ERR_MES_FAIL ;
      break;
    }
    if (rmsg->ret != 0) {
      break;
    }
  case 1:			/*--- Read ROM registration area ---*/
    rmsg->ret = osGbpakReadId(&pfs, &gbid, &(rmsg->status));
    if (rmsg->ret == 0) {
      rmsg->ret = osGbpakCheckConnector(&pfs, &(rmsg->status));
    }
    switch(rmsg->ret){
    case 0:		/*--- Registration area read successful ---*/
#if	COMPANY_CODE == 0
      /*--- When game title has not been registered ---*/
      step ++;
      rmsg->errmes = NULL ;
#else
      /*--- Check game title and maker code ---*/
      /*--- Check whether corresponding game is inserted ---*/
      if ( (bcmp(gbid.game_title, (u8 *)GAME_TITLE, 16) == 0 ) &&
	  (gbid.company_code == COMPANY_CODE) ) {
	step ++;
	rmsg->errmes = NULL ;
      } else {
	rmsg->errmes = GBPAK_ERR_MES_ANOTHER_GAME ;
	step = 1;
	rmsg->ret = GBPAK_ERR_ANOTHER_GAME;
      }
#endif
      break;
    case PFS_ERR_NOPACK:	/*--- Nothing is inserted ---*/
      rmsg->errmes = GBPAK_ERR_MES_NOPACK ;
      step = 0;			/* Retry from osGbpakInit() */
      break;
    case PFS_ERR_DEVICE:	/*--- Pak other than 64GB Pak ---*/
      rmsg->errmes = GBPAK_ERR_MES_ERR_DEVICE ;
      step = 0;			/* Retry from osGbpakInit() */
      break;
    case PFS_ERR_CONTRFAIL:	/*--- Read/write failure  ---*/
      rmsg->errmes = GBPAK_ERR_MES_FAIL_IN_READ_ID;
      step = 0;			/* Retry from osGbpakInit() */
      break;
    case PFS_ERR_NEW_GBCART:	/*--- GB cartridge replaced ---*/
      rmsg->errmes = NULL;
      break;
    case PFS_ERR_NO_GBCART:	/*--- GB cartridge not inserted ---*/
      rmsg->errmes = GBPAK_ERR_MES_NOCART ;
      break;
    }
    if (rmsg->ret != 0) {
      break;
    }
  case 2:			/* Read data from RAM */
    /*--- MBC1 SRAM access initialization function ---*/
    /* Set Register 0 (RAM access enable), Register 3 (ROM/RAM change), etc. */
    rmsg->ret = GbpakSramInit(&pfs);
    if (rmsg->ret == 0) {
      /*--- Display message "Accessing" on screen ---*/
      rmsg->errmes = GBPAK_ERR_MES_NOW_LOADING ;
      osSendMesg(&retMsgQ, (OSMesg *)&retMsg, OS_MESG_BLOCK);
      /*--- MBC1 SRAM access function (See gbpaksramreadwrite.c) ---*/
      rmsg->ret = GbpakSramReadWrite(&pfs, flag, msg->address, msg->buffer,
				    msg->size);
      if (rmsg->ret == 0) {
	/*--- Access completed, turn power OFF ---*/
	rmsg->ret = osGbpakPower(&pfs, OS_GBPAK_POWER_OFF);
      }
    }
    switch(rmsg->ret){
    case 0:			/*--- Read/write successful ---*/
      rmsg->errmes = NULL ;
      break;
    case PFS_ERR_NOPACK:	/*--- Nothing is inserted ---*/
      rmsg->errmes = GBPAK_ERR_MES_NOPACK ;
      step = 0;			/* Retry from osGbpakInit() */
      break;
    case PFS_ERR_DEVICE:	/*--- Pak other than 64GB Pak ---*/
      rmsg->errmes = GBPAK_ERR_MES_ERR_DEVICE ;
      step = 0;			/* Retry from osGbpakInit() */
      break;
    case PFS_ERR_CONTRFAIL:	/*--- Read/write failure ---*/
      rmsg->errmes = GBPAK_ERR_MES_FAIL;
      step = 0;			/* Retry from osGbpakInit() */
      break;
    case PFS_ERR_NEW_GBCART:	/*--- GB cartridge replaced ---*/
      rmsg->errmes = NULL;
      step = 1;
      break;
    case PFS_ERR_NO_GBCART:	/*--- GB cartridge not inserted ---*/
      rmsg->errmes = GBPAK_ERR_MES_NOCART ;
      step = 1;
      break;
    }
  }
}

/*---------------------------------------------------------------------
                  Serial Interface Procedure
  ---------------------------------------------------------------------*/
void
siproc(void)
{
  s32		i;
  static int queryflag = 0;
  SiMsg 	*msg = NULL;
  u8		pattern;

  osCreateMesgQueue(&retMsgQ, retMsgBuf, RET_MSG_NUM);

  osContInit(&n_siMessageQ, &pattern, contstat);
  for (i = 0 ; i < MAXCONTROLLERS ; i++ ) {
    conts[i].flag = 0;
    conts[i].nowcon = 0;
  }
  ContGetStatus();

  while (1){
    osRecvMesg(&siMsgQ, (OSMesg *)&msg, OS_MESG_BLOCK);

    switch(msg->cmd) {
    case SI_CONT_READ:		/* Read data from controller */
      osContStartReadData(&n_siMessageQ);
      osRecvMesg(&n_siMessageQ, NULL, OS_MESG_BLOCK);
      ReadCont();

      /*--- Proceed to checking controller status every other time ---*/
      if (queryflag == 0 ) {
	osContStartQuery(&n_siMessageQ);
	osRecvMesg(&n_siMessageQ, NULL, OS_MESG_BLOCK);
	osContGetQuery(contstat);
	ContGetStatus();
      }
      queryflag ^= 1;

      /* Notice for complete reading */
      retMsg.proc_status = 0;
      osSendMesg(&retMsgQ, (OSMesg *)&retMsg, OS_MESG_BLOCK);
      break;

    case SI_GBPAK_READ:		/* When reading from RAM */
      retMsg.proc_status = 1;
      GbpakRamAccess(msg, &retMsg, OS_READ);
      retMsg.proc_status = 0;
      osSendMesg(&retMsgQ, (OSMesg *)&retMsg, OS_MESG_BLOCK);
      break;

    case SI_GBPAK_WRITE:	/* When writing to RAM (not used) */
      retMsg.proc_status = 1;
      GbpakRamAccess(msg, &retMsg, OS_WRITE);
      retMsg.proc_status = 0;
      osSendMesg(&retMsgQ, (OSMesg *)&retMsg, OS_MESG_BLOCK);
      break;
    }
  }
}
