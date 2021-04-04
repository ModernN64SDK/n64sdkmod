
/*************************************************************

  test_main.c : Nintendo 64 Music Tools Library Sample
  (c) Copyright 1998, Software Creations (Holdings) Ltd.

  Version 3.14

  TESTMENU demo main source file. Uses a simple menu to 
  illustrate the use of various music library functions.

  Once executed use the Dpad to navigate up-down and change 
  setting with left-right, for settings with a large range
  use the shoulder buttons for larger changes. The A button
  executes the menu command. The B button will stop all
  currently running channels.

**************************************************************/

/* include system header files */
#ifndef F3DEX_GBI
#define F3DEX_GBI
#endif

#include <ultra64.h>
#include <sched.h>
#include <libmus.h>

/* include current header file */
#include "test_rom.h"

/* prototypes */
void    StartThread	(void *ignored); 
void    GameThread	(void *ignored);
void    MainLoop	   (void);
int	  InitMusicPlayer	(void);

/* stack sizes */
#define BOOTSTACK_SIZE      (STACKSIZE)
#define MAINSTACK_SIZE      (STACKSIZE)

/* size of the DMA queue */
#define DMA_QUEUE_SIZE      200

/* number of serial messages to support */
#define SERIAL_MESGS       4

/* all geometry modes */
#define GEOM_ALL    G_SHADE|G_SHADING_SMOOTH|G_CULL_BOTH|G_FOG|G_LIGHTING|G_TEXTURE_GEN|G_TEXTURE_GEN_LINEAR|G_LOD

/* load projection and model matrices */
#define LOAD_PROJ   (G_MTX_PROJECTION|G_MTX_LOAD|G_MTX_NOPUSH)
#define LOAD_MODEL  (G_MTX_MODELVIEW|G_MTX_LOAD|G_MTX_NOPUSH)

/* macros - background colour for screen (all three components) */
#define BGCOL       0

/* graphics task message structure */
typedef union 
{    
   short    type;
   OSScMsg  app;    
} message_t;

/* graphics information structure */
typedef struct 
{
   OSScTask       task;
   message_t      mess;
   unsigned short *screen;
} info_t;

/* scheduler structure */
OSSched		sc;

/* threads */
OSThread	gameThread;
OSThread	mainThread;

/* thread stacks */
u64		bootStack       [BOOTSTACK_SIZE/8];
u64		mainThreadStack [MAINSTACK_SIZE/8];
u64		scheduleStack   [OS_SC_STACKSIZE/8];

/* workspace - message queues */
OSMesgQueue     pi_queue;
OSMesg          pi_messages	[DMA_QUEUE_SIZE];
OSMesgQueue     gfx_queue;
OSMesg          gfx_messages	[MAX_MESGS];
OSMesgQueue     dma_queue;
OSMesg          dma_messages	[MAX_MESGS];
OSMesgQueue     serial_queue;
OSMesg          serial_messages	[SERIAL_MESGS];

/* controller pad workspace */
OSContPad	controllerdata  [MAXCONTROLLERS];
OSContStatus	statusdata      [MAXCONTROLLERS];

/* double buffered graphic information */
info_t      info[2];

/* Entry point */
void boot(void *ignored)
{
   osInitialize();

   osCreateThread(&mainThread, 1, StartThread, NULL, (void *)(mainThreadStack+MAINSTACK_SIZE/8), (OSPri)MAIN_PRIORITY);
   osStartThread(&mainThread);
}

/* base thread */
void StartThread(void *ignored) 
{
   /* start PI manager for ROM access */
   osCreatePiManager((OSPri) OS_PRIORITY_PIMGR, &pi_queue, pi_messages, DMA_QUEUE_SIZE);
   
   /* create game thread and start it */
   osCreateThread(&gameThread, 6, GameThread, NULL, bootStack+BOOTSTACK_SIZE/8, (OSPri)GAME_PRIORITY);    
   osStartThread(&gameThread);   

   /* become idle thread */
   osSetThreadPri(0, 0);	
   for(;;);
}

