/*============================================================================

		NINTENDO64 TECHNICAL SUPPORT CENTER 
		
		    NINTENDO64 SAMPLE PROGRAM 2

		Copyright (C) 1997, NINTENDO Co,Ltd.

============================================================================*/

#include <ultra64.h>
#include <PR/ramrom.h>
#include <assert.h>
#include "nnsched.h"
#include "def.h"
#include "controller.h"
#include "message.h"

OSMesgQueue controllerMsgQ;
OSMesg      controllerMsgBuf;

OSContStatus statusdata[MAXCONTROLLERS];
OSContPad   controllerdata[MAXCONTROLLERS];
OSContPad   *controllerdata_ptr[MAXCONTROLLERS];
OSContPad   controllerdataTriger[MAXCONTROLLERS];
int         numControllers = 0;

static OSThread controllerThread;
static u64 controllerThreadStack[STACKSIZE/sizeof(u64)];

/*
 * initilize controller
 *
 * maxcontrollers: want controller max num
 * return        : useable controller num
 */
int controllerInit(int maxcontrollers)
{
  int         i;
  u8          pattern;
  OSMesgQueue serialMsgQ;
  OSMesg      serialMsg;

  osCreateMesgQueue(&serialMsgQ, &serialMsg, 1);
  osSetEventMesg(OS_EVENT_SI, &serialMsgQ, (OSMesg)0);

  osContInit(&serialMsgQ, &pattern, &statusdata[0]);

  osCreateMesgQueue(&controllerMsgQ, &controllerMsgBuf, 1);
  osSetEventMesg(OS_EVENT_SI, &controllerMsgQ, (OSMesg)0);

 
  for(i = 0; i < MAXCONTROLLERS; i++){
    if((pattern & (1<<i)) &&
       !(statusdata[i].errno & CONT_NO_RESPONSE_ERROR)) {
      controllerdata_ptr[numControllers] = & controllerdata[i];
      numControllers++;
      if(numControllers == maxcontrollers){
	return numControllers;
      }
    }
  }
/*  osContStartReadData(&controllerMsgQ); */
  return numControllers;
}

/*
 * Start read controller data
 */
void controllerReadStart(void)
{
  osContStartReadData(&controllerMsgQ);
}

/*
 * Conroller read thread
 */
void controllerProc(void *arg)
{
  int i;
  u16 button;
  static u16 lastbutton[MAXCONTROLLERS];
  static int cnt;
  OSMesgQueue msgQ;
  OSMesg    msgbuf[MAX_MESGS];
  NNScClient client;

  /* create message queue */
  osCreateMesgQueue(&msgQ, msgbuf, MAX_MESGS);

  /* add client to scheduler */
  nnScAddClient((NNSched*)arg, &client, &msgQ);

  while(1){
    (void)osRecvMesg(&msgQ,NULL, OS_MESG_BLOCK);
    controllerReadStart();
    (void)osRecvMesg(&controllerMsgQ, NULL, OS_MESG_BLOCK);
    osContGetReadData(controllerdata);

    for (i=0; i<numControllers; i++) {
      button = controllerdata_ptr[i]->button;
      controllerdataTriger[i].button = 
	button & (~lastbutton[i]);
      lastbutton[i]=button;
    }
  }
}

/*
 *  controller thread make
 *
 *  priority: thread priority
 */
void controllerThreadMake(NNSched *sched)
{
  osCreateThread(&controllerThread, 2, controllerProc, (void *)sched,
		 controllerThreadStack+STACKSIZE/sizeof(u64),
		 CONTROLLER_THREAD_PRI);
  osStartThread(&controllerThread);
}

