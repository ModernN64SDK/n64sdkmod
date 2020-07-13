/*======================================================================*/
/*		NuSYS							*/
/*		nugfxtaskmgr.c						*/
/*									*/
/*		Copyright (C) 1997, NINTENDO Co,Ltd.			*/
/*									*/
/*----------------------------------------------------------------------*/    
/* Ver 1.0	97/10/9		Created by Kensaku Ohki(SLANP)		*/
/*======================================================================*/
/* $Id: nugfxtaskmgr.c,v 1.11 1999/07/27 01:28:59 ohki Exp $	*/
/*======================================================================*/
#include <nusys.h>


static NUScTask* nuGfxTask_ptr;

static short	taskDoneMsg;
static short	swapBufMsg;
static OSThread	GfxTaskMgrThread;		/* gfx taskmgr thread */
static u64	GfxTaskMgrStack[NU_GFX_TASKMGR_STACK_SIZE/sizeof(u64)];
static OSMesg	nuGfxTaskMgrMesgBuf[NU_GFX_TASKMGR_MESGS];


NUUcode*	nuGfxUcode;
NUScTask	nuGfxTask[NU_GFX_TASK_NUM];	/* Graphics task structure */
volatile u32	nuGfxTaskSpool;			/* Number of task spools */
OSMesgQueue	nuGfxTaskMgrMesgQ;



u16**		nuGfxCfb;	/* Pointer to frame buffer */
u32		nuGfxCfbNum = 1;/* Number of frame buffers Frame buffer */
u16*		nuGfxCfb_ptr;	/* Frame buffers to be rendered	*/
u16*		nuGfxZBuffer;	/* Pointer to Z-buffer	*/
u32		nuGfxDisplay;	/* Screen display on/off */
u32		nuGfxCfbCounter;
s32		nuGfxUcodeFifoSize = -1; /*FIFO buffer size -1:size undefined*/
u64*		nuGfxUcodeFifoPtr = NULL;/*Pointer to FIFO buffer */

NUGfxSwapCfbFunc nuGfxSwapCfbFunc = NULL; /* swapbuf callback function ptr */
NUGfxTaskEndFunc nuGfxTaskEndFunc = NULL; /* task end callback  ptr */

/*----------------------------------------------------------------------*/
/*	nuGfxTaskMgr - Task Manager					*/
/*	IN:	*arg	Task generation flag (not specific)		*/
/*									*/
/*	RET:	nothing							*/
/*----------------------------------------------------------------------*/
void nuGfxTaskMgr(void *arg)
{
    NUScMsg*	mesg_type;
    NUScTask*	gfxTask;
    OSIntMask	mask;
    
    /* Create the Task Manager message queue. */
    osCreateMesgQueue(&nuGfxTaskMgrMesgQ, nuGfxTaskMgrMesgBuf, NU_GFX_MESGS);

    /* Receive message that graphics task has ended,   */
    /* and perform postprocessing.		*/
    while(1){

	(void)osRecvMesg(&nuGfxTaskMgrMesgQ,(OSMesg*)&gfxTask, OS_MESG_BLOCK);
	/* Obtain message type.*/
	mesg_type = gfxTask->msg;

	/* Branch the processing depending on the message.*/
	switch(*mesg_type){
	case NU_SC_SWAPBUFFER_MSG:/* Task completed and frame buffer swapped. */
	    
	/*Call application function with message indicating frame buffer swap.*/	    /* The frame buffer swap function is registered by default.	*/
	    if(nuGfxSwapCfbFunc != NULL){
		(*nuGfxSwapCfbFunc)((void*)gfxTask);
	    }

	    if(nuGfxDisplay & NU_GFX_DISPLAY_ON_TRIGGER){
		osViBlack(FALSE);
		nuGfxDisplay = NU_GFX_DISPLAY_ON;
	    }
	    mask = osSetIntMask(OS_IM_NONE);
	    nuGfxTaskSpool--;
	    osSetIntMask(mask);
	    break;
	    
	case NU_SC_GTASKEND_MSG:	/* Task ended. */

	    /* Use the message indicating the end of the graphics task to call the application function.*/
	    /* Default is NULL.				*/
	    if(nuGfxTaskEndFunc != NULL){
		(*nuGfxTaskEndFunc)((void*)gfxTask);
	    }
	    mask = osSetIntMask(OS_IM_NONE);
	    nuGfxTaskSpool--;
	    osSetIntMask(mask);
	    break;
	}

    }
}



