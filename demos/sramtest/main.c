/*---------------------------------------------------------------------
  File   : main.c
  Copyright by Nintendo, Co., Ltd. 1997.
  ---------------------------------------------------------------------*/
#include	<ultra64.h>
#include	"nu64sys.h"
#include	"sram.h"

#define NUM_PI_MSGS     8

public  u64             bootStack[STACKSIZE/8];
private void            idle(void *);
private OSThread        idleThread;
private u64             idleThreadStack[STACKSIZE/8];
extern  void            mainproc(void *);
private OSThread        mainThread;
private u64             mainThreadStack[STACKSIZE/8];
private OSMesg          PiMessages[NUM_PI_MSGS];
private OSMesgQueue     PiMessageQ;
public  OSMesgQueue     n_dmaMessageQ;
private OSMesg          dmaMessageBuf;

extern 		OSPiHandle *SramInit(void);
extern  u32	writebuf[];
extern  u32	readbuf[];
OSPiHandle      *handler;
OSIoMesg	MyIoMesg;

void SRAM_DMA(u32 rw_flag, u32 *data_addr)
{

  osWritebackDCache(data_addr, SRAM_SIZE);

  MyIoMesg.hdr.pri  =  OS_MESG_PRI_NORMAL;
  MyIoMesg.hdr.retQueue  =  &n_dmaMessageQ;
  MyIoMesg.dramAddr  =  data_addr;
  MyIoMesg.devAddr  =  SRAM_START_ADDR;
  MyIoMesg.size  = SRAM_SIZE;

  osEPiStartDma(handler, &MyIoMesg, rw_flag);
  (void)osRecvMesg(&n_dmaMessageQ, NULL, OS_MESG_BLOCK);
}

u32 rand (void)
{
  static unsigned long seed = 1;
  seed = seed * 1103515245 + 12345;
  return seed;
}

void boot(void)
{  
  osInitialize();
  osCreateThread(&idleThread, TID_IDLE, idle, (void*)0,
                 idleThreadStack+STACKSIZE/8, 10);
  osStartThread(&idleThread);
}

void	idle(void *arg)
{
  osCreatePiManager((OSPri)OS_PRIORITY_PIMGR,
		    &PiMessageQ, PiMessages, NUM_PI_MSGS);
  osCreateMesgQueue(&n_dmaMessageQ, &dmaMessageBuf, 1);
  osCreateThread(&mainThread, TID_MAINPROC, mainproc, (void *)0,
		 (void *)(mainThreadStack+STACKSIZE/8), 10);
  osStartThread(&mainThread);
  osSetThreadPri(0, 0);
  while(1){
  }
}

void	mainproc(void *arg)
{	
  u32 i, result;

  handler = SramInit();
  for (i = 0; i < SRAM_SIZE/4; ++i)
    writebuf[i] = rand();

  /* test SRAM patern: RANDOM */
  SRAM_DMA(OS_WRITE, writebuf);
  SRAM_DMA(OS_READ, readbuf);
  if(bcmp(readbuf,writebuf,SRAM_SIZE/4)!=0)
    osSyncPrintf("fill random data test error\n");
  else
    osSyncPrintf("SRAM TEST OK!\n");
}
