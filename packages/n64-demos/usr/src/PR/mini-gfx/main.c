/*---------------------------------------------------------------------
  $Id: main.c,v 1.1.1.1 2002/05/02 03:27:18 blythe Exp $
  
  File : main.c

  Coded     by Yoshitaka Yasumoto.   Apr 11, 1997.
  Copyright by Nintendo, Co., Ltd.           1997.
  ---------------------------------------------------------------------*/
#include	<ultra64.h>
#include	"system.h"

extern	Gfx	clearCfb[];

/*---------------------------------------------------------------------------*
 * Define Task Data
 *---------------------------------------------------------------------------*/
OSTask	tlist = {
  M_GFXTASK,					/* task type                */
  OS_TASK_DP_WAIT|OS_TASK_LOADABLE,		/* task flags               */
  (u64 *)&rspbootTextStart,			/* boot ucode ptr           */
  SP_BOOT_UCODE_SIZE,				/* boot ucode size          */
  (u64 *)&gspF3DEX_fifoTextStart,		/* ucode ptr                */
  SP_UCODE_SIZE,				/* ucode size               */
  (u64 *)&gspF3DEX_fifoDataStart,		/* ucode data ptr           */
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
  
  while(1){
    /*------ リトレース待ち ------*/
    osRecvMesg(&retraceMessageQ, NULL, OS_MESG_BLOCK);

    /*------ RSP 初期設定 ------*/
    gp = glist;

    /*------ GBI 設定 ------*/
    gSPSegment(gp ++, CFB_SEGMENT, system_cfb[draw_frame]);
    gSPDisplayList(gp ++, clearCfb);
    
    /*------ GBI 終了処理 ------*/
    gDPFullSync(gp ++);
    gSPEndDisplayList(gp ++);		// 終端

    /*------ Gfx タスク実行 ------*/
    tlist.t.data_ptr = (u64 *)glist;
    osWritebackDCache(glist, ((u32)gp)-((u32)glist));
    osSpTaskStart(&tlist);

    /*------ 終了待ち ------*/
    osRecvMesg(&rspMessageQ, NULL, OS_MESG_BLOCK);
    osRecvMesg(&rdpMessageQ, NULL, OS_MESG_BLOCK);    
    
    /*------ Frame 切り替え ------*/
    osViSwapBuffer(system_cfb[draw_frame]);
    draw_frame ^= 1;
  }
}

/*======== End of main.c ========*/
