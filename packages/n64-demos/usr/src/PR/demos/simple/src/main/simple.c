/**********************************************************************
 * simple.c
 *
 * simple - Basic example of an Ultra64 application including:
 *     Multiprocessing between host and RSP
 *     Graphics
 *     Multiple controllers for user input
 *     Audio, both music and sound effects
 *
 * simple works by having several threads of various priority.
 *     scheduler thread. The highest priority thread of the game. Its 
 *          function is to see that audio and graphics tasks get built
 *          and executed in a timely fashion.
 *     audio thread. The second highest thread. Creates the audio task
 *          lists to be executed by the audio microcode to synthesize 
 *          the audio. Audio should run at a higher priority than 
 *          graphics, since if a frame of audio gets dropped, it will
 *          cause a serious click, but a frame of graphics can always 
 *          redraw the last frame.
 *     game thread. Loops waiting for messages from the scheduler. 
 *          Upon receiving retrace messages, will branch to the graphics 
 *          routine and the controller read routine.
 *     idle thread. Starts out as the init thread, but becomes the idle
 *          thread. Every game needs one of these, don't get rid of it.
 *
 * Copyright 1995, Silicon Graphics, Inc.
 * All Rights Reserved.
 *
 * This is UNPUBLISHED PROPRIETARY SOURCE CODE of Silicon Graphics,
 * Inc.; the contents of this file may not be disclosed to third
 * parties, copied or duplicated in any form, in whole or in part,
 * without the prior written permission of Silicon Graphics, Inc.
 *
 * RESTRICTED RIGHTS LEGEND:
 * Use, duplication or disclosure by the Government is subject to
 * restrictions as set forth in subdivision (c)(1)(ii) of the Rights
 * in Technical Data and Computer Software clause at DFARS
 * 252.227-7013, and/or in similar or successor clauses in the FAR,
 * DOD or NASA FAR Supplement. Unpublished - rights reserved under the
 * Copyright Laws of the United States.
 **********************************************************************/

/*---------------------------------------------------------------------*
        Copyright (C) 1998 Nintendo. (Originated by SGI)
        
        $RCSfile: simple.c,v $
        $Revision: 1.1.1.1 $
        $Date: 2002/05/02 03:27:21 $
 *---------------------------------------------------------------------*/

#include <ultra64.h>
#include <sched.h>
#include "simple.h"
#include "audio.h"
#include "gfx.h"


/**** threads used by this file ****/
static OSThread gameThread;
static OSThread initThread;

/**** Stack for boot code.  Space can be reused after 1st thread starts ****/
u64    bootStack[STACKSIZEBYTES/sizeof(u64)];

/**** Stacks for the threads, divide by 8 which is the size of a u64 ****/
static u64      gameThreadStack[STACKSIZEBYTES/sizeof(u64)];
static u64      initThreadStack[STACKSIZEBYTES/sizeof(u64)];

/**** function prototypes for private functions in this file ****/
static void     gameproc(void *);
static void     initproc(char *);
static void     initGame(void);

/**** message queues and message buffers used by this app ****/
static OSMesg           PiMessages[DMA_QUEUE_SIZE];
static OSMesgQueue      PiMessageQ;
static OSMesgQueue      dmaMessageQ;
static OSMesg           dmaMessageBuf;

/**** Graphics variables used in this file ****/
OSMesgQueue     gfxFrameMsgQ;
OSMesg          gfxFrameMsgBuf[MAX_MESGS];
Gfx             *glistp;
GFXInfo         gInfo[2];

/**** Scheduler globals ****/
OSSched         sc;
OSMesgQueue	*sched_cmdQ;
u64             scheduleStack[OS_SC_STACKSIZE/8];
OSScClient      gfxClient;

/**** Controller globals ****/
extern u8       validcontrollers;

#ifndef _FINALROM
OSTime  lastTime;
#endif



/**********************************************************************
 *
 * boot is the equivalent of main(). This is where your application will
 * start its execution. This thread should  be kept very simple, and 
 * should start another thread that will ultimately become the idle 
 * thread. Execution will never return from the first call of 
 * osStartThread. In this example, we parse commmand line arguments 
 * passed from gload. In a real application you won't be able to do this.
 *
 **********************************************************************/
OSPiHandle	*handler;

void boot(void *arg)
{
    __osInitialize_common();


    handler = osCartRomInit();
    
    osCreateThread(&initThread, 1, (void(*)(void *))initproc, arg,
                  (void *)(initThreadStack+(STACKSIZEBYTES/sizeof(u64))), 
		   (OSPri)INIT_PRIORITY);

    osStartThread(&initThread);
}

/**********************************************************************
 *
 * initproc sets up the PI manager. It then creates
 * and starts our application's game thread. After returning from that,
 * it becomes the lowest priority thread, and functions as the idle thread.
 *
 **********************************************************************/