/* application thread */
void GameThread(void *ignored)
{
   OSScClient	client;
   u8 pattern;
   int mode;

   /* create message queues */    
   osCreateMesgQueue(&dma_queue, dma_messages, MAX_MESGS);
   osCreateMesgQueue(&serial_queue, serial_messages, SERIAL_MESGS);    
   /* map relevant events to the queues */
   osSetEventMesg(OS_EVENT_SI, &serial_queue, (OSMesg)1);

   /* initialize default scheduler using "osTvType" */
   if (osTvType==0)
      mode = OS_VI_PAL_LAN1;      /* --- PROGRAMMING CAUTION --- */
   else if (osTvType==1)          /* Applications must not use "ovTvType" to switch the TV   */
      mode = OS_VI_NTSC_LAN1;     /* system from NTSC to PAL or PAL to NTSC. A Game Pak must */
   else                           /* not be compatible with both NTSC and PAL formats.       */
      mode = OS_VI_MPAL_LAN1;     
   osCreateScheduler(&sc, (void *)(scheduleStack+OS_SC_STACKSIZE/8), SCHEDULER_PRIORITY, mode, 1);

   info[0].mess.type = OS_SC_DONE_MSG;
   info[0].screen = cfb_A;
   info[1].mess.type = OS_SC_DONE_MSG;
   info[1].screen = cfb_B;
    
   osCreateMesgQueue(&gfx_queue, gfx_messages, MAX_MESGS);
   osScAddClient(&sc, &client, &gfx_queue);
   osContInit(&serial_queue, &pattern, &statusdata[0]);
   
   /* call main loop (never returns from this) */
   MainLoop();
}

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
  [GLOBAL FUNCTION]
  MainLoop()

  [Explanation]
  Test programs main loop.

  [Return value]
  NONE (never returns)
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/

/* draw lists */
u64         glist[2][10000];

/* scheduler workspace */

/* full screen viewports */
Vp          vp0 = 
{
        (SCREEN_XSIZE/2)*4, (SCREEN_YSIZE/2)*4, G_MAXZ/2, 0,  /* scale */
        (SCREEN_XSIZE/2)*4, (SCREEN_YSIZE/2)*4, G_MAXZ/2, 0,  /* translate */
};

/* single shaded triangle */
Vtx         tri[]=
{
  { -64, 64,-5,0,0,0,0,0xff,0,0xff},
  {  64, 64,-5,0,0,0,0,0,0,0xff},
  {  64,-64,-5,0,0,0,0xff,0,0,0xff},
};

/* vertex buffers */
Vtx vtx_buffer1[10000];
Vtx vtx_buffer2[10000];

/* current graphic pointers */
Vtx *vertp;
u64 *glistp, *glist_start;

/* amount of audio heap used */
int heap_used;


