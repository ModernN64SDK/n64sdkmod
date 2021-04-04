/*====================================================================
  File   : motortest.c

  Created   by Koji Mitsunari.  Mar,20 1997.
  Copyright by Nintendo, Co., Ltd.     1997.
  ====================================================================*/
#include <ultra64.h>
#include "nu64sys.h"
#include "motortest.h"

extern  u8      pat[][CHAR_HT];
extern	u32	cfb_16_a[];

static OSMesgQueue	motorMsgQ;
static OSMesg		motorMsgBuf[MOTOR_MSG_NUM];
static OSThread		motorThread;
static u64      	motorStack[STACKSIZEBYTES/sizeof(u64)];

static OSContPad	contdata[MAXCONTROLLERS];
static OSContStatus	contstat[MAXCONTROLLERS];
static OSPfs		pfs[MAXCONTROLLERS];
static int		joyport[MAXCONTROLLERS];
  
static int		motorflag[MAXCONTROLLERS];
static int		motor_stop_fr[MAXCONTROLLERS];

static MotorMsg		motormsg = {MOTOR_DUMMY,0};


/*---------------------------------------------------------------------
                  Display character on the screen
  ---------------------------------------------------------------------*/
void
putchar(u16 color, int curs_x, int curs_y, char c, int flag)
{
  int	i, j;
  int	x = curs_x*CHAR_WD;
  int	y = curs_y*CHAR_HT;
  u16	fgcolor, bgcolor;
  u8	*pc = pat[c-0x20];
  u16	*p = (u16 *)osViGetNextFramebuffer() + x + SCREEN_WD*(y-8);

  if (flag == REVERSE) {
    fgcolor = BGCOLOR;
    bgcolor = color;
  } else {
    fgcolor = color;
    bgcolor = BGCOLOR;
  }

  for (j = 0; j < CHAR_HT; j ++, pc++) {
    for (i = CHAR_WD - 1; i >= 0; i --) {
      if (*pc & (1 << i)) {
	*p = fgcolor;
      } else {
	*p = bgcolor;
      }
      p ++;
    }
    p += SCREEN_WD - CHAR_WD;
  }
}

/*---------------------------------------------------------------------
                  Display dec-data on the screen
  ---------------------------------------------------------------------*/
void
putint_d(u16 color, int curs_x, int curs_y, int i)
{
  char	c;
  int	j;
    
  if (i >= 0) {
    c = ' ';
    j = i;
  } else {
    c = '-';
    j = -i;
  }
  
  do {
    putchar(color, curs_x--, curs_y, (j % 10) + '0', NORMAL);
    j /= 10;
  } while (j > 0);

  putchar(color, curs_x-1, curs_y, ' ', NORMAL);
  putchar(color, curs_x, curs_y, c, NORMAL); 

  if (i > -10 && i < 10){
    putchar(color, curs_x-2, curs_y, ' ', NORMAL); 
  }
}

/*---------------------------------------------------------------------
                  Display string on the screen
  ---------------------------------------------------------------------*/
void
printstr(u16 color, int curs_x, int curs_y, char *s)
{
    int i;

    for (i = 0 ; *s != NULL; i ++) {
      putchar(color, curs_x + i, curs_y, *s++, NORMAL);
    }
}

/*---------------------------------------------------------------------
                  Clear the screen
  ---------------------------------------------------------------------*/
void
gcls(void)
{
  int 	i;
  u16	*p;
  
  p = (u16 *)osViGetNextFramebuffer();
  for (i = 0; i < SCREEN_WD*SCREEN_HT; i ++) {
    *p++ = 0;
  }
}

/*---------------------------------------------------------------------
                  Motor Thread
  ---------------------------------------------------------------------*/
static void
motorMain(void)
{
  s32		ret;
  MotorMsg	*msg=0;
  int		contno;
  s16		cmd;
  
  while (1){
    osRecvMesg(&motorMsgQ, (OSMesg *)&msg, OS_MESG_BLOCK);
    
    contno = msg->contno;
    cmd = msg->cmd;
    switch(cmd) {
    case MOTOR_ON:
      ret = osMotorStart(&pfs[contno]);
      if (!ret) {
	motorflag[contno] = 1;
      }
      break;
    case MOTOR_OFF:
      ret = osMotorStop(&pfs[contno]);
      if (!ret) {
	motorflag[contno] = 0;
      }
      break;
    }
  }
}

#if 0				/* These functions are not used */
/*---------------------------------------------------------------------
                  Start Motor 
  ---------------------------------------------------------------------*/
void
MotorON(int contno) {
  motormsg.cmd = MOTOR_ON;
  motormsg.contno = contno;
  osSendMesg(&motorMsgQ, (OSMesg *)&motormsg, OS_MESG_BLOCK);
}

/*---------------------------------------------------------------------
                  Stop Motor
  ---------------------------------------------------------------------*/
