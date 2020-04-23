
/**************************************************************************
 *                                                                        *
 *               Copyright (C) 1995, Silicon Graphics, Inc.               *
 *                                                                        *
 *  These coded instructions, statements, and computer programs  contain  *
 *  unpublished  proprietary  information of Silicon Graphics, Inc., and  *
 *  are protected by Federal copyright  law.  They  may not be disclosed  *
 *  to  third  parties  or copied or duplicated in any form, in whole or  *
 *  in part, without the prior written consent of Silicon Graphics, Inc.  *
 *                                                                        *
 *************************************************************************/

#include <ultra64.h>

/* app specific includes */
#include "tile_rect.h"


#define CURSOR_SPEED	0.10
#define CURSOR_ACCEL	0.01
#define CURSOR_THRESH	30

OSMesgQueue	controllerMsgQ;
OSMesg		controllerMsgBuf;

OSContStatus     statusdata[MAXCONTROLLERS];
OSContPad        controllerdata[MAXCONTROLLERS];

static u16 		ActiveController;
static u16 		button;
static u16      lastbutton;
static s16 		stickX, stickY;      
static s16 		deltaX, deltaY;      
static u16		press, press_up, press_down;

/*
 *  Extern'd data
 */
float	CursorX, CursorY;
int		PrintEn;
int		FilterEn;

/*
 *
 * Return the lowest number controller connected to system
 */
int initControllers( void )
{
    int             i;
    u8              pattern;
	OSMesgQueue     serialMsgQ;
	OSMesg          serialMsg;

	CursorX = SCREEN_WD / 2;
	CursorY = SCREEN_HT / 2;
	deltaX = deltaY = 0;

    osCreateMesgQueue(&serialMsgQ, &serialMsg, 1);
    osSetEventMesg(OS_EVENT_SI, &serialMsgQ, (OSMesg)1);

    osContInit(&serialMsgQ, &pattern, &statusdata[0]);

    osCreateMesgQueue(&controllerMsgQ, &controllerMsgBuf, 1);
    osSetEventMesg(OS_EVENT_SI, &controllerMsgQ, (OSMesg)0);

    for (i = 0; i < MAXCONTROLLERS; i++) {
        if ((pattern & (1<<i)) &&
                !(statusdata[i].errno & CONT_NO_RESPONSE_ERROR)) {
			ActiveController = i;
            return i;
		}
    }
    return -1;
}


void ReadController0(void)
{

  (void)osRecvMesg(&controllerMsgQ, NULL, OS_MESG_BLOCK);
  osContGetReadData(controllerdata);

  button = controllerdata[ActiveController].button;
  stickX = controllerdata[ActiveController].stick_x;
  stickY = controllerdata[ActiveController].stick_y;

  press = lastbutton ^ button;
  press_up = lastbutton & press;
  press_down = button & press;
  lastbutton = button;


  	if (stickX > 15)
    {
      CursorX += (stickX - 15) * CURSOR_SPEED + deltaX * CURSOR_ACCEL;      
	  if((stickX - 15) > CURSOR_THRESH)
	  	deltaX += (stickX - 15);
    }
  	else if (stickX < -15)
    {
      CursorX += (stickX + 15) * CURSOR_SPEED + deltaX * CURSOR_ACCEL;
	  if((stickX + 15) < -CURSOR_THRESH)
	  	deltaX += (stickX + 15);
    }
  	else 
	{
		deltaX = 0;
	}

  	if (stickY > 15)
    {
      CursorY -= (stickY - 15) * CURSOR_SPEED + deltaY * CURSOR_ACCEL;      
	  if ((stickY - 15) > CURSOR_THRESH)
		deltaY += stickY - 15;
    }
  	else if (stickY < -15)
    {
      CursorY -= (stickY + 15) * CURSOR_SPEED + deltaY * CURSOR_ACCEL;
	  if ((stickY + 15) < -CURSOR_THRESH)
		deltaY += stickY + 15;
    }
  	else
	{
	  deltaY = 0;
	}

  if (press_down & CONT_A)
  {
	PrintEn = 1 - PrintEn;
  }

  if (press_down & CONT_B)
  {
	FilterEn = 1 - FilterEn;
  }

  if (press_down & CONT_C)
  {
		CursorX = SCREEN_WD / 2;
		CursorY = SCREEN_HT / 2;
  }

  if (press_down & CONT_F)
  {      
		CursorX = SCREEN_WD / 2;
		CursorY = SCREEN_HT / 2;
  }

  if (press_down & CONT_D)
  {
		CursorX = SCREEN_WD / 2;
		CursorY = SCREEN_HT / 2;
  }

  if (press_down & CONT_E)
  {      
		CursorX = SCREEN_WD / 2;
		CursorY = SCREEN_HT / 2;
  }


  if (press_down & CONT_G)
  {
		CursorX = SCREEN_WD / 2;
		CursorY = SCREEN_HT / 2;
  }

  if (press_down & CONT_START)
  {
		CursorX = SCREEN_WD / 2;
		CursorY = SCREEN_HT / 2;
  }
}


void ReadController(void)
{
    ReadController0();
}
