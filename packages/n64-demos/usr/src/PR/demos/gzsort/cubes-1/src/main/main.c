/*---------------------------------------------------------------------*
	Copyright (C) 1997, Nintendo.
	
	File		main.c
	Coded    by	Yoshitaka Yasumoto.	Sep 12, 1997.
	Modified by	
	
	$Id: main.c,v 1.4 1997/10/09 09:58:21 yasu Exp $
 *---------------------------------------------------------------------*/
#include	<ultra64.h>
#include	<PR/gzsort.h>
#include	"system.h"
#include	"action.h"
#include	"uc_assert.h"

extern	Gfx	clearCfb[];
extern	Gfx	gfxFinish[];
extern	void	cubeInit(void);
extern	void	cubePass1(Gfx **);
extern	void	cubePass2(Gfx **);

/*---------------------------------------------------------------------------*
 * Define Task Data
 *---------------------------------------------------------------------------*/
OSTask	tlist = {
  M_GFXTASK,					/* task type                */
  OS_TASK_LOADABLE,				/* task flags               */
  (u64 *)&rspbootTextStart,			/* boot ucode ptr           */
  SP_BOOT_UCODE_SIZE,				/* boot ucode size          */
  (u64 *)&gspZSort_fifoTextStart,		/* ucode ptr                */
  SP_UCODE_SIZE,				/* ucode size               */
  (u64 *)&gspZSort_fifoDataStart,		/* ucode data ptr           */
  SP_UCODE_DATA_SIZE,				/* ucode data size          */
  NULL, 					/* dram stack      (不使用) */
  0,						/* dram stack size (不使用) */
  (u64 *)system_rdpfifo,			/* fifo buffer top          */
  (u64 *)system_rdpfifo+RDPFIFO_SIZE,		/* fifo buffer bottom       */
  NULL,						/* data ptr      (後で設定) */
  NULL, 					/* data size     (設定不要) */
  (u64 *)system_rspyield,			/* yield data ptr           */
  OS_YIELD_DATA_SIZE,				/* yield data size          */
};

Gfx     glist[GLIST_LEN];

/*---------------------------------------------------------------------------*
 * M A I N 
 *---------------------------------------------------------------------------*/
void	Main(void *arg)
{
  u8	 draw_frame = 0;
  Gfx	*gp;
  OSTime framestart;
  u32	 rsp1start, rsp1end, rsp2start, rsp2end, rdpend;
  
  actionInit();
  cubeInit();
  
  while(1){

    /*------ Start Reading Controller ------*/
    osContStartReadData(&siMessageQ);
    
    /*------ Retrace Stand-by ------*/
    osRecvMesg(&retraceMessageQ, NULL, OS_MESG_NOBLOCK);
    osRecvMesg(&retraceMessageQ, NULL, OS_MESG_BLOCK);
    framestart = osGetTime();

    /*=======================================================================*
     *    Z S o r t  P a s s  1
     *=======================================================================*/
    /*------ Create Gfx List ------*/
    gp = glist;
    cubePass1(&gp);
    gSPZEndDisplayList(gp ++);
    
    /*------ Run Gfx Task ------*/
    tlist.t.data_ptr = (u64 *)glist;
    osWritebackDCache(glist, ((u32)gp)-((u32)glist));
    rsp1start = OS_CYCLES_TO_USEC(osGetTime() - framestart) / 100;
    osSpTaskStart(&tlist);
    
    /*------ End Stand-by ------*/
#ifndef	xDEBUG
    osRecvMesg(&rspMessageQ, NULL, OS_MESG_BLOCK);
#else
  {
    int cnt = 0;
    while (osRecvMesg(&rspMessageQ, NULL, OS_MESG_NOBLOCK)){
      if (0 == osRecvMesg(&retraceMessageQ, NULL, OS_MESG_NOBLOCK)){
	if (cnt ++ == 10){
	  ucDebugDMEMPrint();
	  ucDebugInfoPrint();
	  while(1);
	}
      }
    }
  }
#endif    
    rsp1end = OS_CYCLES_TO_USEC(osGetTime() - framestart) / 100;

#ifdef	DEBUG
    /*------ Display RSP Condition for DEBUG ------*/
    if (Ac.pad[0].push & A_BUTTON){
      ucDebugRdpFifoPrint(&tlist);
      ucDebugAssertPrint();
      ucDebugInfoPrint();
      osSyncPrintf("RSP %d/%d\n",
		   IO_READ(SP_DMEM_START), IO_READ(SP_DMEM_START+4));
    }
    if (Ac.pad[0].push & B_BUTTON){
      ucDebugIMEMPrint();
      ucDebugDMEMPrint();
    }
#endif
    
    /*=======================================================================*
     *    Z S o r t  P a s s  2
     *=======================================================================*/

    /*------ RSP Initial Settings ------*/
    gp = glist;
    gSPZSegment(gp ++, CFB_SEGMENT, system_cfb[draw_frame]);
    gSPZRdpCmd(gp ++, clearCfb);

    /*------ Create Gfx List ------*/    
    cubePass2(&gp);
    
    /*------ Processing Meter Output ------*/
    gSPZRdpCmd(gp ++, gfxFinish);
    gSPZEndDisplayList(gp ++);
    
    /*------ Run Gfx Task ------*/
    tlist.t.data_ptr = (u64 *)glist;
    osWritebackDCache(glist, ((u32)gp)-((u32)glist));
    osWritebackDCache(gfxFinish+3, sizeof(Gfx)*9);
    rsp2start = OS_CYCLES_TO_USEC(osGetTime() - framestart) / 100;
    osSpTaskStart(&tlist);

    /*------ End Stand-by ------*/
#ifndef	xDEBUG
    osRecvMesg(&rspMessageQ, NULL, OS_MESG_BLOCK);
#else
  {
    int cnt = 0;
    while (osRecvMesg(&rspMessageQ, NULL, OS_MESG_NOBLOCK)){
      if (0 == osRecvMesg(&retraceMessageQ, NULL, OS_MESG_NOBLOCK)){
	if (cnt ++ == 10){
	  ucDebugDMEMPrint();
	  ucDebugInfoPrint();
	  while(1);
	}
      }
    }
  }
#endif    
    rsp2end = OS_CYCLES_TO_USEC(osGetTime() - framestart) / 100;
    osRecvMesg(&rdpMessageQ, NULL, OS_MESG_BLOCK);
    rdpend = OS_CYCLES_TO_USEC(osGetTime() - framestart) / 100;
    
    /*------ Processing Meter Change ------*/
    gDPFillRectangle(&gfxFinish[3],  30,           201, 29+rsp1start, 202);
    gDPFillRectangle(&gfxFinish[4],  30+rsp1end,   201, 29+rsp2start, 202);
    gDPFillRectangle(&gfxFinish[7],  30+rsp1start, 204, 29+rsp1end,   205);
    gDPFillRectangle(&gfxFinish[8],  30+rsp2start, 204, 29+rsp2end,   205);
    gDPFillRectangle(&gfxFinish[11], 30+rsp2start, 207, 29+rdpend,    208);

    /* Frame Swapping */
    osViSwapBuffer(system_cfb[draw_frame]);
    draw_frame ^= 1;

    /* Receive Controller Data */
    osRecvMesg(&siMessageQ, NULL, OS_MESG_BLOCK);
    osContGetReadData(contPad);

    /*------ Controller Processing ------*/
    actionUpdate();
  }
}

/*======== End of main.c ========*/
