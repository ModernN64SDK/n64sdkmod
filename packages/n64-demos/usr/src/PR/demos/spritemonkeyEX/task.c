/*
  task.c
    Copyright (C) 1999, NINTENDO Co,Ltd., MONEGI CORPORATION.
    Jan. 28, 1999.
*/

#include <ultra64.h>
#include <PR/gs2dex.h>
#include "Task.h"

#define RDP_OUTPUT_LEN ( 4096 * 16 )
#if __GNUC__ /* { */
u64	rdp_output_len  __attribute__((aligned (16)));			    /* RDP DL length returned by RSP */
#else /* }{ */
u64	rdp_output_len;			    /* RDP DL length returned by RSP */
#endif /* } */
u64	rdp_output[ RDP_OUTPUT_LEN ];		    /* buffer for RDP DL */

/*
 * RSP Task descriptor.
 */
OSTask task_list =
{
	M_GFXTASK,		     /* task type */
	OS_TASK_DP_WAIT,	     /* task flags */
	NULL,			     /* boot ucode pointer (fill in later) */
	0,			     /* boot ucode size (fill in later) */
	NULL,			     /* task ucode pointer (fill in later) */
	SP_UCODE_SIZE,		     /* task ucode size */
	NULL,			     /* task ucode data pointer (fill in later) */
	SP_UCODE_DATA_SIZE,	     /* task ucode data size */
	&(dram_stack[0]),	     /* task dram stack pointer */
	SP_DRAM_STACK_SIZE8,	     /* task dram stack size */
	&(rdp_output[0]),	     /* task output buffer ptr (not always used) */
	&rdp_output_len,	     /* task output buffer size ptr */
	NULL,			     /* task data pointer (fill in later) */
	0,			     /* task data size (fill in later) */
	NULL,			     /* task yield buffer ptr (not used here) */
	0			     /* task yield buffer size (not used here) */
};

static OSMesgQueue rspMessageQ;
static OSMesg rspMessageBuf;
static OSMesgQueue rdpMessageQ;
static OSMesg rdpMessageBuf;

void init_task( void )
{
    osCreateMesgQueue( &rspMessageQ, &rspMessageBuf, 1 );
    osSetEventMesg( OS_EVENT_SP, &rspMessageQ, NULL );

    osCreateMesgQueue( &rdpMessageQ, &rdpMessageBuf, 1 );
    osSetEventMesg( OS_EVENT_DP, &rdpMessageQ, NULL );
}

void start_task( Gfx *glist_ptr, Dynamic *dynamic_ptr )
{
    OSTask *task_ptr;

    task_ptr = &task_list;
    task_ptr->t.ucode_boot = (u64 *) rspbootTextStart;
    task_ptr->t.ucode_boot_size = (u32) rspbootTextEnd - (u32) rspbootTextStart;

    task_ptr->t.ucode = (u64 *) gspS2DEX2_fifoTextStart;
    task_ptr->t.ucode_data = (u64 *) gspS2DEX2_fifoDataStart;
    task_ptr->t.output_buff_size = (u64 *)((int)rdp_output +
				   (int)(RDP_OUTPUT_LEN*sizeof(u64)));

    /*
     * initial display list: 
     */
    task_ptr->t.data_ptr = (u64 *) dynamic_ptr->glist;
    task_ptr->t.data_size = (u32)((glist_ptr - dynamic_ptr->glist) * sizeof(Gfx));

    /*
     * Write back dirty cache lines that need to be read by the RCP.
     */
    osWritebackDCache(dynamic_ptr, sizeof( Dynamic ) );

    /*
     * start up the RSP task
     */
    osSpTaskStart( task_ptr );

    /*
     * wait for SP,DP completion 
     */
    (void)osRecvMesg( &rspMessageQ, NULL, OS_MESG_BLOCK );
    (void)osRecvMesg( &rdpMessageQ, NULL, OS_MESG_BLOCK );
}



