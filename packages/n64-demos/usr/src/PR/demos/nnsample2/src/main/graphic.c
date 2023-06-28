/*============================================================================

		NINTENDO64 TECHNICAL SUPPORT CENTER 
		
		    NINTENDO64 SAMPLE PROGRAM 2

		Copyright (C) 1997, NINTENDO Co., Ltd.

============================================================================*/
#include <ultra64.h>
#include <PR/ramrom.h>
#include <assert.h>

#include "def.h"
#include "vram.h"
#include "segment.h"
#include "message.h"
#include "nnsched.h"
#include "graphic.h"

/* graphics microcode*/
u32 gfx_ucode[][2] = {
  {(u32)gspF3DEX2_fifoTextStart, (u32)gspF3DEX2_fifoDataStart},
};

OSMesgQueue  gfx_msgQ;
OSMesg       gfx_msgbuf[GFX_MESGS_MAX];
NNScClient gfx_client;
u32          gfx_cfbdrawbuffer = 0;
u32          graphic_no = GFX_NULL;
OSThread     gfxThread;

OSMesgQueue  *sched_gfxMQ;

GFXMsg       gfx_msg;
GFXMsg       gfx_msg_no;
u64          gfxThreadStack[STACKSIZE/sizeof(u64)];
NNScTask     gfx_task[GFX_GTASK_NUM];
Gfx          gfx_glist[GFX_GTASK_NUM][GFX_GLIST_LEN];
Dynamic      gfx_dynamic[GFX_GTASK_NUM];
Gfx*         glist_ptr;
u32          gfx_gtask_no;
CLookAt      lookat;
u32          pendingGFX = 0;
extern void  graphic_00(void);
extern void  graphic_01(void);


/*-----------------------------------------------------------------------------
Initialize graphic parameters
-----------------------------------------------------------------------------*/
void gfxInit(u8* gfxdlistStart_ptr)
{

  /* set graphic end message */
  gfx_msg.gen.type = NN_SC_DONE_MSG;  /* end task, swap frame buffer message */
  gfx_msg_no.gen.type = NN_SC_GTASKEND_MSG;  /* just end task message */

  graphic_no = GFX_NULL; /* initialize graphic thread branch flag */
  gfx_gtask_no = 0;
}

/*-----------------------------------------------------------------------------
 Graphic thread
-----------------------------------------------------------------------------*/
void gfxproc(void* arg)
{
  short* msg_type = NULL;

  pendingGFX = 0;
  /*  create message queue for VI retrace */
  osCreateMesgQueue(&gfx_msgQ, gfx_msgbuf, GFX_MESGS_MAX);

  nnScAddClient((NNSched*)arg, &gfx_client, &gfx_msgQ);

  /* Get graphic message queue */
  sched_gfxMQ = nnScGetGfxMQ((NNSched*)arg);

  /*
     Graphic process loop 
     The NMI(V-SYNC) process portion of SHVC
  */
  while(1){
    (void)osRecvMesg(&gfx_msgQ,(OSMesg *)&msg_type, OS_MESG_BLOCK);

    switch(*msg_type){
    case NN_SC_RETRACE_MSG:    /* retrace message process */

      switch(graphic_no){
      case GFX_00:   /*controller model screen */
	if(pendingGFX <2){
	  graphic_00();
	  pendingGFX++;
	}
	break;
      case GFX_01:  /* fighter plane screen */
      case GFX_10:
      case GFX_20:
      case GFX_30:
      case GFX_NULL:
	break;
      }
      break;
    case NN_SC_DONE_MSG:   /* end graphic task message */
      pendingGFX--;
      break;
    case NN_SC_PRE_NMI_MSG: /* PRE NMI message */
      /* please write a PRE NMI message process here */

      /* !! Important !!  Reset Y scale to 1.0 */
      osViSetYScale(1.0);

      /* so no task is created */
      pendingGFX+=2;
      break;
    }
  }
}

/*-----------------------------------------------------------------------------
Create and start graphic thread
-----------------------------------------------------------------------------*/
void gfxCreateGraphicThread(NNSched* sched)
{
  osCreateThread(&gfxThread, GRAPHIC_THREAD_ID, gfxproc,(void *) sched,
		 gfxThreadStack+STACKSIZE/sizeof(u64),
		 GRAPHIC_THREAD_PRI);
  osStartThread(&gfxThread);
}