/*----------------------------------------------------------------------*/
/*	nuGfxTaskMgrInit - Initializes the Task Manager			*/
/*	IN:	nothing							*/
/*									*/
/*	RET:	nothing							*/
/*----------------------------------------------------------------------*/
void nuGfxTaskMgrInit(void)
{
    u32	cnt;

    taskDoneMsg	= NU_SC_GTASKEND_MSG;
    swapBufMsg	= NU_SC_SWAPBUFFER_MSG;
    nuGfxTaskSpool = 0;
    nuGfxDisplayOff();		/* Screen display off */

    /* Start the Graphics Task Manager thread. */
    osCreateThread(&GfxTaskMgrThread, NU_GFX_TASKMGR_THREAD_ID, nuGfxTaskMgr,
		   (void*)NULL,
		   (GfxTaskMgrStack + NU_GFX_TASKMGR_STACK_SIZE/sizeof(u64)),
		   NU_GFX_TASKMGR_THREAD_PRI );
    osStartThread(&GfxTaskMgrThread);
    
    /* Initialize the task structure.  */
    /* First, set the constants. */
    for(cnt = 0; cnt < NU_GFX_TASK_NUM; cnt++){
	 nuGfxTask[cnt].next			= &nuGfxTask[cnt+1];
	 nuGfxTask[cnt].msgQ			= &nuGfxTaskMgrMesgQ;
	 nuGfxTask[cnt].list.t.type		= M_GFXTASK;
	 nuGfxTask[cnt].list.t.flags		= 0x00;
	 nuGfxTask[cnt].list.t.ucode_boot	= (u64*)rspbootTextStart;
	 nuGfxTask[cnt].list.t.ucode_boot_size 	= ((s32) rspbootTextEnd 
						    - (s32) rspbootTextStart);
	 nuGfxTask[cnt].list.t.ucode_size 	= SP_UCODE_SIZE;
	 nuGfxTask[cnt].list.t.ucode_data_size 	= SP_UCODE_DATA_SIZE;
	 nuGfxTask[cnt].list.t.dram_stack	= (u64*) nuDramStack;
	 nuGfxTask[cnt].list.t.dram_stack_size 	= SP_DRAM_STACK_SIZE8;
//	 nuGfxTask[cnt].list.t.output_buff	= (u64 *)&nuRDPOutputBuf[0];
//	 nuGfxTask[cnt].list.t.output_buff_size =
//	     (u64 *)(nuGfxFifoBufPtr + nuGfxUcodeFifoSize);
	 nuGfxTask[cnt].list.t.yield_data_ptr	= (u64 *) nuYieldBuf;
	 nuGfxTask[cnt].list.t.yield_data_size	= NU_GFX_YIELD_BUF_SIZE;
     }
     nuGfxTask[NU_GFX_TASK_NUM-1].next = &nuGfxTask[0];
     nuGfxTask_ptr	= &nuGfxTask[0];

 }