void
MotorOFF(int contno) {
  motormsg.cmd = MOTOR_OFF;
  motormsg.contno = contno;
  osSendMesg(&motorMsgQ, (OSMesg *)&motormsg, OS_MESG_BLOCK);
}
#endif

/*---------------------------------------------------------------------
                  Initialize Motor Thread
  ---------------------------------------------------------------------*/
void
MotorThreadInit(void)
{
  osCreateMesgQueue(&motorMsgQ, motorMsgBuf, MOTOR_MSG_NUM);
  osCreateThread(&motorThread, 8, (void *)motorMain, 0,
		 (void *)(motorStack+STACKSIZEBYTES/sizeof(u64)),
		 MOTOR_PRIORITY);
  osStartThread(&motorThread);
}

/*---------------------------------------------------------------------
                  Initialize Motor
  ---------------------------------------------------------------------*/
void
MotorInit(void)
{
  u8	pattern;
  int	channel;
  int	ret;

  osContInit(&n_siMessageQ, &pattern, contstat);
  for (channel = 0  ; channel < MAXCONTROLLERS ; channel ++) {
    joyport[channel] = 0;
    if ((pattern >> channel) & 1) {
      if ((contstat[channel].type & CONT_JOYPORT) != 0 &&
	  (contstat[channel].status & CONT_CARD_ON) != 0) {
	ret = osPfsInitPak(&n_siMessageQ, &pfs[channel], channel);

	/* In case of game application, if PFS_ERR_ID_FATAL occurs */
	/* here, ID needs to be restored by player's judgement.   */
	/*                                                        */
	/*                                                        */
	/* if (ret == PFS_ERR_ID_FATAL)                           */
	/*     if (AskPlayer())                                   */
	/*         ret = osPfsRepairId(&pfs[channel]);            */
	/*                                                        */
	/* If only the Rumble Pak is used, Controller Pak does    */
	/* not need to be initialized.                            */

	if (ret == PFS_ERR_DEVICE) {
	  ret = osMotorInit(&n_siMessageQ, &pfs[channel], channel);
	  if (!ret) {		/* Motor is connected */
	    printstr(WHITE, JOY_X-1+channel*DIS_X, JOY_Y + 3, "OK");
	    joyport[channel] = 1;
	  } else {		/* Device except Motor is connected */
	    printstr(WHITE, JOY_X-1+channel*DIS_X, JOY_Y + 3, "--");
	  }
	} else if (ret == 0){	/* CPAK is connected */
	  printstr(WHITE, JOY_X-1+channel*DIS_X, JOY_Y + 3, "CP");
	}
      }
    }
  }
}

/*---------------------------------------------------------------------
                  Main Thread
  ---------------------------------------------------------------------*/