/*----------------------------------------------------------------------------
  gfxWaitMessage

Wait for message from scheduler 
 
Return : Types of messages from scheduler 
                  OS_SC_RETRACE_MSG :  retrace message 
	  OS_SC_PRE_NMI_MSG:    PRE NMI message
----------------------------------------------------------------------------*/
short gfxWaitMessage(void)
{
  short* msg_type = NULL;
  (void)osRecvMesg(&gfx_msgQ,(OSMesg *)&msg_type, OS_MESG_BLOCK);
  return *msg_type;
}
/*----------------------------------------------------------------------------
  gfxSendMessage

Send message to scheduler to start graphic task

  NNScTask *gtask :       task structure
  Gfx*        glist_ptr :        display list pointer
  s32         glsit_size:        display list size
  u32         ucode_type:    microcode type ( refer to graphic.c) 
  u32         flag:                   change frame buffer flag
----------------------------------------------------------------------------*/
void gfxTaskStart(NNScTask *gtask,Gfx* glist_ptr, s32 glist_size,
		    u32 ucode_type, u32 flag)
{

  gtask->list.t.data_ptr = (u64 *)glist_ptr;
  gtask->list.t.data_size = glist_size;

  gtask->list.t.type = M_GFXTASK;
  gtask->list.t.flags = OS_TASK_LOADABLE;
  gtask->list.t.ucode_boot = (u64 *)rspbootTextStart;
  gtask->list.t.ucode_boot_size = ((s32) rspbootTextEnd 
				  - (s32) rspbootTextStart);
  gtask->list.t.ucode =(u64*) gfx_ucode[ucode_type][0];
  gtask->list.t.ucode_data =  (u64 *)gfx_ucode[ucode_type][1];
  gtask->list.t.ucode_data_size = SP_UCODE_DATA_SIZE;
  gtask->list.t.dram_stack = (u64 *) dram_stack;
  gtask->list.t.dram_stack_size = SP_DRAM_STACK_SIZE8;
  gtask->list.t.output_buff =  (u64 *)&rdp_output[0];
  gtask->list.t.output_buff_size =  (u64 *)&rdp_output[GFX_RDP_OUTPUT_SIZE];

  gtask->list.t.yield_data_ptr = (u64 *) gfxYieldBuf;
  gtask->list.t.yield_data_size = OS_YIELD_DATA_SIZE;

  gtask->next        = 0;
  gtask->flags       = flag;
  gtask->msgQ        = &gfx_msgQ;

  /* set task over message */
  if(flag & NN_SC_SWAPBUFFER){
    /* if  one whole screen task is complete*/
    gtask->msg         = (OSMesg)&gfx_msg;
  } else {
    /* if  one screen task not complete */
    gtask->msg         = (OSMesg)&gfx_msg_no;
  }

  gtask->framebuffer = cfb[gfx_cfbdrawbuffer];

  /* start graphic task */
  osSendMesg(sched_gfxMQ, (OSMesg *) gtask, OS_MESG_BLOCK);

  /* swap frame buffer */
  if(flag & NN_SC_SWAPBUFFER){
    gfx_cfbdrawbuffer ^= 1;
  }

  /* swap display list task buffer */
  gfx_gtask_no++;
  gfx_gtask_no %= GFX_GTASK_NUM;
}

/*----------------------------------------------------------------------------
  gfxRCPIinit
  Initialize RSP RDP
----------------------------------------------------------------------------*/
void gfxRCPInit(void)
{
  static int rdpinit_flag = 1;    /* RDP initialization flag */

  /* RSP segment register settings */
  gSPSegment(glist_ptr++, 0, 0x0);  /* for CPU virtual address use */

    /* RSP settings */
  gSPDisplayList(glist_ptr++, OS_K0_TO_PHYSICAL(setup_rspstate));

  /* initialize RDP (only once) */
  if(rdpinit_flag){
    gSPDisplayList(glist_ptr++, OS_K0_TO_PHYSICAL(rdpstateinit_dl));
    rdpinit_flag = 0;
  }
  /* RDP settings */
  gSPDisplayList(glist_ptr++, OS_K0_TO_PHYSICAL(setup_rdpstate));
}

/*----------------------------------------------------------------------------
  gfxClearCfb

  Clear frame buffer/Z buffer
----------------------------------------------------------------------------*/
void gfxClearCfb(void)
{
  /* clear Z buffer*/
  gDPSetDepthImage(glist_ptr++, OS_K0_TO_PHYSICAL(zbuffer));
  gDPPipeSync(glist_ptr++);
  gDPSetCycleType(glist_ptr++, G_CYC_FILL);
  gDPSetColorImage(glist_ptr++, G_IM_FMT_RGBA, G_IM_SIZ_16b,SCREEN_WD,
		   OS_K0_TO_PHYSICAL(zbuffer));
  gDPSetFillColor(glist_ptr++,(GPACK_ZDZ(G_MAXFBZ,0) << 16 |
			       GPACK_ZDZ(G_MAXFBZ,0)));
  gDPFillRectangle(glist_ptr++, 0, 0, SCREEN_WD-1, SCREEN_HT-1);
  gDPPipeSync(glist_ptr++);
  
    /* clear frame buffer */
  gDPSetColorImage(glist_ptr++, G_IM_FMT_RGBA, G_IM_SIZ_16b, SCREEN_WD,
		   osVirtualToPhysical(cfb[gfx_cfbdrawbuffer]));
  gDPSetFillColor(glist_ptr++, (GPACK_RGBA5551(0, 0, 0, 1) << 16 | 
				GPACK_RGBA5551(0, 0, 0, 1)));
  gDPFillRectangle(glist_ptr++, 0, 0, SCREEN_WD-1, SCREEN_HT-1);
  gDPPipeSync(glist_ptr++);

}

