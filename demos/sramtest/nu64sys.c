/*---------------------------------------------------------------------
  File  : nu64sys.c

  Created  by  Yoshitaka Yasumoto.   Jul,11 1995.
  Modified by  
  
  Copyright by Nintendo, Co., Ltd. 1997.
  ---------------------------------------------------------------------*/
#include	<ultra64.h>
#include	<ramrom.h>
#include	"nu64sys.h"
#include	"thread.h"

/*
 *		BOOT section
 */
public	u64		bootStack[STACKSIZE/8];

/*
 *		IDLE thread
 */
private	void		idle(void *);
private	OSThread	idleThread;
private	u64		idleThreadStack[STACKSIZE/8];

/*
 *		MAIN thread
 */
extern	void		mainproc(void *);
private	OSThread	mainThread;
private	u64		mainThreadStack[STACKSIZE/8];

/*
 *		Pi message queue
 */
#define	NUM_PI_MSGS     8
private	OSMesg		PiMessages[NUM_PI_MSGS];
private	OSMesgQueue	PiMessageQ;

/*
 *		Other message queues
 */
public	OSMesgQueue	n_dmaMessageQ,	n_rdpMessageQ;
private	OSMesg		dmaMessageBuf,	rdpMessageBuf;

/*
 *		Cartridge message queues
 */
public	OSMesgQueue	cartMessageQ;
private	OSMesg		cartMessageBuf;


/*---------------------------------------------------------------------*
 *	IDLE THREAD (Pri.= 0)
 *---------------------------------------------------------------------*/
private	void	idle(void *arg)
{
  
  int i;
  OSMesg	dummyMesg;
  unsigned char   pattern;
  
  /*
   *		Initialize Pi manager
   */
  osCreatePiManager((OSPri)OS_PRIORITY_PIMGR,
		    &PiMessageQ, PiMessages, NUM_PI_MSGS);
  
  /*
   *		Setup message queue
   */
  osCreateMesgQueue(&n_dmaMessageQ, &dmaMessageBuf, 1);		/* DMA */

  osCreateMesgQueue(&cartMessageQ, &cartMessageBuf, 1);
  osSetEventMesg(OS_EVENT_CART, &cartMessageQ, (OSMesg)1);
    

  /*
   *		Create & start MAINPROC thread
   */  
  osCreateThread(&mainThread, TID_MAINPROC, mainproc, (void *)0,
		 (void *)(mainThreadStack+STACKSIZE/8), 10);
  
  osStartThread(&mainThread);
  
  /*
   *		Now idling
   */
  osSetThreadPri(0, 0);
  while(1){
  }
}

/*---------------------------------------------------------------------*
 *	BOOT PART
 *---------------------------------------------------------------------*/
public void boot(void)
{  
  /*
   *		Initialize OS
   */

  osInitialize();
    
  /*
   *		Create idle thread & start it
   */
  osCreateThread(&idleThread, TID_IDLE, idle, (void*)0,
		 idleThreadStack+STACKSIZE/8, 10);
  osStartThread(&idleThread);
}