void MainLoop(void)
{
   int i, frame, drawing_flag;
   float angle;
   u32 work;
   OSScTask    *task;    
   Mtx projection_2d, mtxwork, model_2d[2];
   Mtx none;
   message_t *end_msg;

   frame = drawing_flag = 0;
   angle = 0.0;

   /* initialise for 2D */
   guOrtho(&projection_2d, 0.0, 320.0, 240.0, 0.0, -200.0, 200.0, 1.0);
   guTranslate(&none, 0.0, 0.0, 0.0);

   /* initialise menu processing */
   MenuInit();

   /* download song data to RAM */
   DownloadSongs();
   /* install the music player */
   heap_used = InitMusicPlayer();

   /* loop for ever */
   while (1)
   {
      MenuProcessController();

      frame ^= 1;
      /* rotating triangle */
      guTranslate(&model_2d[frame], 160.0, 120.0, 0.0);
      guRotate(&mtxwork, angle, 0.0F,0.0F,1.0F);    
      guMtxCatL(&mtxwork, &model_2d[frame], &model_2d[frame]);
      angle += 1.0;
      if (angle>=360.0)
         angle-=360.0; 
      glist_start = glistp = &glist[frame][0];

      /* special video tackle */
      osViSetSpecialFeatures(OS_VI_DITHER_FILTER_ON);
      osViSetSpecialFeatures(OS_VI_GAMMA_OFF | OS_VI_GAMMA_DITHER_OFF);
      /* setup segments */
      gSPSegment(glistp++, 0, 0);
      gSPSegment(glistp++,  CFB_SEG, OS_K0_TO_PHYSICAL(cfb_A) );
      /* set defaults */
      gSPViewport(glistp++, &vp0);
      gSPClearGeometryMode (glistp++,GEOM_ALL);
      gSPTexture           (glistp++,0, 0, 0, 0, G_OFF);
      gSPSetGeometryMode(glistp++,G_SHADE | G_SHADING_SMOOTH);

      gDPSetCycleType      (glistp++,G_CYC_1CYCLE);
      gDPPipelineMode      (glistp++,G_PM_1PRIMITIVE);
      gDPSetScissor        (glistp++,G_SC_NON_INTERLACE, 0, 0, 320, 240);
      gDPSetTextureLOD     (glistp++,G_TL_TILE);
      gDPSetTextureLUT     (glistp++,G_TT_NONE);
      gDPSetTextureDetail  (glistp++,G_TD_CLAMP);
      gDPSetTexturePersp   (glistp++,G_TP_PERSP);
      gDPSetTextureFilter  (glistp++,G_TF_BILERP);
      gDPSetTextureConvert (glistp++,G_TC_FILT);
      gDPSetCombineMode    (glistp++,G_CC_SHADE, G_CC_SHADE);
      gDPSetCombineKey     (glistp++,G_CK_NONE);
      gDPSetAlphaCompare   (glistp++,G_AC_NONE);
      gDPSetColorDither    (glistp++,G_CD_DISABLE);

      /* set colour buffer address */
      gDPSetColorImage(glistp++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 320, osVirtualToPhysical(frame ? cfb_B : cfb_A));
      gDPPipeSync          (glistp++);

      /* clear screen */
      work = GPACK_RGBA5551(BGCOL, BGCOL, BGCOL,1);
      gDPSetCycleType      (glistp++,G_CYC_FILL);
      gDPSetFillColor(glistp++, (work<<16) | work);
      gDPFillRectangle     (glistp++,0, 0, 320-1, 240-1);
      gDPPipeSync(glistp++);

      /* now setup to draw something */
      gSPMatrix(glistp++, OS_K0_TO_PHYSICAL(&projection_2d), LOAD_PROJ);
      gSPMatrix(glistp++, OS_K0_TO_PHYSICAL(&model_2d[frame]), LOAD_MODEL);
      gDPSetCycleType(glistp++, G_CYC_1CYCLE);
      gDPSetRenderMode(glistp++, G_RM_AA_OPA_SURF, G_RM_AA_OPA_SURF2);
 
      /* draw simple triangle */
      gDPPipeSync(glistp++);      
      gSPVertex(glistp++,  OS_K0_TO_PHYSICAL(&tri[0]),3,0);
      gSP1Triangle(glistp++, 0, 1, 2, 0);
      gDPPipeSync(glistp++);

      /* setup for drawing text */
      gSPTexture(glistp++,0xffff,0xffff,0,G_TX_RENDERTILE,G_ON);
      gDPSetCombineMode(glistp++, G_CC_DECALRGBA, G_CC_DECALRGBA);
      gDPSetRenderMode(glistp++, G_RM_TEX_EDGE, G_RM_TEX_EDGE2);
      gSPMatrix(glistp++, OS_K0_TO_PHYSICAL(&none), LOAD_MODEL);
      gDPSetTextureFilter(glistp++,G_TF_POINT);

      /* select correct vertex buffer */
      if (frame)
         vertp=vtx_buffer1;
      else
         vertp=vtx_buffer2;

      /* call menu */
      MenuProcess();

      /* finish off draw list */
      gDPFullSync(glistp++);
      gSPEndDisplayList(glistp++);

      /* make sure previous draw list has been finished */
      if (drawing_flag)
      {
         do
         {
            osRecvMesg(&gfx_queue, (OSMesg *)&end_msg, OS_MESG_BLOCK);
         } while (end_msg->type!=OS_SC_DONE_MSG);
      }
      drawing_flag = 1;

      /* create graphic task */
      task = &info[frame].task;
      task->list.t.data_ptr = (u64 *) glist_start;
      task->list.t.data_size = glistp-glist_start;
      task->list.t.type = M_GFXTASK;
      task->list.t.flags = 0x0;
      task->list.t.ucode_boot = (u64 *)rspbootTextStart;
      task->list.t.ucode_boot_size = ((int) rspbootTextEnd - (int) rspbootTextStart);
      task->list.t.ucode = (u64 *) gspF3DEX_fifoTextStart;
      task->list.t.ucode_data = (u64 *) gspF3DEX_fifoDataStart;
      task->list.t.ucode_size      = 4096;
      task->list.t.ucode_data_size = 2048;
      task->list.t.dram_stack = NULL;
      task->list.t.dram_stack_size = 0;
      task->list.t.output_buff = fifo_buffer;
      task->list.t.output_buff_size = &fifo_buffer[FIFO_DATA_SIZE/sizeof(u64)];
      task->list.t.yield_data_ptr = yield_buffer;
      task->list.t.yield_data_size = OS_YIELD_DATA_SIZE;
      task->next     = 0;
      task->flags = OS_SC_NEEDS_RSP | OS_SC_NEEDS_RDP | OS_SC_LAST_TASK | OS_SC_SWAPBUFFER;
      task->msgQ     = &gfx_queue;
      task->msg      = (OSMesg)&info[frame].mess;
      task->framebuffer = (void *)info[frame].screen;

      /* start graphic task */
      osWritebackDCacheAll();
      osSendMesg(osScGetCmdQ(&sc), (OSMesg) task, OS_MESG_BLOCK);
   }
}

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
  [EXTERNAL FUNCTION]
  DmaRomToRam(rom, ram, len)

  [Parameters]
  rom		   address of ROM source
  ram		   address of RAM destination 
  len		   number of bytes to transfer

  [Explanation]
  Download an area of ROM to RAM. Note this function limits the size of any DMA
  generated to 16k so as to cause clashes with audio DMAs.

  [Return value]
  NONE
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/