void
mainproc(void) {
  int	i, j;
  int	mode = InitializeMode;
  s16	cmd;

  int	motormode[MAXCONTROLLERS];
  int	motorcnt[MAXCONTROLLERS];
  int	controller[MAXCONTROLLERS];
  int  	newbutton[MAXCONTROLLERS];
  int  	oldbutton[MAXCONTROLLERS];

  motormsg.cmd = MOTOR_DUMMY;

  gcls();
  MotorThreadInit();
  MotorInit();
  
  for (i = 0 ; i < MAXCONTROLLERS ; i ++){
    controller[i] = 1;
    newbutton[i] = 0;
    oldbutton[i] = 0;
    motorflag[i] = 0;
    motormode[i] = 0;
    motorcnt[i] = 0;
  }

  while(1){
    osContStartReadData(&n_siMessageQ);
/*    osWritebackDCacheAll(); */
    osWritebackDCache(cfb_16_a, SCREEN_HT*SCREEN_WD*2);
    osRecvMesg(&n_siMessageQ, NULL, OS_MESG_BLOCK);
    osRecvMesg(&n_retraceMessageQ, NULL, OS_MESG_BLOCK);
    osContGetReadData(contdata);
    
    for (i = 0 ; i < MAXCONTROLLERS ; i ++ ) {
      if (!(contdata[i].errno & CONT_NO_RESPONSE_ERROR)) {
	if (controller[i]==0) {
	  putint_d(WHITE, JOY_X-2 + i*DIS_X, MES_Y-1, i+1);
	  putchar(WHITE,  MES_X+1 + i*DIS_X, MES_Y-1, 'P', NORMAL);
	  printstr(WHITE, MES_X + i*DIS_X, MES_Y+1, MES_OFF);
	  motormode[i] = 0;
	  motorcnt[i] = 0;
	}
	controller[i] = 1;
	oldbutton[i] = newbutton[i];
	newbutton[i] = contdata[i].button;
	putint_d(WHITE, JOY_X + i*DIS_X, JOY_Y,   contdata[i].stick_x);
	putint_d(WHITE, JOY_X + i*DIS_X, JOY_Y+1, contdata[i].stick_y);
      } else {
	if (controller[i]) {
	  putint_d(GRAY, JOY_X-2 + i*DIS_X, MES_Y-1, i+1);
	  putchar(GRAY,  MES_X+1 + i*DIS_X, MES_Y-1, 'P', NORMAL);
	  printstr(GRAY, MES_X + i*DIS_X, MES_Y, MES_ON);
	  printstr(GRAY, MES_X + i*DIS_X, MES_Y+1, MES_OFF);
	  printstr(GRAY, MES_X + i*DIS_X, MES_Y+2, MES_ON_OFF);
	}
	controller[i] = 0;
      }
    }
    switch (mode) {
    case InitializeMode:
      printstr(WHITE, TITLE_MES_X, TITLE_MES_Y, TITLE_MES);
      printstr(WHITE, MES_X, MES_Y - 1, "1P    2P    3P    4P");
      putchar(BLUE2, MESB_X, MES_Y,   'A', REVERSE);
      putchar(GREEN, MESB_X, MES_Y+1, 'B', REVERSE);
      putchar(WHITE, MESB_X, MES_Y+2, 'Z', REVERSE);
      printstr(WHITE, MESD_X, MES_Y,   "...");
      printstr(WHITE, MESD_X, MES_Y+1, "...");
      printstr(WHITE, MESD_X, MES_Y+2, "...");
      putchar(WHITE, MESB_X, JOY_Y,   'X', NORMAL);
      putchar(WHITE, MESB_X, JOY_Y+1, 'Y', NORMAL);

      for (i = 0 ; i < MAXCONTROLLERS ; i ++) {
	printstr(GRAY,  MES_X + i*DIS_X, MES_Y, MES_ON);
	printstr(WHITE, MES_X + i*DIS_X, MES_Y+1, MES_OFF);
	printstr(GRAY,  MES_X + i*DIS_X, MES_Y+2, MES_ON_OFF);
      }
      mode = MainMode;
      break;
    case MainMode:
      for (i = 0 ; i < MAXCONTROLLERS ; i ++ ) {
	if (controller[i]==0 || joyport[i]==0){
	  continue;
	}
	cmd = MOTOR_DUMMY;
	if (motor_stop_fr[i] > 0) {
	  motor_stop_fr[i] --;
	  cmd = MOTOR_OFF;
	}

	j = newbutton[i] & ~oldbutton[i];
	if (j & A_BUTTON)	{
	  motormode[i] = 1;	/* Start Motor */
	  motorcnt[i] = 0;
	  printstr(WHITE, MES_X + i*DIS_X, MES_Y, MES_ON);
	  printstr(GRAY,  MES_X + i*DIS_X, MES_Y+1, MES_OFF);
	  printstr(GRAY,  MES_X + i*DIS_X, MES_Y+2, MES_ON_OFF);
	} else if (j & B_BUTTON) {
	  motormode[i] = 0;	/* Stop Motor */
	  motorcnt[i] = 0;
	  motor_stop_fr[i] = MOTOR_STOP_FR;
	  printstr(GRAY, MES_X + i*DIS_X, MES_Y, MES_ON);
	  printstr(WHITE, MES_X + i*DIS_X, MES_Y+1, MES_OFF);
	  printstr(GRAY, MES_X + i*DIS_X, MES_Y+2, MES_ON_OFF);
	} else if (j & Z_TRIG) {
	  motormode[i] = 2;
	  motorcnt[i] = 0;
	  printstr(GRAY, MES_X + i*DIS_X, MES_Y, MES_ON);
	  printstr(GRAY, MES_X + i*DIS_X, MES_Y+1, MES_OFF);
	  printstr(WHITE, MES_X + i*DIS_X, MES_Y+2, MES_ON_OFF);
	}

	switch(motormode[i]) {
	case 0:
	  if (motorflag[i] != 0) {
	    cmd = MOTOR_OFF;
	  }
	  break;
	case 1:
	  if (motorflag[i] == 0) {
	    cmd = MOTOR_ON;
	  }
	  break;
	case 2:
	  j = max(abs(contdata[i].stick_x), abs(contdata[i].stick_y));
	  if (i > 70 && motorflag[i]==0) {
	    cmd = MOTOR_ON;
	  } else if (i < 6 && motorflag[i]!=0) {
	    motor_stop_fr[i] = MOTOR_STOP_FR;
	    cmd = MOTOR_OFF;
	  } else {
	    if (motorcnt[i] >= 256) {
	      motorcnt[i] -= 256;
	      if (!motorflag[i]) {
		cmd = MOTOR_ON;
	      }
	    } else {
	      motorcnt[i] += ((int)(j * j * j) >> 8);
	      if (motorflag[i]) {
		motor_stop_fr[i] = MOTOR_STOP_FR;
		cmd = MOTOR_OFF;
	      }
	    }
	  }
	  break;
	}
	if (cmd != MOTOR_DUMMY) {
	  motormsg.cmd = cmd;
	  motormsg.contno = i;
	  osSendMesg(&motorMsgQ, (OSMesg *)&motormsg, OS_MESG_BLOCK);
	}
      }
      break;
    }
  }
}
