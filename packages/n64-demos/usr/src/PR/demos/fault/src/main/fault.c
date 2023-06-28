/**************************************************************************
 *                                                                        *
 *               Copyright (C) 1995, Silicon Graphics, Inc.               *
 *                                                                        *
 *  These coded instructions, statements, and computer programs  contain  *
 *  unpublished  proprietary  information of Silicon Graphics, Inc., and  *
 *  are protected by Federal copyright law.  They  may  not be disclosed  *
 *  to  third  parties  or copied or duplicated in any form, in whole or  *
 *  in part, without the prior written consent of Silicon Graphics, Inc.  *
 *                                                                        *
 **************************************************************************/

/*---------------------------------------------------------------------*
        Copyright (C) 1998 Nintendo. (Originated by SGI)
        
        $RCSfile: fault.c,v $
        $Revision: 1.5 $
        $Date: 1998/09/25 21:48:19 $
 *---------------------------------------------------------------------*/


#include "ultra64.h"
#include "PR/os_internal.h"
#include "fault.h"


#define PRINTF		osSyncPrintf
#define MSG_FAULT	0x10


/*
 * Thread and stack structures
 */
char   bootStack[STACKSIZE];

OSThread idleThread;
char     idleThreadStack[STACKSIZE];

OSThread mainThread;
char     mainThreadStack[STACKSIZE];

OSThread faultThread;
char     faultThreadStack[STACKSIZE];

OSThread user1Thread;
char     user1ThreadStack[STACKSIZE];

OSThread user2Thread;
char     user2ThreadStack[STACKSIZE];

OSThread user3Thread;
char     user3ThreadStack[STACKSIZE];

/*
 * Message queues and message buffers used by this app 
 */
static OSMesg           PiMessages[DMA_QUEUE_SIZE];
static OSMesgQueue      PiMessageQ;

static OSMesgQueue      faultMsgQ;
static OSMesg           faultMsgBuf;

static OSMesgQueue      user2MsgQ;
static OSMesg           user2MsgBuf;

static OSMesgQueue      user3MsgQ;
static OSMesg           user3MsgBuf;

/*
 * Local variables and routines
 */
static int		debugger = 0;

static void		idleproc(char *);
static void		mainproc(char *);
static void		faultproc(char *);
static void		user1proc(char *);
static void		user2proc(char *);
static void		user3proc(char *);

/*
 * Extern declarations
 */
extern int		printFaultData(OSThread *);


/*
 * This program is an example of how to catch various CPU exceptions such
 * as TLB on read/write. This program uses osSyncPrintf for printing to 
 * the debug port.  
 * exceptions, stops all user threads, and dumps out the registers of the 
 * faulted thread.
 *
 * The program flow is as follows:
 * - Boot code initializes OS and starts idle thread
 * - Idle thread gains control and starts PI manager
 * - PI manager gains control and blocks to listen to PI requests
 * - Idle thread regains control, starts main thread, resets its
 *   priority to 0
 * - Main thread gains control and starts user thread #1
 * - User thread #1 gains control and causes an immediate fault
 * - Fault handling thread receives fault message, prints all regs
 *   of faulted thread, and returns to listen for other fault messages
 * - Main thread regains control and starts user thread #2
 * - User thread #2 gains control and blocks in listening for a message
 *   in its queue
 * - Main thread regains control and starts user thread #3
 * - User thread #3 gains control and blocks in listening for a message
 *   in its queue
 * - Main thread regains control and sends a message to user thread #2
 * - User thread #2 gains control and causes an immediate fault
 * - Fault handling thread receives fault message, prints all regs
 *   of faulted thread, and returns to listen for other fault messages
 * - Main thread regains control and sends a message to user thread #3
 * - User thread #3 gains control and causes an immediate fault
 * - Fault handling thread receives fault message, prints all regs
 *   of faulted thread, and returns to listen for other fault messages
 * - Main thread regains control and prints all the faulted threads
 * - And that's all folks!
 */

void boot(void)
{
    osInitialize();
    osInitialize_isv();
    osCreateThread(&idleThread, 1, (void(*)(void *))idleproc, (void *)0,
                   idleThreadStack+STACKSIZE, 8);
    osStartThread(&idleThread);
}


static void
idleproc(char *argv)		/* priority 8 */
{
    /*
     * Start PI Mgr for access to cartridge - start before the debugger
     */
    osCreatePiManager((OSPri) OS_PRIORITY_PIMGR, &PiMessageQ, PiMessages,
            DMA_QUEUE_SIZE);

    /*
     * The main thread's priority must be the same or lower than the original
     * idle's thread priority. This allows the idle thread to change its
     * priority to 0 before the main thread starts execution.
     */
    osCreateThread(&mainThread, 3, (void(*)(void *))mainproc, argv,
           mainThreadStack+STACKSIZE/8, (OSPri)7);
    osStartThread(&mainThread);

    osSetThreadPri(0, OS_PRIORITY_IDLE);
    for(;;);                                    /* idle thread */
}


