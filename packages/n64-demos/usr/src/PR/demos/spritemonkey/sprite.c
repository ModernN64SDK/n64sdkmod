/**************************************************************************
 *                                                                        *
 *               Copyright (C) 1995, Silicon Graphics, Inc.               *
 *                                                                        *
 *  These coded instructions, statements, and computer programs  contain  *
 *  unpublished  proprietary  information of Silicon Graphics, Inc., and  *
 *  are protected by Federal copyright  law.  They  may not be disclosed  *
 *  to  third  parties  or copied or duplicated in any form, in whole or  *
 *  in part, without the prior written consent of Silicon Graphics, Inc.  *
 *                                                                        *
 *************************************************************************/

/*---------------------------------------------------------------------*
        Copyright (C) 1998 Nintendo. (Originated by SGI)
        
        $RCSfile: sprite.c,v $
        $Revision: 1.13 $
        $Date: 1999/04/21 07:43:29 $
 *---------------------------------------------------------------------*/

/*
 * File:	lines.c
 * Create Date:	Thu Jun 22 09:28:01 PDT 1995
 *
 *
 */

#include <ultra64.h>
#include <PR/ramrom.h>	/* needed for argument passing into the app */
#include <assert.h>

#include "sprite.h"
#include "static.h"
#include "controller.h"

int SCREEN_WD   = 640;
int SCREEN_HT   = 480;

int   TranslateHorizontal   =          0;
int   TranslateVertical     =          0;

int   ControllerMode        =          2;
int   ActiveController;

int   TextureStartS;
int   TextureStartT;
int   RectangleWidth;
int   RectangleHeight; 
int   TextureScaleX;
int   TextureScaleY;
int   TextureNumber;

int   ImageWidth;
int   ImageHeight;
int   ImageType;
int   ImageSize;

int   FlipTextureX;
int   FlipTextureY;

void *ImagePointer;
void *TlutPointer;

static void Text1Init(void);
static void Text1Func(void);
static void Text1Engine(void);

static void doLinePoly(void);

TestStructureElement TestStruct[] =
  {
    "Text1",  Text1Func,  doLinePoly,  Text1Init,  Text1Engine,
  };

int     ControllerInput       =  0;
int     PrintInstructions     =  1;
int     NewTextureNumber      =  1;

extern char _codeSegmentBssEnd[], _cfbSegmentBssStart[];
extern char _staticSegmentRomStart[], _staticSegmentRomEnd[];

/*
 * Stacks for the threads as well as message queues for synchronization
 * This stack is ridiculously large, and could also be reclaimed once
 * the main thread is started.
 */
u64	bootStack[STACKSIZE/sizeof(u64)];

static void	idle(void *);
static void	mainproc(void *);

static OSThread	idleThread;
static u64	idleThreadStack[STACKSIZE/sizeof(u64)];

static OSThread	mainThread;
static u64	mainThreadStack[STACKSIZE/sizeof(u64)];

/* this number (the depth of the message queue) needs to be equal
 * to the maximum number of possible overlapping PI requests.
 * For this app, 1 or 2 is probably plenty, other apps might
 * require a lot more.
 */
#define NUM_PI_MSGS     8

static OSMesg PiMessages[NUM_PI_MSGS];
static OSMesgQueue PiMessageQ;

OSMesgQueue     dmaMessageQ, rspMessageQ, rdpMessageQ, retraceMessageQ;
OSMesg          dmaMessageBuf, rspMessageBuf, rdpMessageBuf, retraceMessageBuf;
OSMesg          dummyMessage;
OSIoMesg        dmaIOMessageBuf;

char		*staticSegment;
OSMesg		dummyMesg;
OSTask		*tlistp;
Dynamic		*dynamicp;

/*
 * Dynamic data.
 */
Dynamic dynamic;

/*
 * necessary for RSP tasks:
 */
u64 dram_stack[SP_DRAM_STACK_SIZE64];	/* used for matrix stack */

    
/*
 * Task descriptor.
 */
