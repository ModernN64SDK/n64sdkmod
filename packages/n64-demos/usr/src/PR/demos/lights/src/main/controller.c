
/*---------------------------------------------------------------------*
        Copyright (C) 1997 Nintendo. (Originated by SGI)
        
        $RCSfile: controller.c,v $
        $Revision: 1.7 $
        $Date: 1998/12/24 09:03:00 $
 *---------------------------------------------------------------------*/
#include <ultra64.h>

/* #include <rom.h> */
#include <ramrom.h>

#include "controller.h"

OSMesgQueue	controllerMsgQ;
OSMesg		controllerMsgBuf;

int initControllers(void);
void readController(int);
OSContStatus     statusdata[MAXCONTROLLERS];
OSContPad        controllerdata[MAXCONTROLLERS];


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

void readController(int controllerSlot)
{
  static u16             button, lastbutton;
    
  if (controllerSlot < 0) {
#ifdef DEBUG
    osSyncPrintf("no controller connection\n");
#endif
    return;
  }

  osContStartReadData(&controllerMsgQ);
  (void)osRecvMesg(&controllerMsgQ, NULL, OS_MESG_BLOCK);
  osContGetReadData(controllerdata);
  button = controllerdata[controllerSlot].button;
    
  if ((button & CONT_LEFT) && !(lastbutton & CONT_LEFT)) {
    shadowScaleX -= 0.1;
#ifdef DEBUG
    osSyncPrintf("shadowScaleX = %f\n",shadowScaleX);
#endif
  }
	    
  if ((button & CONT_RIGHT) && !(lastbutton & CONT_RIGHT)) {
    shadowScaleX += 0.1;
#ifdef DEBUG
    osSyncPrintf("shadowScaleX = %f\n",shadowScaleX);
#endif
  }
	    
  if ((button & CONT_UP) && !(lastbutton & CONT_UP)) {
    shadowScaleZ += 0.1;
#ifdef DEBUG
    osSyncPrintf("shadowScaleZ = %f\n",shadowScaleZ);
#endif
  }
	    
  if ((button & CONT_DOWN) && !(lastbutton & CONT_DOWN)) {
    shadowScaleZ -= 0.1;
#ifdef DEBUG
    osSyncPrintf("shadowScaleZ = %f\n",shadowScaleZ);
#endif
  }
	    
  if (button & CONT_F) {
    shadowTranslateX += 1.0;
#ifdef DEBUG
    osSyncPrintf("shadowTranslateX = %f\n",shadowTranslateX);
#endif
  }
	    
  if (button & CONT_C) {
    shadowTranslateX -= 1.0;
#ifdef DEBUG
    osSyncPrintf("shadowTranslateX = %f\n",shadowTranslateX);
#endif
  }
	    
  if (button & CONT_E) {
    shadowTranslateZ -= 1.0;
#ifdef DEBUG
    osSyncPrintf("shadowTranslateZ = %f\n",shadowTranslateZ);
#endif
  }
	    
  if (button & CONT_D) {
    shadowTranslateZ += 1.0;
#ifdef DEBUG
    osSyncPrintf("shadowTranslateZ = %f\n",shadowTranslateZ);
#endif
  }
	    
  lastbutton = button;
}