static void initproc(char *argv) 
{
    /**** Start PI Mgr for access to cartridge ****/
    osCreatePiManager((OSPri) OS_PRIORITY_PIMGR, &PiMessageQ, PiMessages,
                        DMA_QUEUE_SIZE);

    /**** Create the game thread and start it up ****/
    osCreateThread(&gameThread, 6, gameproc, argv, gameThreadStack + 
		   (STACKSIZEBYTES/sizeof(u64)), (OSPri)GAME_PRIORITY);

    osStartThread(&gameThread);

    /**** Set the thread to be the idle thread ****/
    osSetThreadPri(0, 0);
    for(;;);
}

/**********************************************************************
 *
 * A continual loop, primarily used for servicing the starts of graphic 
 * tasks and controller reads. Audio is serviced by the audio thread. 
 * You receive several message on the same queue, they are:
 *    OS_SC_RETRACE_MSG: this comes from the scheduler every retrace. 
 *         If you don't already have 2 graphics tasks either pending or 
 *         processing, start creating another.
 *    OS_SC_DONE_MSG: sent by the scheduler when a graphics task has 
 *         completed. 
 *    SIMPLE_CONTROLLER_MSG: sent by the controller code indicating that the 
 *         controller read has completed, so now you can use that info 
 *         in game play. 
 *    OS_SC_PRE_NMI_MSG: sent to indicate the reset button has been 
 *         pressed, and that the game will reboot momentarily. Might 
 *         want to fade or say goodbye.
 *
 **********************************************************************/
static void gameproc(void *argv)
{
    u32         drawbuffer = 0;
    u32         pendingGFX = 0;
    u32         cntrlReadInProg = 0;
    GFXMsg      *msg = NULL;
#ifndef _FINALROM
    u32         doneTask = 0;
#endif

    initGame();

    while (1) 
    {
        (void) osRecvMesg(&gfxFrameMsgQ, (OSMesg *)&msg, OS_MESG_BLOCK);

        switch (msg->gen.type) 
        {
            case (OS_SC_RETRACE_MSG):

                /**** Create a new gfx task unless we already have 2  ****/                 
                if (pendingGFX < 2) 
                {
                    createGfxTask(&gInfo[drawbuffer]);
                    pendingGFX++;
                    drawbuffer ^= 1; /* switch the drawbuffer */
                }

                /* request latest controller information (ie poll) */
                if (validcontrollers && !cntrlReadInProg)
                {
                    cntrlReadInProg = 1;
                    osContStartReadData(&gfxFrameMsgQ);
                }
                break;

            case (OS_SC_DONE_MSG):
#ifndef _FINALROM
		lastTime = gInfo[doneTask].task.totalTime;
		doneTask ^= 1;
#endif
                pendingGFX--;        /* decrement number of pending tasks */
                break;
                
            case SIMPLE_CONTROLLER_MSG:
                UpdateController();
                cntrlReadInProg = 0;
                break;
                
            case (OS_SC_PRE_NMI_MSG): /* stop creation of graphics tasks */
                pendingGFX += 2;
                break;
        }
    }
}

/**********************************************************************
 *
 * A simple utility routine for copying data from rom to ram
 * Note that the routine assumes that it has exclusive use of
 * the dmaMessageQ.
 *
 *********************************************************************/
void romCopy(const char *src, const char *dest, const int len)
{
    OSIoMesg dmaIoMesgBuf;
    OSMesg dummyMesg;
    
    /*
     * Always invalidate cache before dma'ing data into the buffer.
     * This is to prevent a flush of the cache in the future from 
     * potentially trashing some data that has just been dma'ed in.
     * Since you don't care if old data makes it from cache out to 
     * memory, you can use the cheaper osInvalDCache() instead of one
     * of the writeback commands
     */
    osInvalDCache((void *)dest, (s32) len);

    dmaIoMesgBuf.hdr.pri      = OS_MESG_PRI_NORMAL;
    dmaIoMesgBuf.hdr.retQueue = &dmaMessageQ;
    dmaIoMesgBuf.dramAddr     = (void*)dest;
    dmaIoMesgBuf.devAddr      = (u32)src;
    dmaIoMesgBuf.size         = (u32)len;

    osEPiStartDma(handler, &dmaIoMesgBuf, OS_READ);
    (void) osRecvMesg(&dmaMessageQ, &dummyMesg, OS_MESG_BLOCK);
}

/**********************************************************************
 *
 * initGame sets up the message queues used, and starts the scheduler.
 * After that call routines to init the graphics, init the controllers,
 * and init the audio.
 *
 *********************************************************************/
static void initGame(void)
{
    /**** set up a needed message q's ****/
    osCreateMesgQueue(&dmaMessageQ, &dmaMessageBuf, 1);
    osCreateMesgQueue(&gfxFrameMsgQ, gfxFrameMsgBuf, MAX_MESGS);

    /**** Initialize the RCP task scheduler ****/
    osCreateScheduler(&sc, (void *)(scheduleStack + OS_SC_STACKSIZE/8),
                      SCHEDULER_PRIORITY, OS_VI_NTSC_LAN1, NUM_FIELDS);

    /**** Add ourselves to the scheduler to receive retrace messages ****/
    osScAddClient(&sc, &gfxClient, &gfxFrameMsgQ);  

    sched_cmdQ = osScGetCmdQ(&sc);

    /**** Call the initialization routines ****/
    initGFX(); 
    initCntrl();
    initAudio();
}

