/*======================================================================*/
/*		NuSYS							*/
/*		nuaustlmgr.c						*/
/*									*/
/*		Copyright (C) 1997, NINTENDO Co,Ltd.			*/
/*									*/
/*======================================================================*/
/* $Id: nuaustlmgr.c,v 1.3 1999/04/30 13:54:40 ohki Exp $		*/
/*======================================================================*/
#include <nusys.h>
#include <nualstl.h>

u8			nuAuTaskStop = 1;
u8			nuAuPreNMI = 0;
NUAuPreNMIFunc		nuAuPreNMIFunc = NULL;

NUScTask		nuAuTask;
OSMesgQueue		nuAuMesgQ;
OSMesgQueue 		nuAuRtnMesgQ;
NUScClient		nuAuClient;
static OSMesg		nuAuMesgBuf[NU_AU_MESG_MAX];
static OSMesg		nuAuRtnMesgBuf;

static void nuAuStlSchedInstall(void);
static void nuAuStlSchedWaitFrame(void);
static void nuAuStlSchedDoTask(musTask *task);

musSched nuAuStlSchedFunc = {
    nuAuStlSchedInstall, nuAuStlSchedWaitFrame, nuAuStlSchedDoTask
};

/*----------------------------------------------------------------------*/
/*	nuAuStlMgrInit -Initialize the Audio Manager.			*/
/*	IN:	config	SoundTools configuration structure			*/
/*	RET:	size of heap area used					*/
/*----------------------------------------------------------------------*/
s32 nuAuStlMgrInit(musConfig* config)
{
    nuAuTaskStop = NU_AU_TASK_RUN;	/* Enable task execution */
    
    nuAuTask.msg			= 0;
    nuAuTask.msgQ			= &nuAuRtnMesgQ;
    nuAuTask.list.t.type		= M_AUDTASK;
    nuAuTask.list.t.ucode_boot		= (u64*)rspbootTextStart;
    nuAuTask.list.t.ucode_boot_size	=
	((int) rspbootTextEnd - (int) rspbootTextStart);
    nuAuTask.list.t.ucode_data_size	= SP_UCODE_DATA_SIZE;
    nuAuTask.list.t.dram_stack		= (u64*) NULL;
    nuAuTask.list.t.dram_stack_size	= 0;
    nuAuTask.list.t.output_buff		= (u64*) NULL;
    nuAuTask.list.t.output_buff_size	= 0;
    nuAuTask.list.t.yield_data_ptr	= NULL;
    nuAuTask.list.t.yield_data_size	= 0;

    MusSetScheduler(&nuAuStlSchedFunc);
    return(MusInitialize(config));
}

/*----------------------------------------------------------------------*/
/*	nuAuStlSchedInstall - Initializes the Scheduler			*/
/*	Register with the Scheduler the message queue that receives		*/
/*	 retrace and PRE NMI messages.					*/
/*	IN:	nothing							*/
/*	RET:	nothing							*/
/*----------------------------------------------------------------------*/
static void nuAuStlSchedInstall(void)
{
    osCreateMesgQueue(&nuAuMesgQ, nuAuMesgBuf, NU_AU_MESG_MAX);
    osCreateMesgQueue(&nuAuRtnMesgQ, &nuAuRtnMesgBuf, 1);
    nuScAddClient(&nuAuClient, &nuAuMesgQ, NU_SC_RETRACE_MSG | NU_SC_PRENMI_MSG);
}

/*----------------------------------------------------------------------*/
/*	nuAuStlSchedWaitFrame - Waits for message from the Scheduler	*/
/*	IN:	nothing							*/
/*	RET:	nothing							*/
/*----------------------------------------------------------------------*/
static void nuAuStlSchedWaitFrame(void)
{
    NUScMsg*	mesg_type;
    while(1){
	osRecvMesg(&nuAuMesgQ, (OSMesg*)&mesg_type, OS_MESG_BLOCK);
	switch(*mesg_type){
	case NU_SC_RETRACE_MSG:
	    return;
	case NU_SC_PRENMI_MSG:
	    if(nuAuPreNMIFunc){
		(*nuAuPreNMIFunc)(NU_SC_PRENMI_MSG, nuAuPreNMI);
	    }
	    nuAuPreNMI++;
	    break;
	default:
	    break;
	}
    }
}

/*----------------------------------------------------------------------*/
/*	nuAuStlSchedDoTask - Starts task				*/
/*	Starts audio tasks.  If a callback function is registered, calls that function	*/
/*	 continuously after a 	PRE NMI.					*/
/*	IN:	nothing							*/
/*	RET:	nothing							*/
/*----------------------------------------------------------------------*/
static void nuAuStlSchedDoTask(musTask *task)
{
    if(nuAuTaskStop == NU_AU_TASK_RUN){
	nuAuTask.list.t.data_ptr  = task->data; 
	nuAuTask.list.t.data_size = task->data_size;

	nuAuTask.list.t.ucode		= (u64*)task->ucode;
	nuAuTask.list.t.ucode_data	= (u64*)task->ucode_data;
	osSendMesg(&nusched.audioRequestMQ, (OSMesg*)&nuAuTask, OS_MESG_BLOCK);
	osRecvMesg(&nuAuRtnMesgQ, NULL, OS_MESG_BLOCK);
    }
    
    /* If a PRE NMI message occurs, this function is subsequently called continuously. */
    if((u32)nuAuPreNMIFunc && nuAuPreNMI){
	(*nuAuPreNMIFunc)(NU_SC_RETRACE_MSG, nuAuPreNMI);
	nuAuPreNMI++;
    }
}