/*----------------------------------------------------------------------*/
/*	nuGfxTaskStart - Starts the graphics task			*/
/*									*/
/*	Sends the graphics task startup message to the Scheduler.	*/
/*	For flag, specify one of the following values according to the  */
/*	specified UCODE.						*/
/*		NU_SC_NOSWAPBUFFER	Do not swap frame buffer after  */
/*	the task ends. 							*/
/*		NU_SC_SWAPBUFFER	Swap frame buffer after the task*/
/*	ends. 								*/
/*					Specify this as the final task  */
/*	in the frame. 							*/
/*					The Task Manager actually swaps */
/*	the frame buffer. 						*/
/*		NU_SC_NORDP		Do not use the RDP, or do not wait*/
/*					for messages from the RDP.  Usually*/
/*					specified with RDP-only microcode.*/
/*		NU_SC_UCODE_XBUS	Specify this with XBUS microcode .*/
/*		NU_SC_TASK_YIELDED	Equivalent to OS_TASK_YIELDED	*/
/*		NU_SC_TASK_DP_WAIT	Equivalent to OS_TASK_DP_WAIT	*/
/*		NU_SC_TASK_LODABLE	Equivalent to OS_TASK_LOADABLE	*/
/*					Specify when the self-loading   */
/*	feature of UCODE is used. 					*/
/*									*/
/*		NU_SC_TASK_SP_ONLY	Equivalent to OS_TASK_SP_ONLY   */
/*									*/
/*	IN:	gfxListptr	Pointer to the display list		*/
/*		gfxListSize 	Size of the display list		*/
/*		ucode		Microcode array number 			*/
/*		flag		Flag used when starting task 	 	*/
/*	RET:	nothing							*/
/*----------------------------------------------------------------------*/
 void nuGfxTaskStart(Gfx *gfxList_ptr, u32 gfxListSize, u32 ucode, u32 flag)
 {
     OSIntMask	mask;
     static u16	beforeFlag = 0;
	 
#ifdef NU_DEBUG
     if(!(flag & NU_SC_UCODE_XBUS) && (nuGfxUcodeFifoSize < 0)){
	 osSyncPrintf("nuGfxTaskStart: Must set FIFO buffer for fifo-ucode(use nuGfxSetUcodeFifo)\n");
	 return;
     }
#endif /* NU_DEBUG */
	 
     nuGfxTask_ptr->list.t.data_ptr	= (u64*)gfxList_ptr;
     nuGfxTask_ptr->list.t.data_size	= gfxListSize;
     nuGfxTask_ptr->list.t.flags	= flag >> 16;
     nuGfxTask_ptr->list.t.ucode 	= nuGfxUcode[ucode].ucode;
     nuGfxTask_ptr->list.t.ucode_data	= nuGfxUcode[ucode].ucode_data;
     nuGfxTask_ptr->list.t.output_buff	= nuGfxUcodeFifoPtr;
     nuGfxTask_ptr->list.t.output_buff_size =
	 (nuGfxUcodeFifoPtr + nuGfxUcodeFifoSize /sizeof(u64));
     nuGfxTask_ptr->flags		= flag & 0x0000ffff;
     nuGfxTask_ptr->framebuffer		= (u16*)nuGfxCfb_ptr;

     /* When the previously started microcode was XBUS microcode,       */
     /* the OSTask structure flag must to set to OS_TASK_DP_WAIT,       */
     /* so this is checked.  However, this is usually not problematic   */
     /* because the function waits for the RDP to finish as along as    */
     /* NU_SC_NORDP is not specified. 					*/
     
     if(beforeFlag & NU_SC_UCODE_XBUS){
	 nuGfxTask_ptr->list.t.flags |= OS_TASK_DP_WAIT;
	 beforeFlag ^= NU_SC_UCODE_XBUS;	 
     }
     beforeFlag = flag;
     
     /*  Set the message indicating the end of the task. */
     if(flag & NU_SC_SWAPBUFFER){
	 /* When 1 screen of the task has been completed */
	 nuGfxTask_ptr->msg         	= (OSMesg)&swapBufMsg;
	 
	 nuGfxCfbCounter = (nuGfxCfbCounter+1) % nuGfxCfbNum;
	 nuGfxCfb_ptr = nuGfxCfb[nuGfxCfbCounter];
     } else {
	 /* When 1 screen of the task has not been completed */
	 nuGfxTask_ptr->msg         = (OSMesg)&taskDoneMsg;
     }
     
     mask = osSetIntMask(OS_IM_NONE);
     nuGfxTaskSpool++;
     osSetIntMask(mask);
     
     /* Start the graphics task. */
     osWritebackDCacheAll();
     osSendMesg(&nusched.graphicsRequestMQ,
		(OSMesg*)nuGfxTask_ptr, OS_MESG_BLOCK);
     
     
     /* Change the task buffer to the next pointer. */
     nuGfxTask_ptr = nuGfxTask_ptr->next;
     
}