OSTask	tlist =
{
    M_GFXTASK,			/* task type */
    OS_TASK_DP_WAIT,		/* task flags */
    NULL,			/* boot ucode pointer (fill in later) */
    0,				/* boot ucode size (fill in later) */
    NULL,			/* task ucode pointer (fill in later) */
    SP_UCODE_SIZE,		/* task ucode size */
    NULL,			/* task ucode data pointer (fill in later) */
    SP_UCODE_DATA_SIZE,		/* task ucode data size */
    &(dram_stack[0]),		/* task dram stack pointer */
    SP_DRAM_STACK_SIZE8,	/* task dram stack size */
    NULL,            		/* task output buffer ptr (not always used) */
    NULL,            		/* task output buffer size ptr */
    NULL,			/* task data pointer (fill in later) */
    0,				/* task data size (fill in later) */
    NULL,			/* task yield buffer ptr (not used here) */
    0				/* task yield buffer size (not used here) */
};

Gfx		*glistp;	/* global for test case procs */
    
/*
 * global variables
 */

static int      draw_buffer = 0;
static int      FrameRate   = 1;

void    *cfb_ptrs[2];

#ifdef DEBUG
void parse_args(char *);
#endif

OSPiHandle	*handler;

void
boot(void)
{

#ifdef DEBUG
    int i;
    u32 *argp;
    u32 argbuf[16];
#endif
    
    osInitialize();

    handler = osCartRomInit();

#ifdef DEBUG
    argp = (u32 *)RAMROM_APP_WRITE_ADDR;
    for (i=0; i<sizeof(argbuf)/4; i++, argp++) 
      {
	osEPiReadIo(handler, (u32)argp, &argbuf[i]); /* Assume no DMA */
      }

    parse_args((char *) argbuf);
#endif
    
    osCreateThread(&idleThread, 1, idle, (void *)0,
		   idleThreadStack+STACKSIZE/sizeof(u64), 10);
    osStartThread(&idleThread);

    /* never reached */
}


/* copied out of simple's misc.c */

static int myatoi(char *str)
{
    int		log10[5], logn = 0, val = 0, i, pow10 = 1;

    if (str == NULL || *str == '\0')
	return(-2);

    while (*str != '\0') 
	{
	    if (!(*str == '0' ||
		  *str == '1' ||
		  *str == '2' ||
		  *str == '3' ||
		  *str == '4' ||
		  *str == '5' ||
		  *str == '6' ||
		  *str == '7' ||
		  *str == '8' ||
		  *str == '9')) 
		{
		    logn = 0;
		    break;
		}
	    
	    log10[logn++] = *str - '0';
	    str++;
	}

    if (logn == 0)
	return(-3);
    
    for (i=logn-1; i>= 0; i--) 
	{
	    val += log10[i] * pow10;
	    pow10 *= 10;
	}

    return (val);
}


#ifdef DEBUG
void parse_args(char *argstring)
{
  int		argc = 1;
  char	*arglist[32], **argv = arglist;	/* max 32 args */
  char	*ptr;

  if (argstring == NULL || argstring[0] == '\0')
    return;

  /* re-organize argstring to be like main(argv,argc) */

  ptr = argstring;
  while (*ptr != '\0') {
    while (*ptr != '\0' && (*ptr == ' ')) 
      {
	*ptr = '\0';
	ptr++;
      }
    if (*ptr != '\0')
      arglist[argc++] = ptr;
    while (*ptr != '\0' && (*ptr != ' ')) 
      {
	ptr++;
      }
  }

  /* process the arguments: */
  while ((argc > 1) && (argv[1][0] == '-')) 
    {
      switch(argv[1][1]) 
	{
	case 'i':
	  PrintInstructions = 0;
	  break;

	default:
	  break;
	}
      
      argc--;
      argv++;
    }
}
#endif


static void
idle(void *arg)
{
    /* Initialize video */
    osCreateViManager(OS_PRIORITY_VIMGR);
    osViSetMode(&osViModeTable[OS_VI_NTSC_HAF1]);

    osViBlack(TRUE);
    
    /*
     * Start PI Mgr for access to cartridge
     */
    osCreatePiManager((OSPri)OS_PRIORITY_PIMGR, &PiMessageQ, PiMessages, 
		      NUM_PI_MSGS);
    
    /*
     * at this point, AND NOT BEFORE, we can now do an
     * PRINTF()
     */
    
    /*
     * Create main thread
     */
    osCreateThread(&mainThread, 3, mainproc, arg,
		   mainThreadStack+STACKSIZE/sizeof(u64), 10);
    
    osStartThread(&mainThread);

    /*
     * Become the idle thread
     */
    osSetThreadPri(0, 0);

    for (;;);
}

