
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

/*---------------------------------------------------------------------*
        Copyright (C) 1997 Nintendo. (Originated by SGI)
        
        $RCSfile: controller.c,v $
        $Revision: 1.4 $
        $Date: 1998/12/24 08:33:15 $
 *---------------------------------------------------------------------*/

#include <ultra64.h>

#include <PR/ramrom.h>

/* app specific includes */
#include "fill.h"
#include "timer.h"

OSMesgQueue	controllerMsgQ;
OSMesg		controllerMsgBuf;

int initControllers(void);
OSContStatus     statusdata[MAXCONTROLLERS];
OSContPad        controllerdata[MAXCONTROLLERS];

static u16 		button;
static u16      lastbutton;
static s16 		stickX, stickY;      
static u16		press;
static u16		press_up, press_down;
int 			ControllerMode = 0;

/*
 *
 * Return the lowest number controller connected to system
 */
int initControllers()
{
    OSMesgQueue     serialMsgQ;
    OSMesg          serialMsg;
    int             i;
    u8              pattern;

    osCreateMesgQueue(&serialMsgQ, &serialMsg, 1);
    osSetEventMesg(OS_EVENT_SI, &serialMsgQ, (OSMesg)1);

    osContInit(&serialMsgQ, &pattern, &statusdata[0]);

    osCreateMesgQueue(&controllerMsgQ, &controllerMsgBuf, 1);
    osSetEventMesg(OS_EVENT_SI, &controllerMsgQ, (OSMesg)0);

    for (i = 0; i < MAXCONTROLLERS; i++) {
        if ((pattern & (1<<i)) &&
                !(statusdata[i].errno & CONT_NO_RESPONSE_ERROR))
            return i;
    }
    return -1;
}

void ResetParameters(void)
{
  	UseZMode            =         0;
  	UseAAMode           =         0;
  	TimerTicks          =         7;
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

  if (press_down & CONT_A)
    {
      UseAAMode = 1 - UseAAMode;
      ControllerInput = 1;
    }

  if (press_down & CONT_B)
    {
      UseZMode = 1 - UseZMode;
      ControllerInput = 1;
    }

  if (press_down & CONT_F)
    {      
      DoClrClr = 1 - DoClrClr;
      ControllerInput = 1; 
    }

  if (press_down & CONT_START)
    {
      ResetParameters();
      ControllerInput = 1;
    }
}


void ReadController(void)
{
    ReadController0();
}
