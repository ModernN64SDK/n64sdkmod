/*---------------------------------------------------------------------*
        Copyright (C) 1998 Nintendo.
        
        $RCSfile: cont.c,v $
        $Revision: 1.3 $
        $Date: 1999/01/12 02:24:36 $
 *---------------------------------------------------------------------*/
/*====================================================================
  File   : cont.c     Ver 1.0

  Created   by Koji Mitsunari.  Jun,19 1997.
  Copyright by Nintendo, Co., Ltd.     1997.
  ====================================================================*/
#include <ultra64.h>
#include "cont.h"

OSContPad	contdata[MAXCONTROLLERS];
OSContStatus	contstat[MAXCONTROLLERS];
Conts		conts[MAXCONTROLLERS];

/*---------------------------------------------------------------------
                  Initialize Controller
  ---------------------------------------------------------------------*/
void	initcont(OSMesgQueue *siMesgQ)
{
  int	i;
  u8	pattern;
  osContInit(siMesgQ, &pattern, contstat);
  for (i = 0 ; i < MAXCONTROLLERS ; i ++) {
    conts[i].stat = contstat[i].status;
    conts[i].nowcon = 0;
    if ( ((pattern >> i) & 1) != 0 &&
	((contstat[i].type & CONT_TYPE_MASK) == CONT_TYPE_NORMAL)) {
      conts[i].flag = 1;
    } else {
      conts[i].flag = 0;
    }
  }
}
/*---------------------------------------------------------------------
                  Read Controller
  ---------------------------------------------------------------------*/
void	readcont(void)
{
  int	i;
  osContGetReadData(contdata);
  for(i = 0 ; i < MAXCONTROLLERS ; i ++){
    if (!(contdata[i].errno & CONT_NO_RESPONSE_ERROR)) {
      conts[i].flag = 1;
      conts[i].oldcon = conts[i].nowcon;
      conts[i].nowcon = (int)contdata[i].button;
      conts[i].nowtrg = conts[i].nowcon & ~conts[i].oldcon;
      conts[i].sx = contdata[i].stick_x;
      conts[i].sy = contdata[i].stick_y;
      
      conts[i].repeat = conts[i].nowcon & conts[i].oldcon;
      if (conts[i].repeat) {
	conts[i].repcnt ++;
	if (conts[i].repcnt < 12) {
	  conts[i].repeat = 0;
	} else if (conts[i].repcnt < 32) {
	  if (conts[i].repcnt & 3) {
	    conts[i].repeat = 0;
	  }
	}
      } else {
	conts[i].repcnt = 0;
      }
    } else {
      conts[i].flag = 0;
    }
  }
}