static void 
mainproc(char *argv)			/* priority of 7 */
{
    int i;
    OSThread *curr, *last;

    PRINTF("\n=> mainproc...\n");

    /*
     * Create message queue and thread structures for fault handling thread
     */
    osCreateMesgQueue(&faultMsgQ, &faultMsgBuf, 1);
    osCreateThread(&faultThread, 5, (void(*)(void *))faultproc, argv,
           faultThreadStack+STACKSIZE/8, (OSPri)50);

    /*
     * Create message queue and thread structures for user thread #3 
     */
    osCreateMesgQueue(&user3MsgQ, &user3MsgBuf, 1);
    osCreateThread(&user3Thread, 8, (void(*)(void *))user3proc, argv,
           user3ThreadStack+STACKSIZE/8, (OSPri)14);

    /*
     * Create message queue and thread structures for user thread #2 
     */
    osCreateMesgQueue(&user2MsgQ, &user2MsgBuf, 1);
    osCreateThread(&user2Thread, 7, (void(*)(void *))user2proc, argv,
           user2ThreadStack+STACKSIZE/8, (OSPri)12);

    /*
     * Create thread structure for user thread #1 
     */
    osCreateThread(&user1Thread, 6, (void(*)(void *))user1proc, argv,
           user1ThreadStack+STACKSIZE/8, (OSPri)10);

    PRINTF("\n=> mainproc - starting fault thread...\n");
    osStartThread(&faultThread);

    PRINTF("\n=> mainproc - starting user1 thread...\n");
    /* this causes an immediate crash */
    osStartThread(&user1Thread);

    PRINTF("\n=> mainproc - starting user2 thread...\n");
    osStartThread(&user2Thread);		

    PRINTF("\n=> mainproc - starting user3 thread...\n");
    osStartThread(&user3Thread);		

    PRINTF("\n=> mainproc - sending msg to user2 thread...\n");
    /* this causes an immediate crash */
    (void) osSendMesg(&user2MsgQ, (OSMesg)0x02, OS_MESG_BLOCK);

    PRINTF("\n=> mainproc - sending msg to user3 thread...\n");
    /* this causes an immediate crash */
    (void) osSendMesg(&user3MsgQ, (OSMesg)0x03, OS_MESG_BLOCK);

    PRINTF("\n=> mainproc - printing out all faulted threads...\n");

    curr = last = (OSThread *)NULL;
    i = 1;
    while (1) {

        /* 
         * This routine returns the next faulted thread from the active
         * thread list. It uses the thread argument as the starting point of
         * the search: if NULL, starts from the beginning of the list.
         * The routine returns NULL if it can't find any faulted thread.
         */
        curr = __osGetNextFaultedThread(last);
        if (curr) {
            last = curr;
            PRINTF("\t%3d) Thread: addr=0x%08x, id=%2d, priority=%3d\n", 
		i, curr, curr->id, curr->priority);
            i++;
        }
        else
	    break;
    }

    PRINTF("\n=> THE END\n");
}


/*
 * Fault handler: simply waits for the fault message, gets the current 
 *  faulted thread, and prints out all the registers of this thread.
 */
static void
faultproc(char *argv)
{
    OSMesg msg;
    static OSThread *curr, *last;

    osSetEventMesg(OS_EVENT_FAULT, &faultMsgQ, (OSMesg)MSG_FAULT);

    last = (OSThread *)NULL;
    while (1) {
        PRINTF("\n=> faultproc - waiting for message...\n");
        (void) osRecvMesg(&faultMsgQ, (OSMesg *)&msg, OS_MESG_BLOCK);
        PRINTF("\n=> faultproc - got a fault message...\n");

        /* This routine returns the most recent faulted thread */
        curr = __osGetCurrFaultedThread();
        if (curr) {
            printFaultData(curr);
        }
    }
}


static void
user1proc(char *argv)
{
    long e1;

    PRINTF("\n=> user1proc - we're gonna crash!\n");
    e1 = *(long *)1;	/* TLB exception on load/instruction fetch */

}


static void
user2proc(char *argv)
{
    OSMesg msg;

    while (1) {
        PRINTF("\n=> user2proc - waiting for message...\n");
        (void) osRecvMesg(&user2MsgQ, (OSMesg *)&msg, OS_MESG_BLOCK);

        PRINTF("\n=> user2proc - we're gonna crash!\n");
        *(long *)2 = 2;	/* TLB exception on store */
    }
}


static void
user3proc(char *argv)
{
    OSMesg msg;
    u32 fpstat;
    float f0;

    /*
     * Fetch the current floating-point control/status register
     * Enable divide-by-zero exception for floating point
     */
    fpstat = __osGetFpcCsr();
    __osSetFpcCsr(fpstat|FPCSR_EZ);

    while (1) {
        PRINTF("\n=> user3proc - waiting for message...\n");
        (void) osRecvMesg(&user3MsgQ, (OSMesg *)&msg, OS_MESG_BLOCK);

        PRINTF("\n=> user3proc - we're gonna crash!\n");
        f0 = 1.0/0;	/* Floating-point exception: divide-by-zero */
    }
}


