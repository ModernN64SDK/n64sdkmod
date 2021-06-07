/*============================================================================
  graphic.c
  ============================================================================*/
#define F3DEX_GBI  /* F3DEX ucode */
#include <ultra64.h>
#include <PR/ramrom.h>
#include <assert.h>

#include "def.h"
#include "vram.h"
#include "segment.h"
#include "message.h"
#include "nnsched.h"
#include "graphic.h"

/* Graphic micro code */
u32 gfx_ucode[][2] = {
    {(u32)gspF3DEX_fifoTextStart, (u32)gspF3DEX_fifoDataStart},
    {(u32)gspF3DLX_fifoTextStart, (u32)gspF3DLX_fifoDataStart},
    {(u32)gspF3DLX_Rej_fifoTextStart,(u32)gspF3DLX_Rej_fifoDataStart},
    {(u32)gspF3DLP_Rej_fifoTextStart,(u32)gspF3DLP_Rej_fifoDataStart},
    {(u32)gspL3DEX_fifoTextStart, (u32)gspL3DEX_fifoDataStart},
    {(u32)gspLine3D_fifoTextStart, (u32)gspLine3D_fifoDataStart},
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
NNScTask   gfx_task[GFX_GTASK_NUM];
Gfx          gfx_glist[GFX_GTASK_NUM][GFX_GLIST_LEN];
Dynamic      gfx_dynamic[GFX_GTASK_NUM];
Gfx*         glist_ptr;
u32          gfx_gtask_no;
CLookAt      lookat;
u32          pendingGFX = 0;
extern void graphic_00(void);

/*-----------------------------------------------------------------------------
  Initialize graphic parameter
  -----------------------------------------------------------------------------*/
void gfxInit(u8* gfxdlistStart_ptr)
{

    /* Set graphic task end message */
    gfx_msg.gen.type = NN_SC_DONE_MSG; /* End task/switch buffer message  */
    gfx_msg_no.gen.type = NN_SC_GTASKEND_MSG; /*End task message only */

    graphic_no = GFX_NULL;	/*Initialize graphic thread branch flag */
    gfx_gtask_no = 0;
}

/*-----------------------------------------------------------------------------
Graphic thread
  -----------------------------------------------------------------------------*/
void gfxproc(void* arg)
{
    short* msg_type = NULL;

    pendingGFX = 0;
    /*  create message queue for VI reatrace */
    osCreateMesgQueue(&gfx_msgQ, gfx_msgbuf, GFX_MESGS_MAX);

    nnScAddClient((NNSched*)arg, &gfx_client, &gfx_msgQ);

    /*Acquire graphic message queue */
    sched_gfxMQ = nnScGetGfxMQ((NNSched*)arg);

    /*
       graphic processing loop
       the portion to be NMI(V-SYNC) processed in SHVC 
       */
    while(1){
	(void)osRecvMesg(&gfx_msgQ,(OSMesg *)&msg_type, OS_MESG_BLOCK);

	switch(*msg_type){
	  case NN_SC_RETRACE_MSG: /* Retrace message processing */
	    switch(graphic_no){
	      case GFX_00:	/*controller model screen */
		if(pendingGFX <1){
		    graphic_00();
		    pendingGFX++;
		}
		break;
	      case GFX_NULL:
		break;
	    }
	    break;
	  case NN_SC_DONE_MSG:	/* graphic task end message*/
	    pendingGFX--;
	    break;
	  case NN_SC_PRE_NMI_MSG: /* PRE NMI message */
	    /* !!important!! Make Y scale value back to 1.0  */
	    osViSetYScale(1.0);

	    /*do not create task */
	    pendingGFX+=2;
	    break;
	}
    }
}

/*-----------------------------------------------------------------------------
  Create and activate graphic thread
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
  return : message type from scheduler
  OS_SC_RETRACE_MSG    :retrace message
  OS_SC_PRE_NMI_MSG    :  PRE NMI message
  ----------------------------------------------------------------------------*/
short gfxWaitMessage(void)
{
    short* msg_type = NULL;
    (void)osRecvMesg(&gfx_msgQ,(OSMesg *)&msg_type, OS_MESG_BLOCK);
    return *msg_type;
}
/*----------------------------------------------------------------------------
  gfxSendMessage
  
Send graphic task activating message to scheduler
  NNScTask *gtask     :task structure
  Gfx*        glist_ptr :pointer of display list
  s32         glist_size:display list size
  u32         ucode_type:type of micro code (see graphic.h)
  u32         flag      :flag to change frame buffer
  ----------------------------------------------------------------------------*/
void gfxTaskStart(NNScTask *gtask,Gfx* glist_ptr, s32 glist_size,
		  u32 ucode_type, u32 flag)
{

    gtask->list.t.data_ptr = (u64 *)glist_ptr;
    gtask->list.t.data_size = glist_size;

    gtask->list.t.type = M_GFXTASK;
    gtask->list.t.flags = 0x0;
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

    /*Set task end message*/
    if(flag & NN_SC_SWAPBUFFER){
	/* if task for the amount of 1 screen ended */
	gtask->msg         = (OSMesg)&gfx_msg;
    } else {
	/* if task for the amount of 1 screen has not ended */
	gtask->msg         = (OSMesg)&gfx_msg_no;
    }

    gtask->framebuffer = cfb[gfx_cfbdrawbuffer];

    /* Activate graphic task */
    osSendMesg(sched_gfxMQ, (OSMesg *) gtask, OS_MESG_BLOCK);

    /* Switch frame buffer */
    if(flag & NN_SC_SWAPBUFFER){
	gfx_cfbdrawbuffer ^= 1;
    }

    /* Switch display list, task buffer */
    gfx_gtask_no++;
    gfx_gtask_no %= GFX_GTASK_NUM;
}

/*----------------------------------------------------------------------------
  gfxRCPIinit
  
  Initialize RSP RDP 
  ----------------------------------------------------------------------------*/
void gfxRCPInit(void)
{
    static int rdpinit_flag = 1; /* flag to initialize RDP  */

    /* set RSP segment register  */
    gSPSegment(glist_ptr++, 0, 0x0); /* for CPU virtual address */

    /* set RSP  */
    gSPDisplayList(glist_ptr++, OS_K0_TO_PHYSICAL(setup_rspstate));

    /* initialize RDP (one time only)  */
    if(rdpinit_flag){
	gSPDisplayList(glist_ptr++, OS_K0_TO_PHYSICAL(rdpstateinit_dl));
	rdpinit_flag = 0;
    }
    /* set RDP*/
    gSPDisplayList(glist_ptr++, OS_K0_TO_PHYSICAL(setup_rdpstate));
}

/*----------------------------------------------------------------------------
  gfxClearCfb
  
Clear frame buffer/Z buffer
  ----------------------------------------------------------------------------*/
void gfxClearCfb(void)
{
    /* clear Z buffer */
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


void
DrawBg_16b( int posx, int posy, int width, int height, u16 *bmp )
/*DrawBg_16b( int posx, int posy, int width, int height, unsigned short *bmp )*/
{
    int idx;

/*    gSPSegment(glist_ptr++, STATIC_SEGMENT, osVirtualToPhysical(gfx_dlist_buf[0]));*/
    for(idx = 0; idx < height; idx++){
	gDPSetCycleType(glist_ptr++, G_CYC_1CYCLE);
	gDPSetTextureFilter(glist_ptr++, G_TF_POINT);
	gDPSetRenderMode(glist_ptr++, G_RM_TEX_EDGE, G_RM_TEX_EDGE);
	
	gSPTexture(glist_ptr++, 0xffff, 0xffff, 0, G_TX_RENDERTILE, G_ON);
	gDPSetCombineMode(glist_ptr++, G_CC_DECALRGBA, G_CC_DECALRGBA);
	gDPSetTexturePersp(glist_ptr++, G_TP_NONE);
	
	gDPSetTextureLUT(glist_ptr++, G_TT_NONE);
	
	gDPLoadTextureTile(glist_ptr++,
			   bmp,
			   G_IM_FMT_RGBA,
			   G_IM_SIZ_16b,
			   width, height,
			   0, idx, width-1, idx+1,
			   0,
			   G_TX_WRAP, G_TX_WRAP,
			   0, 0,
			   G_TX_NOLOD, G_TX_NOLOD);
	gSPTextureRectangle(glist_ptr++,
			    (int)(posx) << 2,
			    (int)(posy+idx) << 2,
			    (int)(posx+width) << 2,
			    (int)(posy+idx+1) << 2,
			    G_TX_RENDERTILE,
			    (0 << 5),
			    (0 << 5),
			    (int)(1024.0f),
			    (int)(1024.0f)
			    );
	gDPPipeSync(glist_ptr++);
    }
}

void
DrawBg_16b2( int posx, int posy, int width, int height, u16 *bmp )
{
    int idx;

/*    gSPSegment(glist_ptr++, STATIC_SEGMENT, osVirtualToPhysical(gfx_dlist_buf[0]));*/
    for(idx = 0; idx < height; idx++){
	gDPSetCycleType(glist_ptr++, G_CYC_1CYCLE);
	gDPSetTextureFilter(glist_ptr++, G_TF_POINT);
	gDPSetRenderMode(glist_ptr++, G_RM_TEX_EDGE, G_RM_TEX_EDGE);
	
	gSPTexture(glist_ptr++, 0xffff, 0xffff, 0, G_TX_RENDERTILE, G_ON);
	gDPSetCombineMode(glist_ptr++, G_CC_DECALRGBA, G_CC_DECALRGBA);
	gDPSetTexturePersp(glist_ptr++, G_TP_NONE);
	
	gDPSetTextureLUT(glist_ptr++, G_TT_NONE);
	
	gDPLoadTextureTile(glist_ptr++,
			   bmp,
			   G_IM_FMT_RGBA,
			   G_IM_SIZ_16b,
			   width, height,
			   0, idx, width-1, idx+1,
			   0,
			   G_TX_WRAP, G_TX_WRAP,
			   0, 0,
			   G_TX_NOLOD, G_TX_NOLOD);
	gSPTextureRectangle(glist_ptr++,
			    (int)(posx) << 2,
			    (int)(posy+idx) << 2,
			    (int)(posx+width) << 2,
			    (int)(posy+idx+1) << 2,
			    G_TX_RENDERTILE,
			    (int)(0 << 5),
			    (int)(0 << 5),
			    (int)(1 << 10),
			    (int)(1 << 10)
			    );
	gDPPipeSync(glist_ptr++);
    }
}

/******************************************
  Draw window using gDPFillRectangle()   
  ******************************************/
void
DrawWindow( int xpos, int ypos, int width, int height, int r, int g, int b, int alpha)
{
    gDPSetRenderMode(glist_ptr++ , G_RM_NOOP, G_RM_NOOP2);
    gDPSetCycleType(glist_ptr++, G_CYC_FILL);		/*fill mode */

    gDPSetFillColor(glist_ptr++,
		    GPACK_RGBA5551( r , g , b , alpha ) << 16 |
		    GPACK_RGBA5551( r , g , b , alpha ) );

    gDPFillRectangle( glist_ptr++, xpos, ypos, xpos + width, ypos + height );

    gDPPipeSync(glist_ptr++);
    gDPSetCycleType(glist_ptr++, G_CYC_1CYCLE);		/* return to original state */
}
