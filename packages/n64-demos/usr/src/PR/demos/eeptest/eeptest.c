
/*---------------------------------------------------------------------*
        Copyright (C) 1998 Nintendo.
        
        $RCSfile: eeptest.c,v $
        $Revision: 1.5 $
        $Date: 1999/01/12 02:06:25 $
 *---------------------------------------------------------------------*/
#include <ultra64.h>
#include "nu64sys.h"
#include "graph.h"
#include "eeptest.h"

static	OSContStatus	contstat[MAXCONTROLLERS];
static	OSContPad	pad[MAXCONTROLLERS];
static	u8		controller[MAXCONTROLLERS];

static	int		lastx[MAXCONTROLLERS];
static	int      	lasty[MAXCONTROLLERS];
static	int      	lastbutton[MAXCONTROLLERS];
static	int		oldbutton[MAXCONTROLLERS];

/*---------------------------------------------------------------------
                  main
  ---------------------------------------------------------------------*/
void
mainproc(void)
{
  int   i, ret, error;
  int	mode = InitializeMode;
  int	repeat_trig = 0, repeat_cnt = 0;
  u8	pattern;
  u8	long_buffer[EEPROM_BLOCK_SIZE * EEP16K_MAXBLOCKS];

  gcls();
  
  osContInit(&n_siMessageQ, &pattern, contstat);
  for (i = 0  ; i < MAXCONTROLLERS ; i ++) {
    lastbutton[i] = 0;
    lastx[i] = 0;
    lasty[i] = 0;
    if ((pattern >> i) & 1) {
      if (((contstat[i].type & CONT_TYPE_MASK) == CONT_TYPE_NORMAL)) {
	controller[i] = 1;
      } else {
	controller[i] = 0;	/* Not a normal controller */
      }
    } else {
      controller[i] = 0;	/* Controller is not inserted */
    }
  }

  while(1) {
    osContStartReadData(&n_siMessageQ);
    osWritebackDCacheAll();
    n_WaitMesg(si);
    n_WaitMesg(retrace);
    osContGetReadData(pad);

    for (i = 0; i < MAXCONTROLLERS; i ++) {
      if (!(pad[i].errno & 
	    (CONT_NO_RESPONSE_ERROR | CONT_OVERRUN_ERROR))) {
	controller[i] = 1;
	oldbutton[i] = lastbutton[i];
	lastbutton[i] = pad[i].button;
	lastx[i] = pad[i].stick_x;
	lasty[i] = pad[i].stick_y;
      } else {
	controller[i] = 0;
      }
    }
    repeat_trig = lastbutton[ContNo] & oldbutton[ContNo];
    if (repeat_trig) {
      repeat_cnt ++;
      if (repeat_cnt < 32 ) {
	repeat_trig = 0;
      } else if (repeat_cnt < 100) {
	if ( (repeat_cnt & 7) != 0 ) {
	  repeat_trig = 0;
	}
      }
    } else {
      repeat_cnt = 0;
    }

    switch(mode) {
    case InitializeMode:
      gcls();
      printstr(WHITE, TITLE_X, TITLE_Y, TITLE_MES);
      ret = osEepromProbe(&n_siMessageQ);
      switch ( ret ) {
      case EEPROM_TYPE_4K:
	printstr(WHITE, MES_X, MES_Y, "EEPROM TYPE 4K");
	break;
      case EEPROM_TYPE_16K:
	printstr(WHITE, MES_X, MES_Y, "EEPROM TYPE 16K");
	break;
      case 0:
	printstr(WHITE, MES_X, MES_Y, "No EEPROM!");
	break;
      default :
	printstr(RED, MES_X, MES_Y, "osEepromProbe Error!");
      }
#ifdef	DEBUG_EEPROM
      mode = TestLong4KMode;
#else
      if ( ret == EEPROM_TYPE_4K ) {
	mode = TestLong4KMode;
      } else if ( ret == EEPROM_TYPE_16K ) {
	mode = TestLong16KMode;
      } else {
	mode = MainMode;
      }
#endif
      break;
    case TestLong4KMode:
      for (i  = 0; i < EEPROM_BLOCK_SIZE * EEPROM_MAXBLOCKS; i++) {
	long_buffer[i] = i & 0xff;
      }
      ret = osEepromLongWrite(&n_siMessageQ, 0, long_buffer,
			      EEPROM_BLOCK_SIZE * EEPROM_MAXBLOCKS);
      if (ret == 0) {
	printstr(WHITE, MES_X, MES_Y+1, "LONG  4K WRITE OK!");
      } else {
	printstr(RED,   MES_X, MES_Y+1, "LONG  4K WRITE NG!");
      }
      ret = osEepromLongRead(&n_siMessageQ, 0, long_buffer,
			       EEPROM_BLOCK_SIZE * EEPROM_MAXBLOCKS);
      if (ret == 0) {
	error = 0;
	for (i = 0; i < EEPROM_BLOCK_SIZE * EEPROM_MAXBLOCKS; i++) {
	  if (long_buffer[i] != (i & 0xff)) {
	    error++;
	  }
	}
	if (error == 0) {
	  printstr(WHITE, MES_X, MES_Y+2, "LONG  4K READ  OK!");
	} else {
	  printstr(RED, MES_X, MES_Y+2, "LONG  4K READ  NG...DATA BAD!");
	}
      } else {
	printstr(RED, MES_X, MES_Y+2, "LONG  4K READ  NG!");
      }
#ifdef	DEBUG_EEPROM
      mode = TestLong16KMode;
#else
      mode = MainMode;
#endif
      break;
    case TestLong16KMode:
      for (i  = 0; i < EEPROM_BLOCK_SIZE * EEP16K_MAXBLOCKS; i++) {
	long_buffer[i] = i & 0xff;
      }
      ret = osEepromLongWrite(&n_siMessageQ, 0, long_buffer,
			      EEPROM_BLOCK_SIZE * EEP16K_MAXBLOCKS);
      if (ret == 0) {
	printstr(WHITE, MES_X, MES_Y+3, "LONG 16K WRITE OK!");
      } else {
	printstr(RED,   MES_X, MES_Y+3, "LONG 16K WRITE NG!");
      }
      ret = osEepromLongRead(&n_siMessageQ, 0, long_buffer,
			       EEPROM_BLOCK_SIZE * EEP16K_MAXBLOCKS);
      if (ret == 0) {
	error = 0;
	for (i = 0; i < EEPROM_BLOCK_SIZE * EEP16K_MAXBLOCKS; i++) {
	  if (long_buffer[i] != (i & 0xff)) {
	    error++;
	  }
	}
	if (error == 0) {
	  printstr(WHITE, MES_X, MES_Y+4, "LONG 16K READ  OK!");
	} else {
	  printstr(RED, MES_X, MES_Y+4, "LONG 16K READ  NG...DATA BAD!");
	}
      } else {
	printstr(RED, MES_X, MES_Y+4, "LONG 16K READ  NG!");
      }
      mode = MainMode;
      break;
    case MainMode:
      printstr(WHITE, MES_X, MES_Y+6, "EEPROM Check END!");
      break;
    }
  }
}  