void CreateMessageQueues(void)
{
  osCreateMesgQueue(&dmaMessageQ, &dmaMessageBuf, 1);
  
  osCreateMesgQueue(&rspMessageQ, &rspMessageBuf, 1);
  osSetEventMesg(OS_EVENT_SP, &rspMessageQ, dummyMessage);
  
  osCreateMesgQueue(&rdpMessageQ, &rdpMessageBuf, 1);
  osSetEventMesg(OS_EVENT_DP, &rdpMessageQ, dummyMessage);
  
  osCreateMesgQueue(&retraceMessageQ, &retraceMessageBuf, 1);
  osViSetEvent(&retraceMessageQ, dummyMessage, FrameRate);  
}

void DMAStaticSegment(void)
{
  if ((u32) _codeSegmentBssEnd        + 
      (u32) _staticSegmentRomEnd   -   
      (u32) _staticSegmentRomStart >=
      (u32) _cfbSegmentBssStart)
    {
      while(1);
    }
      
  staticSegment = _codeSegmentBssEnd;

  dmaIOMessageBuf.hdr.pri      = OS_MESG_PRI_NORMAL;
  dmaIOMessageBuf.hdr.retQueue = &dmaMessageQ;
  dmaIOMessageBuf.dramAddr     = staticSegment;
  dmaIOMessageBuf.devAddr      = (u32)_staticSegmentRomStart;
  dmaIOMessageBuf.size         = (u32)_staticSegmentRomEnd-(u32)_staticSegmentRomStart;

  osEPiStartDma(handler, &dmaIOMessageBuf, OS_READ);
  
  (void)osRecvMesg(&dmaMessageQ, NULL, OS_MESG_BLOCK);     
}

void SetupSegments(void)
{
  /* Tell RCP where each segment is */

  gSPSegment(glistp++, 0, 0x0);   
  gSPSegment(glistp++, STATIC_SEGMENT, osVirtualToPhysical(staticSegment));
}

void CreateTaskStructure(void)
{
  /* Build graphics task */

  tlistp->t.ucode_boot = (u64 *) rspbootTextStart;
  tlistp->t.ucode_boot_size = (u32)rspbootTextEnd - (u32)rspbootTextStart;

  /* choose which ucode to run */

  tlistp->t.ucode      = (u64 *) gspSprite2DTextStart;
  tlistp->t.ucode_data = (u64 *) gspSprite2DDataStart;
  
  /* initial display list: */
  tlistp->t.data_ptr = (u64 *) dynamicp->glist;
}

static void SwapViBuffer(void)
{
  /* setup to swap buffers */
  osViSwapBuffer(cfb_ptrs[draw_buffer]);

  /* Make sure there isn't an old retrace in queue */
  if (MQ_IS_FULL(&retraceMessageQ))
    (void)osRecvMesg(&retraceMessageQ, NULL, OS_MESG_BLOCK);
  
  /* Wait for Vertical retrace to finish swap buffers */
  (void)osRecvMesg(&retraceMessageQ, NULL, OS_MESG_BLOCK);

  draw_buffer ^= 1;
}

static void IdentMatrices(void)
{
  guRotate(&dynamicp->modeling1, 0, 0.0F, 0.0F, 1.0F); 

  gSPMatrix(glistp++, OS_K0_TO_PHYSICAL(&(dynamic.modeling1)),
	    G_MTX_MODELVIEW|G_MTX_LOAD|G_MTX_NOPUSH);
}

static void SetupViewing(void)
{
  guOrtho(&dynamicp->projection,
	  0.00, SCREEN_WD-1,
	  0.00, SCREEN_HT-1, 
	  -200, 200, 
	  1.0);

  gSPMatrix(glistp++, OS_K0_TO_PHYSICAL(&(dynamic.projection)),
		G_MTX_PROJECTION|G_MTX_LOAD|G_MTX_NOPUSH);        
}