/* avoid DMA clashes */
#define DMA_LEN 16384
                   
void DmaRomToRam(void *rom, void *ram, int len)
{
   OSIoMesg io_mesg;
   OSMesg dummyMesg;
   int length;
   u32 src;
   unsigned char *dest;

   src = (u32)rom;
   dest = ram;
    
   while (len)
   {
      if (len>DMA_LEN)
         length = DMA_LEN;
      else
         length = len;
      osInvalDCache(dest, length);
      osPiStartDma(&io_mesg, OS_MESG_PRI_NORMAL, OS_READ, src, dest, length, &dma_queue);
      osRecvMesg(&dma_queue, &dummyMesg, OS_MESG_BLOCK);
      src+=length;
      dest+=length;
      len-=length;
   }
}

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
  [GLOBAL FUNCTION]
  DownloadSongs()

  [Explanation]
  Download songs to RAM.

  [Return value]
  NONE
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/

/* tune buffers */
int tune_buffer1[6000/sizeof(int)]__attribute__((aligned(16)));
int tune_buffer2[6000/sizeof(int)]__attribute__((aligned(16)));
int tune_buffer3[6000/sizeof(int)]__attribute__((aligned(16)));
int tune_buffer4[6000/sizeof(int)]__attribute__((aligned(16)));
int sfx_buffer1[6000/sizeof(int)]__attribute__((aligned(16)));

/* ROM addresses */
#define MUSIC_START1	_tune1SegmentRomStart
#define MUSIC_END1	_tune1SegmentRomEnd
#define MUSIC_START2	_tune2SegmentRomStart
#define MUSIC_END2	_tune2SegmentRomEnd
#define SFX_START1	_sfxSegmentRomStart
#define SFX_END1		_sfxSegmentRomEnd

extern char MUSIC_START1[];
extern char MUSIC_END1[];
extern char MUSIC_START2[];
extern char MUSIC_END2[];
extern char SFX_START1[];
extern char SFX_END1[];

void DownloadSongs(void)
{
   DmaRomToRam(MUSIC_START1, (unsigned char *)tune_buffer1, MUSIC_END1-MUSIC_START1);
   DmaRomToRam(MUSIC_START2, (unsigned char *)tune_buffer2, MUSIC_END2-MUSIC_START2);
   DmaRomToRam(MUSIC_START1, (unsigned char *)tune_buffer3, MUSIC_END1-MUSIC_START1);
   DmaRomToRam(MUSIC_START2, (unsigned char *)tune_buffer4, MUSIC_END2-MUSIC_START2);
   DmaRomToRam(SFX_START1, (unsigned char *)sfx_buffer1, SFX_END1-SFX_START1);
}

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
  [GLOBAL FUNCTION]
  InitMusicPlayer()

  [Explanation]
  Initialise the music player for the test program.

  [Return value]
  amount of the audio heap used by the music player
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/

/* ROM addresses */
#define WBANK_START	_wbankSegmentRomStart
#define PBANK_START 	_pbankSegmentRomStart
#define PBANK_END	   _pbankSegmentRomEnd

extern char WBANK_START[];
extern char PBANK_START[];
extern char PBANK_END[];

/* size of permanent RAM buffers */
#define AUDIO_HEAP_SIZE	300000
#define PBANK_SIZE	   5000

/* permanent RAM buffers */
unsigned char	audio_memory	[AUDIO_HEAP_SIZE];
int		pointer_buf	      [PBANK_SIZE/sizeof(int)]__attribute__((aligned(16)));


int InitMusicPlayer(void)
{
   musConfig init;

   /* download pointer bank to RAM */
   DmaRomToRam(PBANK_START, (char *)pointer_buf, PBANK_END-PBANK_START);

   /* setup configuration structure */
   init.control_flag	= 0;
   init.channels		= 24;
   init.sched		= &sc;
   init.thread_priority	= 12;
   init.heap		= audio_memory;
   init.heap_length	= AUDIO_HEAP_SIZE;
   init.fifo_length	= 64;
   init.ptr		= (char *)pointer_buf;
   init.wbk		= WBANK_START;
	init.default_fxbank = sfx_buffer1;
   init.syn_output_rate	= 44100;
   init.syn_updates	= 256;
   init.syn_rsp_cmds	= 4096;
   init.syn_retraceCount = 1;
   init.syn_num_dma_bufs = 36;
   init.syn_dma_buf_size = 0x1000;

   return (MusInitialize(&init));
}


/* end of file */
