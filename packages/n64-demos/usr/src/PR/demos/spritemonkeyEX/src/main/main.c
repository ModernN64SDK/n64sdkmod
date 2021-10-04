/*
  main.c
    Copyright (C) 1999, NINTENDO Co,Ltd., MONEGI CORPORATION.
    Jan. 28, 1999.
*/

#include <ultra64.h>
#include <PR/ramrom.h>
#include <assert.h>
#include <PR/gs2dex.h>

#include "Task.h"
#include "RcpRdp.h"
#include "FrameBuffer.h"
#include "Dma.h"

#include "Tile_Rect.h"
#include "Cont.h"

/* static */
extern u32 _staticSegmentRomStart[];
extern u32 _staticSegmentRomEnd[];
extern u32 _staticSegmentStart[];
/* dk */
extern u32 _dkSegmentRomStart[];
extern u32 _dkSegmentRomEnd[];
extern u32 _dkSegmentStart[];
extern u32 _dkSegmentEnd[];
/* dk7 */
extern u32 _dk7SegmentRomStart[];
extern u32 _dk7SegmentRomEnd[];
extern u32 _dk7SegmentStart[];
extern u32 _dk7SegmentEnd[];
/* road */
extern u32 _roadSegmentRomStart[];
extern u32 _roadSegmentRomEnd[];
extern u32 _roadSegmentStart[];
extern u32 _roadSegmentEnd[];

/* l2_tv */
extern u32 _l2_tvSegmentRomStart[];
extern u32 _l2_tvSegmentRomEnd[];
extern u32 _l2_tvSegmentStart[];
extern u32 _l2_tvSegmentEnd[];

extern void game_init( void );
extern Gfx *game_main( Gfx *glistp, Control *cont );


/*
 * Stacks for the threads as well as message queues for synchronization
 * This stack is ridiculously large, and could also be reclaimed once
 * the main thread is started.
 */
u64             bootStack[ STACKSIZE / sizeof(u64) ];

void boot(void);
static void mainproc(void *);

static void     idle(void *);

static OSThread idleThread;
static u64      idleThreadStack[ STACKSIZE / sizeof(u64) ];

static OSThread mainThread;
static u64      mainThreadStack[ STACKSIZE / sizeof(u64) ];

#ifdef	_DEBUG
static OSThread rmonThread;
static u64      rmonStack[ RMON_STACKSIZE / sizeof(u64) ];
#endif	/* _DEBUG */

/*
 * this number (the depth of the message queue) needs to be equal
 * * to the maximum number of possible overlapping PI requests.
 * * For this app, 1 or 2 is probably plenty, other apps might
 * * require a lot more.
 */
#define NUM_PI_MSGS     8

static OSMesg      PiMessages[ NUM_PI_MSGS ];
static OSMesgQueue PiMessageQ;

Dynamic dynamic;

/*
 * global variables
 */
static int debugflag = 0;
static int draw_buffer = 0;

void boot(void)
{
    /*
     * notice that you can't call osSyncPrintf() until you set
     * * up the rmon thread.
     */

    osInitialize();

    osCreateThread( &idleThread, 1, idle, (void *) 0,
                    idleThreadStack + STACKSIZE / sizeof(u64), 10 );

    osStartThread(&idleThread);
}

static void idle(void *arg)
{
    /*
     * Initialize video 
     */
    osCreateViManager( OS_PRIORITY_VIMGR );
#ifdef	USE_CFB32
    osViSetMode( &osViModeTable[OS_VI_NTSC_LAN2] );
#else	/* USE_CFB32 */
    osViSetMode( &osViModeTable[OS_VI_NTSC_LAN1] );
#endif	/* USE_CFB32 */
    osViSetSpecialFeatures( OS_VI_DITHER_FILTER_ON | OS_VI_DIVOT_OFF | 
			    OS_VI_GAMMA_ON);

    /*
     * Start PI Mgr for access to cartridge
     */
    osCreatePiManager( (OSPri) OS_PRIORITY_PIMGR, &PiMessageQ,
                       PiMessages, NUM_PI_MSGS);

#ifdef	_DEBUG
    /*
     * Start RMON for debugging & data xfer (make sure to start 
     * PI Mgr first)
     */
    osCreateThread( &rmonThread, 0, rmonMain, (void *) 0,
		    (void *) (rmonStack + RMON_STACKSIZE / 8),
		    (OSPri) OS_PRIORITY_RMON);
    osStartThread(&rmonThread);
#endif	/* _DEBUG */

    /*
     * Create main thread
     */
    osCreateThread( &mainThread, 3, mainproc, arg,
		    mainThreadStack + STACKSIZE / sizeof(u64), 10 );

    if (!debugflag)
        osStartThread(&mainThread);

    /*
     * Become the idle thread
     */
    osSetThreadPri(0, 0);

    for (;;) ;
}

static void mainproc(void *arg)
{
    Dynamic    *dynamicp;
    Gfx        *glistp;
    Control    cont;

    init_dma();
    init_task();
    init_framebuffer();

    read_rom( _staticSegmentStart,
              _staticSegmentRomStart, _staticSegmentRomEnd  );
    read_rom( _dkSegmentStart, _dkSegmentRomStart, _dkSegmentRomEnd  );
    read_rom( _dk7SegmentStart, _dk7SegmentRomStart, _dk7SegmentRomEnd  );
    read_rom( _roadSegmentStart, _roadSegmentRomStart,
              _roadSegmentRomEnd  );
    read_rom( _l2_tvSegmentStart, _l2_tvSegmentRomStart,
              _l2_tvSegmentRomEnd  );

    init_controlers( &cont );

    game_init();

    while (1)
    {
        read_controler( &cont );

        dynamicp = &dynamic;

        guOrtho( &dynamicp->projection,
                 -(float) SCREEN_WD / 2.0F, (float) SCREEN_WD / 2.0F,
                 -(float) SCREEN_HT / 2.0F, (float) SCREEN_HT / 2.0F,
                 1.0F, 10.0F, 1.0F );
        guRotate( &dynamicp->modeling, 0.0F, 0.0F, 0.0F, 1.0F );

        glistp = dynamicp->glist;

        /* rcp rdp & color frame buffer initialize */
        glistp = init_rcprdp( glistp, (char *)_staticSegmentStart, draw_buffer );
        glistp = clear_cfb( glistp );

        /* game main */
        glistp = game_main( glistp, &cont );

        gDPFullSync(glistp++);
        gSPEndDisplayList(glistp++);

        assert((glistp - dynamicp->glist) < GLIST_LEN);

        start_task( glistp, dynamicp );

        swap_framebuffer( draw_buffer );
        draw_buffer ^= 1;
    }
}