static void SetupPolyMatrices(void)
{
  guTranslate(&dynamicp->modeling2,  
	      SCREEN_WD/2.0 + TranslateHorizontal/320.0 * SCREEN_WD, 
	      SCREEN_HT/2.0 - TranslateVertical/240.0 * SCREEN_HT,
	      0.0);

  gSPMatrix(glistp++, OS_K0_TO_PHYSICAL(&(dynamic.modeling2)),
	    G_MTX_MODELVIEW|G_MTX_LOAD|G_MTX_NOPUSH);

  guRotate(&dynamicp->modeling3, 0, 1.0, 1.0, 0.0);  
  gSPMatrix(glistp++, OS_K0_TO_PHYSICAL(&(dynamic.modeling3)),
	    G_MTX_MODELVIEW|G_MTX_MUL|G_MTX_NOPUSH);

  guTranslate(&dynamicp->modeling4, 
	      -SCREEN_WD/2.0,
	      -SCREEN_HT/2.0,
	      0.0); 

  gSPMatrix(glistp++, OS_K0_TO_PHYSICAL(&(dynamic.modeling4)),
	    G_MTX_MODELVIEW|G_MTX_MUL|G_MTX_NOPUSH);
}

static void DoCommonSetup(void)
{
  /* Setup display modes antialiased in 1 cycle */

  gDPSetRenderMode(glistp++, G_RM_AA_OPA_SURF, G_RM_AA_OPA_SURF2);
  
  SetupViewing();
  IdentMatrices(); 
  SetupPolyMatrices();
}

/* stupid procedure name... */
static void CleanupAndSendDisplayList(void)
{
  gDPFullSync(glistp++);
  gSPEndDisplayList(glistp++);

#ifdef DEBUG	
#ifndef __MWERKS__
  assert((glistp-dynamicp->glist) < GLIST_LEN);
#endif
#endif
	
  tlistp->t.data_size = (u32)((glistp - dynamicp->glist) * sizeof(Gfx));
	
  /* Write back dirty cache lines that need to be read by the RCP */
  osWritebackDCache(&dynamic, sizeof(dynamic));
	
  /* start up the RSP task */
  CreateTaskStructure();

  /* osSpTaskStart(tlistp); */
  osSpTaskLoad(tlistp);
  
  osSpTaskStartGo(tlistp);

  /* wait for SP completion */
  (void)osRecvMesg(&rspMessageQ, NULL, OS_MESG_BLOCK);
	
  (void)osRecvMesg(&rdpMessageQ, NULL, OS_MESG_BLOCK);
}

static void InitViewport(void)
{
  static Vp mvp;
  
  mvp.vp.vscale[0] = SCREEN_WD*2;
  mvp.vp.vscale[1] = SCREEN_HT*2;
  mvp.vp.vscale[2] = G_MAXZ/2;
  mvp.vp.vscale[3] = 0;
  
  mvp.vp.vtrans[0] = SCREEN_WD*2;
  mvp.vp.vtrans[1] = SCREEN_HT*2;
  mvp.vp.vtrans[2] = G_MAXZ/2;
  mvp.vp.vtrans[3] = 0;
  
  gSPViewport(glistp++, &mvp);
}

static void InitDisplayLists(void)
{
  /* pointers to build the display list */
  glistp = dynamicp->glist;
  
  SetupSegments();

  /* Setup scissor values */
  gDPSetScissor(glistp++, G_SC_NON_INTERLACE, 0, 0, SCREEN_WD, SCREEN_HT);
  
  /* Initialize RDP state */
  gSPDisplayList(glistp++, rdpinit_dl);

   /* Setup the Viewport */
  InitViewport();
  
  /* Initialize RSP state */
  gSPDisplayList(glistp++, rspinit_dl);  
}

static void ClearFrameBuffer(void)
{
  /* Clear color framebuffer */

  gDPSetCycleType(glistp++, G_CYC_FILL);

  gDPSetColorImage(glistp++, G_IM_FMT_RGBA, G_IM_SIZ_16b, SCREEN_WD, 
		   OS_K0_TO_PHYSICAL(cfb_ptrs[draw_buffer]));
  gDPSetFillColor(glistp++, GPACK_RGBA5551(64,64,64,1) << 16 | 
		  GPACK_RGBA5551(64,64,64,1));

  gDPFillRectangle(glistp++, 0, 0, SCREEN_WD-1, SCREEN_HT-1);
  gDPPipeSync(glistp++);

  gDPSetCycleType(glistp++, G_CYC_1CYCLE);  
}

/* Example of how to do a static sprite structure */
static uSprite mySpriteStruct =
{
  dk,             /* Image Pointer */ 
  NULL,           /* TlutPointer */
  640,            /* Stride */
  200,            /* SubImageWidth */
  100,            /* SubImageHeight */
  G_IM_FMT_RGBA,  /* SourceImageType */
  G_IM_SIZ_32b,   /* SourceImageBitSize */
  0, 0,           /* SourceImageOffsetS, SourceImageOffsetT */
};

Gfx sprite_dl[] =
{
  gsSPSprite2DBase(&mySpriteStruct),
  gsSPSprite2DScaleFlip(512, 512, 0, 0),
  gsSPSprite2DDraw(400, 400),
  gsSPEndDisplayList(),
};

uSprite MySprite;
static void DoText1Rect(void)
{  
#if 1
  guSprite2DInit(&MySprite, ImagePointer, TlutPointer, 
		 ImageWidth, RectangleWidth, RectangleHeight,
		 ImageType, 
		 ImageSize,		 
		 TextureStartS, TextureStartT);	  
  
  gSPSprite2DBase(glistp++, OS_K0_TO_PHYSICAL(&MySprite));      
  gSPSprite2DScaleFlip(glistp++, TextureScaleX, TextureScaleY,
		       FlipTextureX, FlipTextureY);
  gSPSprite2DDraw(glistp++, TranslateHorizontal, TranslateVertical); 


#else
  gSPDisplayList(glistp++, sprite_dl);
#endif
}

static void InitForNewTexture(void)
{
  switch (TextureNumber)
    {

    case 0:
      ImageWidth    = 640;
      ImageHeight   = 479;
      ImageType     = G_IM_FMT_RGBA;
      ImageSize     = G_IM_SIZ_32b;
      ImagePointer  = dk;
      break;

    case 1:
      ImageWidth    = 512;
      ImageHeight   = 256;
      ImageType     = G_IM_FMT_CI;
      ImageSize     = G_IM_SIZ_4b;
      ImagePointer  = dk7;
      TlutPointer   = dk7tlut;
      break;

    default:

#ifdef DEBUG
      PRINTF("!!! ERROR, bad texture number %d \n", TextureNumber);
#endif

      break;
    }

  if (RectangleWidth > ImageWidth) RectangleWidth = ImageWidth;
  if (RectangleHeight > ImageHeight) RectangleHeight = ImageHeight;

  {
    int MaxS, MaxT;

    MaxS = ImageWidth  - 1 - (RectangleWidth - 1);
    MaxT = ImageHeight - RectangleHeight;

    if (TextureStartS > MaxS) TextureStartS = MaxS;
    if (TextureStartT > MaxT) TextureStartT = MaxT;
  }
}

static void Text1Init(void)
{
  FlipTextureY          = 0;

  RectangleWidth        = 512;
  RectangleHeight       = 400; 

  TextureStartS         = 0;
  TextureStartT         = 0; 

  TranslateHorizontal   =   58*4;
  TranslateVertical     =   44*4; 

  TextureScaleX         = 1024;
  TextureScaleY         = 1024;

  TextureNumber         = 0;

  InitForNewTexture();
}

static void Text1Func(void)
{
  DoText1Rect();

  CleanupAndSendDisplayList();	
}

static void Text1Engine(void)
{
}

void WriteOutInstructions1(void)
{

#ifdef DEBUG
  PRINTF(" L/R        (top buttons)    toggle X/Y flipping \n");
  PRINTF(" Stick                       translate image \n");
  PRINTF(" Left/Right (pad)            Change Texture Start S \n");
  PRINTF(" Up/Down    (pad)            Change Texture Start T \n");

  PRINTF(" A                           Change texture displayed \n");
  PRINTF(" C/F        (arrows)         Change Rectangle Width \n");
  PRINTF(" D/E        (arrows)         Change Rectangle Height \n");

  PRINTF(" G          (underneath)     toggle controller mode \n");
  PRINTF(" Start                       reset values \n"); 
#endif

}

void WriteOutInstructions2(void)
{

#ifdef DEBUG
  PRINTF(" L/R        (top buttons)    toggle X/Y flipping \n");
  PRINTF(" Stick                       translate image \n");

  PRINTF(" Left/Right (pad)            Change Texture Scale X \n");
  PRINTF(" Up/Down    (pad)            Change Texture Scale Y \n");

  PRINTF(" C/F        (arrows)         Change Rectangle Width \n");
  PRINTF(" D/E        (arrows)         Change Rectangle Height \n");

  PRINTF(" G          (underneath)     toggle controller mode \n");
  PRINTF(" Start                       reset values \n"); 
#endif

}

void ClearScreen(void)
{

#ifdef DEBUG
  /* write screen clear, take out if it's too obnoxious  */
  PRINTF("%c%c%c%c%c%c\n", 27, 91, 72, 27, 91, 74); 
#endif

}

void PrintCurrentTextureSettings(void)
{

#ifdef DEBUG
  ClearScreen();
  PRINTF("TranslateHorizontal -> %.2f\tTranslateVertical -> %.2f \n",
	 TranslateHorizontal/4.0, TranslateVertical/4.0);

  PRINTF("Texture Start S -> %d\tTextureStart T -> %d \n",
	 TextureStartS, TextureStartT);

  PRINTF("Rectangle Width -> %d\t Rectangle Height -> %d \n",
	 RectangleWidth, RectangleHeight);

  PRINTF("Texture ScaleX -> %.3f\tTexture ScaleY -> %.3f\tTexture %d \n", 
	 (float) TextureScaleX / (float) (1 << 10),
	 (float) TextureScaleY / (float) (1 << 10),
	 TextureNumber);

  PRINTF("\n");
  
  PRINTF("Texture Width -> %d\tTexture Height -> %d \n",
	 ImageWidth, ImageHeight);

  PRINTF("Texel Type    -> %s\tTexel Size     -> %d Bits \n",
	 (ImageType == G_IM_FMT_RGBA) ? "RGBA" : 
	 (ImageType == G_IM_FMT_IA)   ? "IA"   :
	 (ImageType == G_IM_FMT_CI)   ? "CI"   : "OTHER",
	 (ImageSize == G_IM_SIZ_4b)   ? 4      : 
	 (ImageSize == G_IM_SIZ_8b)   ? 8      :
	 (ImageSize == G_IM_SIZ_16b)  ? 16     : 32);

  PRINTF("Flip X\t\t-> %.3s\tFlip Y\t\t -> %.3s \n",
	 (FlipTextureX) ? "yes" : "no",
	 (FlipTextureY) ? "yes" : "no");

  PRINTF("\n");
#endif

}

static void doLinePoly(void)
{
  ClearFrameBuffer(); 
  DoCommonSetup(); 
      
  (*(TestStruct[0]).TestDisplayFunc)();
  (*(TestStruct[0]).TestEngineFunc)();
      
  SwapViBuffer();      
}

void InitVariables(void)
{
  InitForNewTexture();
  Text1Init();
}

static void WriteOutInstructions(void)
{
  switch (ControllerMode)
    {
    case 1:
      WriteOutInstructions1();
      break;
    case 2:
      WriteOutInstructions2();
      break;
    default:

#ifdef DEBUG
      PRINTF("!!! ERROR: Bad instruction case \n");
#endif

      break;
    }
}

int PrintScreenData(void)
{
  if (ControllerInput == 1)
    {
      if (PrintInstructions) 
	{
	  PrintCurrentTextureSettings();
	  WriteOutInstructions();
	}
      ControllerInput = 0;
    }
}

int DoMainAppLoops(void)
{
  while (1)
    {
      /* Moved this call to here for efficiency */
      osContStartReadData(&controllerMsgQ);

      ReadController();

      if (NewTextureNumber)
	{
	  InitForNewTexture();

	  NewTextureNumber = 0;
	}

      PrintScreenData();

      InitDisplayLists();
      (*(TestStruct[0]).TestMainLoop)();
    }
}

static void mainproc(void *arg)
{
  tlistp = &tlist;
  dynamicp = &dynamic;
  cfb_ptrs[0] = cfb_a; 
  cfb_ptrs[1] = cfb_b; 

  ActiveController = initControllers();
  
  CreateMessageQueues();
  DMAStaticSegment();

  InitVariables();
  (*(TestStruct[0]).TestInitFunc)();
  
  if (PrintInstructions) 
    {
      PrintCurrentTextureSettings();
      WriteOutInstructions();
    }

  osViBlack(FALSE);
  DoMainAppLoops();
}
