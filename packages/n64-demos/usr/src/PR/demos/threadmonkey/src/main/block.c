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
  Copyright (C) 1997,1998 Nintendo. (Originated by SGI)
  
  $RCSfile: block.c,v $
  $Revision: 1.2 $
  $Date: 1999/07/15 09:25:06 $
*---------------------------------------------------------------------*/

/*
 * File:	block.c
 * Create Date:	Tue Sep 19 16:20:26 PDT 1995
 * Creator:     Kevin Luster, kluster@sgi.com
 *
 */

#include <ultra64.h>
#include <PR/ramrom.h>	/* needed for argument passing into the app */
#include <assert.h>
#include <PR/os.h>

#include "controller.h"
#include "block.h"
#include "static.h"
#include "subcube.h"
#include "subplane.h"
#include "timer.h"

#ifdef DEBUG
#define PRINTF osSyncPrintf
#endif

/*
 * Symbol genererated by "makerom" to indicate the end of the code segment
 * in virtual (and physical) memory
 */
extern char _codeSegmentEnd[];

/*
 * Symbols generated by "makerom" to tell us where the static segment is
 * in ROM.
 */

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

OSMesg		dummyMesg;
OSTask		*tlistp[MAXTASKS];
Dynamic		*dynamicp;

int             ActiveController;
int             ControllerInput     =  1;
int             CreateNewCubes      =  1;
int             CreateNewPlanes     =  1;
int             UseAAMode           =  0;
int             UseZMode            =  1;
int             SelfScaleTimer      =  1;
//int             SelfScaleTimer      =  0;
int             Quiet               =  0;
//int             Quiet               =  1;
int             EyeZPosition        =  0;
int             TranslateHorizontal =  0;
int             TranslateVertical   =  0;
int             RotateGlobalXAxis   =  0;
int             RotateGlobalYAxis   =  0;
int             ClipRatio           =  2;
int             TimerTicks          =  7;
int             UseTextureMode      =  0;
int             ShadingMode         =  1;
int             UseGlobalTransforms =  1;
int             ChangeVideoModes    =  0;
int		numtasks	    =  1;

int             UseViModeX          =  0;
int             ScreenWidth         =  SCREEN_WD_320;
int             ScreenWidthDelta    =  0;
int             rdp_flag            =  0; /* 0:xbus , 1:fifo */
int		OutLen              =  RDP_OUTPUT_LEN;

int             HorizontalOffset    =  0;
int             VerticalOffset      =  0;

float           TimePerFrame[MAXTASKS];
float           TimePerFrameSp[MAXTASKS];
float           TimePerFrameDp[MAXTASKS];
float           TicksPerSec;

OSTime          t1, t2, tm, tm2;

/*
 * Dynamic data.
 */
Dynamic dynamic;

/*
 * Task descriptor.
 */
OSTask  tlist[MAXTASKS];

Gfx		*glistp;	/* global for test case procs */
     
/*
 * global variables
 */

int      draw_buffer        = 0;

void    *cfb_ptrs[2];

OSViMode viModeXpn1;	/* Structure for test Vi mode Xpn1
			   which is based on Lpn1 */

OSViMode *CurrentVideoModePtr;
OSViMode OriginalVideoMode;

OSPiHandle	*handler;

void boot(void)
{
    /* notice that you can't call osSyncPrintf() until you set
     * up an idle thread.
     */
    
    osInitialize();
    osInitialize_isv();

    handler = osCartRomInit();

    osCreateThread(&idleThread, 1, idle, (void *)0,
		   idleThreadStack+STACKSIZE/sizeof(u64), 10);
    osStartThread(&idleThread);

    /* never reached */
}

static void idle(void *arg)
{
    /* Initialize video */
    osCreateViManager(OS_PRIORITY_VIMGR);
  
    osViSetMode(&osViModeTable[OS_VI_NTSC_LAN1]);
    OriginalVideoMode = osViModeTable[OS_VI_NTSC_LAN1];
    CurrentVideoModePtr = &osViModeTable[OS_VI_NTSC_LAN1];
  
    ScreenWidth = SCREEN_WD_320;
    /*
     * Replicate Lpn1 mode to Xpn1
     */
    bcopy((char *)&osViModeNtscLpn1, (char *)&viModeXpn1,
	  sizeof(osViModeNtscLpn1));
  
    osViSetSpecialFeatures(OS_VI_DITHER_FILTER_ON);

    /* turn divot feature off so that tick marks don't get munged */
    osViSetSpecialFeatures(OS_VI_DIVOT_OFF);
  
    /*
     * Start PI Mgr for access to cartridge
     */
    osCreatePiManager((OSPri)OS_PRIORITY_PIMGR, &PiMessageQ, PiMessages, 
		      NUM_PI_MSGS);
  
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
    osViSetEvent(&retraceMessageQ, dummyMessage, 1);  
}

void SetupSegments(void)
{
    /* Tell RCP where each segment is */

    gSPSegment(glistp++, 0, 0x0);   
}

void CreateTaskStructure(int n)
{
    /* Build graphics task */

    tlistp[n]->t.ucode_boot = (u64 *) rspbootTextStart;
    tlistp[n]->t.ucode_boot_size = (u32)rspbootTextEnd - (u32)rspbootTextStart;

    /* choose which ucode to run */
    switch (rdp_flag) {
      case 0:			/* xbus */
	tlistp[n]->t.ucode      	    = (u64 *) gspF3DEX2_xbusTextStart;
	tlistp[n]->t.ucode_data 	    = (u64 *) gspF3DEX2_xbusDataStart;
	tlistp[n]->t.output_buff 	    = NULL;
	tlistp[n]->t.output_buff_size = NULL;
	tlistp[n]->t.flags	    = OS_TASK_DP_WAIT;
	break;
      case 1:			/* fifo */
	tlistp[n]->t.ucode      	    = (u64 *) gspF3DEX2_fifoTextStart;
	tlistp[n]->t.ucode_data 	    = (u64 *) gspF3DEX2_fifoDataStart;
	tlistp[n]->t.output_buff 	    = rdp_output;
	tlistp[n]->t.output_buff_size = (u64 *) ((u8 *)rdp_output + RDP_OUTPUT_LEN);
	tlistp[n]->t.flags      	    = 0;
	break;
    }

    /* initial display list: */
    tlistp[1]->t.data_ptr = (u64 *) dynamicp->glist;

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

void ApplyGlobalTransformations(void)
{
    guTranslate(&dynamicp->Translation,  
		TranslateHorizontal/320.0 * ScreenWidth, 
		TranslateVertical/240.0 * SCREEN_HT,
		0.0);

    guRotate(&dynamicp->RotationX, RotateGlobalXAxis, 1.0F, 0.0F, 0.0F); 
    guRotate(&dynamicp->RotationY, RotateGlobalYAxis, 0.0F, 1.0F, 0.0F); 

    gSPMatrix(glistp++, OS_K0_TO_PHYSICAL(&(dynamic.Translation)),
	      G_MTX_MODELVIEW|G_MTX_LOAD|G_MTX_NOPUSH);
  
    guTranslate(&dynamicp->TranslateOut,  
		+(XMAX+XMIN)/2.0, 
		+(YMAX+YMIN)/2.0,
		-(ZMAX+ZMIN)/2.0);
  
    gSPMatrix(glistp++, OS_K0_TO_PHYSICAL(&(dynamic.TranslateOut)),
	      G_MTX_MODELVIEW|G_MTX_MUL|G_MTX_NOPUSH);
  
  
    gSPMatrix(glistp++, OS_K0_TO_PHYSICAL(&(dynamic.RotationX)),
	      G_MTX_MODELVIEW|G_MTX_MUL|G_MTX_NOPUSH);
  
    gSPMatrix(glistp++, OS_K0_TO_PHYSICAL(&(dynamic.RotationY)),
	      G_MTX_MODELVIEW|G_MTX_MUL|G_MTX_NOPUSH);
  
    guTranslate(&dynamicp->TranslateIn,  
		-(XMAX+XMIN)/2.0,
		-(YMAX+YMIN)/2.0,
		+(ZMAX+ZMIN)/2.0);
  
    gSPMatrix(glistp++, OS_K0_TO_PHYSICAL(&(dynamic.TranslateIn)),
	      G_MTX_MODELVIEW|G_MTX_MUL|G_MTX_NOPUSH);  
}

Lights1 diffuseL1 =  gdSPDefLights1(0x10, 0x10, 0x10,
				    0xff, 0xff, 0xff,	  0,   0,  127);

static void SetupViewing(void)
{
    u16 perspNorm;

    guPerspective(&dynamicp->projection,&perspNorm,
		  33, 320.0/240.0, 1, 2000, 1.0);

    if (ShadingMode == LIGHTING)
      {
	  guLookAtHilite(&(dynamicp->viewing), 
			 &(dynamicp->lookat[0]), 
			 &(dynamicp->hilite[0]),
		     
			 160, 120, EYEZPOS - EyeZPosition,
			 160, 120, -EyeZPosition,
			 0, 1, 0,
		     
			 0.0, 0.0, 127.0, 
			 0.0, 0.0, 127.0,
		     
			 32,32);
      }
    else
      {
	  guLookAt(&dynamicp->viewing, 
		   160, 120, EYEZPOS - EyeZPosition,
		   160, 120, -EyeZPosition,
		   0, 1, 0);
      }

    gSPMatrix(glistp++, OS_K0_TO_PHYSICAL(&(dynamicp->projection)),
	      G_MTX_PROJECTION|G_MTX_LOAD|G_MTX_NOPUSH);

    gSPMatrix(glistp++, OS_K0_TO_PHYSICAL(&(dynamicp->viewing)),
	      G_MTX_PROJECTION|G_MTX_MUL|G_MTX_NOPUSH);  

    if (ShadingMode == LIGHTING)
      {
	  gSPLookAt(glistp++, &(dynamicp->lookat[0]));
	  gSPSetLights1(glistp++, diffuseL1);     
      }

    gSPPerspNormalize(glistp++, perspNorm);  

}

unsigned int xseed = (7789<<16)+13399;	

unsigned int xrand(void)
{
    unsigned int x;
    
    x = (xseed<<2) + 2;
    
    x *= (x+1);
    x = x >> 2;

    xseed = x;

    return( x );
}

float frand(void)
{
    return((xrand()%32767) / 32768.0);
}

static void SetRenderMode(void)
{
    if (UseAAMode==0 && UseZMode)
      {
	  gSPSetGeometryMode(glistp++, G_ZBUFFER);
	  gDPSetRenderMode(glistp++, 
			   (UseTextureMode == 2) ? G_RM_PASS : G_RM_AA_ZB_OPA_SURF, 
			   G_RM_AA_ZB_OPA_SURF2);      
      }
    else if (UseAAMode==2 && UseZMode)
      {
	  gSPSetGeometryMode(glistp++, G_ZBUFFER);
	  gDPSetRenderMode(glistp++, 
			   (UseTextureMode == 2) ? G_RM_PASS : G_RM_ZB_OPA_SURF , 
			   G_RM_ZB_OPA_SURF2);      
      }
    else if (UseAAMode==0 && !UseZMode)
      {
	  gSPClearGeometryMode(glistp++, G_ZBUFFER);
	  gDPSetRenderMode(glistp++, 
			   (UseTextureMode == 2) ? G_RM_PASS : G_RM_AA_OPA_SURF, 
			   G_RM_AA_OPA_SURF2);      
      }
    else if (UseAAMode==1 && UseZMode)
      {
	  gSPSetGeometryMode(glistp++, G_ZBUFFER);
	  gDPSetRenderMode(glistp++, 
			   (UseTextureMode == 2) ? G_RM_PASS : G_RM_RA_ZB_OPA_SURF, 
			   G_RM_RA_ZB_OPA_SURF2);      
      }
    else if (UseAAMode==1 && !UseZMode)
      {
	  gSPClearGeometryMode(glistp++, G_ZBUFFER);
	  gDPSetRenderMode(glistp++, 
			   (UseTextureMode == 2) ? G_RM_PASS : G_RM_RA_OPA_SURF, 
			   G_RM_RA_OPA_SURF2);      
      }
    else
      {
	  gSPClearGeometryMode(glistp++, G_ZBUFFER);
	  gDPSetRenderMode(glistp++, 
			   (UseTextureMode == 2) ? G_RM_PASS : G_RM_OPA_SURF, 
			   G_RM_OPA_SURF2);      
      }  
}

static void SetClipRation(void)
{
    switch (ClipRatio)    
      {
	case 1:
	  gSPClipRatio(glistp++, FRUSTRATIO_1);
	  break;
	case 2:
	  gSPClipRatio(glistp++, FRUSTRATIO_2);
	  break;
	case 3:
	  gSPClipRatio(glistp++, FRUSTRATIO_3);
	  break;
	case 4:
	  gSPClipRatio(glistp++, FRUSTRATIO_4);
	  break;
	case 5:
	  gSPClipRatio(glistp++, FRUSTRATIO_5);
	  break;
	case 6:
	  gSPClipRatio(glistp++, FRUSTRATIO_6);
	  break;
      }    
}

static void doPolyFuncs(int n)
{
    /* Setup display modes antialiased in 1 cycle */

    SetRenderMode();
    SetClipRation();
    SetupViewing();

    if (UseGlobalTransforms)
      ApplyGlobalTransformations();

    if (numtasks == 1 || n==1)
      CreatePlanes();
    if (numtasks == 1 || n>1) 
      CreateBlocks(n);
}

static void InitRsp(void)
{
    static Vp vp;

    vp.vp.vscale[0] = ScreenWidth*2;
    vp.vp.vscale[1] = SCREEN_HT*2;
    vp.vp.vscale[2] = G_MAXZ/2;
    vp.vp.vscale[3] = 0;
    vp.vp.vtrans[0] = ScreenWidth*2;
    vp.vp.vtrans[1] = SCREEN_HT*2;
    vp.vp.vtrans[2] = G_MAXZ/2;
    vp.vp.vtrans[3] = 0;

    gSPViewport(glistp++, &vp);

    gSPClearGeometryMode(glistp++, G_SHADE | G_SHADING_SMOOTH | G_CULL_BOTH |
			 G_FOG | G_LIGHTING | G_TEXTURE_GEN |
			 G_TEXTURE_GEN_LINEAR | G_LOD | G_ZBUFFER);

    gSPSetGeometryMode(glistp++, G_CULL_BACK);

    if (ShadingMode == GOURAUD)   gSPSetGeometryMode(glistp++, G_SHADE | G_SHADING_SMOOTH);
    if (ShadingMode == LIGHTING)  gSPSetGeometryMode(glistp++, G_SHADE | G_LIGHTING);
}

static void InitRdp(void)
{
    gDPPipelineMode(glistp++, G_PM_NPRIMITIVE);
    gDPSetScissor(glistp++, G_SC_NON_INTERLACE, 0, 0, ScreenWidth, SCREEN_HT);
    gDPSetTextureLOD(glistp++, G_TL_TILE);
    gDPSetTextureLUT(glistp++, G_TT_NONE);
    gDPSetTextureDetail(glistp++, G_TD_CLAMP);
    gDPSetTexturePersp(glistp++, G_TP_PERSP);
    gDPSetTextureFilter(glistp++, G_TF_BILERP);
    gDPSetTextureConvert(glistp++, G_TC_FILT);
    gDPSetCombineKey(glistp++, G_CK_NONE);
    gDPSetAlphaCompare(glistp++, G_AC_NONE);
    gDPSetColorDither(glistp++, G_CD_BAYER);
    gDPSetMaskImage(glistp++, zbuffer);
    gDPSetPrimColor(glistp++, 0, 0, 0, 0, 64, 255);
    gDPPipeSync(glistp++);
}

int ModifyHStart(void)
{
    int RegisterValue;

    RegisterValue = OriginalVideoMode.comRegs.hStart;

    (*CurrentVideoModePtr).comRegs.hStart = 
      (((((RegisterValue >> 16) & 0xffff) + HorizontalOffset) % 0xffff) << 16) | 
	(((((RegisterValue >>  0) & 0xffff) + HorizontalOffset) % 0xffff) <<  0);
}

int ModifyVStart(void)
{
    int RegisterValue;

    /* Do Field 0 change */
    RegisterValue = OriginalVideoMode.fldRegs[0].vStart;
    
    (*CurrentVideoModePtr).fldRegs[0].vStart = 
      (((((RegisterValue >> 16) & 0xffff) + VerticalOffset) % 0xffff) << 16) | 
	(((((RegisterValue >>  0) & 0xffff) + VerticalOffset) % 0xffff) <<  0);


    /* Do Field 1 change */
    RegisterValue = OriginalVideoMode.fldRegs[1].vStart;
    
    (*CurrentVideoModePtr).fldRegs[1].vStart = 
      (((((RegisterValue >> 16) & 0xffff) + VerticalOffset) % 0xffff) << 16) | 
	(((((RegisterValue >>  0) & 0xffff) + VerticalOffset) % 0xffff) <<  0);
}

static void ModifyScreenPosition(void)
{
    if (HorizontalOffset) ModifyHStart();
    if (VerticalOffset)   ModifyVStart();  
}

static void UpdateViMode(void)
{
    u32 xScale;
    OSIntMask im;

    ScreenWidth = SCREEN_WD_MIN+ (UseViModeX ? ScreenWidthDelta : 0);

    if (ScreenWidth < SCREEN_WD_MIN)
      ScreenWidth = SCREEN_WD_MIN;
    if (ScreenWidth > SCREEN_WD_MAX)
      ScreenWidth = SCREEN_WD_MAX;
    xScale = (u32)((ScreenWidth * XSCALE_MAX) / SCREEN_WD_MAX);

    /* Change width, xScale, and origin */
    im = osSetIntMask(OS_IM_VI);
    viModeXpn1.comRegs.width     = ScreenWidth;
    viModeXpn1.comRegs.xScale    = xScale;
    viModeXpn1.fldRegs[0].origin = ScreenWidth*2;
    viModeXpn1.fldRegs[1].origin = ScreenWidth*2;
    (void)osSetIntMask(im);

    if (ChangeVideoModes)
      {
	  /* Write back original Mode settings */
	  *CurrentVideoModePtr = OriginalVideoMode;
      
	  if (!UseViModeX)
	    {
		if (UseAAMode)
		  {
		      osViSetMode(&osViModeTable[OS_VI_NTSC_LAN1]);
		      OriginalVideoMode   = osViModeTable[OS_VI_NTSC_LAN1];
		      CurrentVideoModePtr = &osViModeTable[OS_VI_NTSC_LAN1];
		      ModifyScreenPosition();
		  }
		else
		  {
		      osViSetMode(&osViModeTable[OS_VI_NTSC_LPN1]);
		      OriginalVideoMode   = osViModeTable[OS_VI_NTSC_LPN1];
		      CurrentVideoModePtr = &osViModeTable[OS_VI_NTSC_LPN1];
		      ModifyScreenPosition();
		  }
	  
		ScreenWidth = SCREEN_WD_320;
	    }
	  else 
	    {
		osViSetMode(&viModeXpn1);
		OriginalVideoMode   = viModeXpn1;
		CurrentVideoModePtr = &viModeXpn1;
		ModifyScreenPosition();
	    }

	  osViSetSpecialFeatures(OS_VI_DITHER_FILTER_ON);
	  ChangeVideoModes = 0;
      }
}

static void InitDisplayLists(void)
{
  
    SetupSegments();
  
    /* Initialize RDP state */
    InitRdp();

    /* Initialize RSP state */
    InitRsp();

    if (UseTextureMode)
      {
	  gDPSetTextureFilter(glistp++, G_TF_BILERP);

	  if (UseTextureMode == 2)
	    {
		gSPTexture(glistp++, 0x8000, 0x8000, 5, G_TX_RENDERTILE, G_ON);
		gDPSetTextureDetail(glistp++, G_TD_CLAMP);      
		gDPSetTextureLOD(glistp++, G_TL_LOD);
		gDPPipelineMode(glistp++, G_PM_1PRIMITIVE);
	  
		if (ShadingMode == PRIMCOLOR)
		  gDPSetCombineMode(glistp++, G_CC_TRILERP, G_CC_DECALRGB2)
		    else
		      gDPSetCombineMode(glistp++, G_CC_TRILERP, G_CC_MODULATERGB2); 
	    }
	  else
	    {
		gSPTexture(glistp++, 0x8000, 0x8000, 0, G_TX_RENDERTILE, G_ON);
	  
		if (ShadingMode == PRIMCOLOR)
		  gDPSetCombineMode(glistp++, G_CC_DECALRGB, G_CC_DECALRGB)
		    else
		      gDPSetCombineMode(glistp++, G_CC_MODULATERGB, G_CC_MODULATERGB);  	  
	    }
      }
    else
      {
	  gSPTexture(glistp++, 0, 0, 0, 0, G_OFF);

	  if (ShadingMode == PRIMCOLOR)
	    gDPSetCombineMode(glistp++, G_CC_PRIMITIVE, G_CC_PRIMITIVE)
	      else 
		gDPSetCombineMode(glistp++, G_CC_SHADE, G_CC_SHADE);          
      }
}

static void ClearFrameBuffer(void)
{
    /* Clear color framebuffer */

    gDPSetCycleType(glistp++, G_CYC_FILL);

    gDPSetColorImage(glistp++, G_IM_FMT_RGBA, G_IM_SIZ_16b, ScreenWidth, 
		     OS_K0_TO_PHYSICAL(cfb_ptrs[draw_buffer]));
  
    gDPSetFillColor(glistp++, GPACK_RGBA5551(64,0,64,1) << 16 | 
		    GPACK_RGBA5551(64,0,64,1));
  
    gDPFillRectangle(glistp++, 0, 0, ScreenWidth-1, SCREEN_HT-1);

    gDPPipeSync(glistp++);

    if (UseTextureMode == 2)
      gDPSetCycleType(glistp++, G_CYC_2CYCLE)
	else
	  gDPSetCycleType(glistp++, G_CYC_1CYCLE);      
}

static void ClearZBuffer (void)
{
    gDPSetColorImage(glistp++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 
		     ScreenWidth, OS_K0_TO_PHYSICAL(zbuffer));

    gDPPipeSync(glistp++);

    gDPSetCycleType(glistp++, G_CYC_FILL);

    gDPSetFillColor(glistp++, 
		    GPACK_ZDZ(G_MAXFBZ, 0) << 16 | GPACK_ZDZ(G_MAXFBZ, 0));

    gDPFillRectangle(glistp++, 0, 0, ScreenWidth-1, SCREEN_HT-1);

    gDPPipeSync(glistp++);
}

static void CleanupDisplayList(int n)
{     

    if ( n==numtasks ) gDPFullSync(glistp++);
    gSPEndDisplayList(glistp++);

#ifdef DEBUG
#ifndef __MWERKS__
    assert((glistp-dynamicp->glist) < GLIST_LEN);
#endif
#endif
	
    tlistp[n+1]->t.data_ptr = (u64 *) glistp;
    tlistp[n]->t.data_size = 0; /* ignored */
	
    /* Write back dirty cache lines that need to be read by the RCP */
    osWritebackDCache(&dynamic, sizeof(dynamic));
	
    /* start up the RSP task */
    CreateTaskStructure(n);
}

static void SendDisplayLists(void)
{
    int spTask, dpTask;
    OSTime spstart, spend, dpstart, dpend;
    OSTime startallsp, endalldp;
    int    i;

    dpTask = 1;
    spTask = 1;

    osSpTaskLoad(tlistp[spTask]);
    spstart = startallsp = dpstart = osGetTime();
    osSpTaskStartGo(tlistp[spTask]);

    while ((dpTask <= numtasks) || (spTask < numtasks)) {

	if (osRecvMesg(&rspMessageQ, NULL, OS_MESG_NOBLOCK) != -1) {
	    spend = osGetTime();

	    switch (rdp_flag) {
	      case 0:		/* xbus */
		TimePerFrameSp[spTask] = 
		  1000.0*(float)(spend - spstart) /((OSTime) (TicksPerSec));
		spTask++;
		if (spTask <= numtasks) {
		    osSpTaskLoad(tlistp[spTask]);
		    spstart = osGetTime();
		    osSpTaskStartGo(tlistp[spTask]);
		}
		break;
	      case 1:		/* fifo */
		TimePerFrameSp[spTask] = 
		  1000.0*(float)(spend - spstart) /((OSTime) (TicksPerSec));
		spTask++;
		if (spTask <= numtasks) {
		    osSpTaskLoad(tlistp[spTask]);
		    spstart = osGetTime();
		    osSpTaskStartGo(tlistp[spTask]);
		}
		break;
	    }
	}

	if (osRecvMesg(&rdpMessageQ, NULL, OS_MESG_NOBLOCK) != -1) {
	    dpend = endalldp = osGetTime();

	    switch (rdp_flag) {
	      case 0:		/* xbus */
		TimePerFrameDp[1] = 
		  1000.0*(float)(dpend - dpstart) /((OSTime) (TicksPerSec));
		for (i=2; i<=numtasks; i++)
		  TimePerFrameDp[i] = 0;
		TimePerFrame[dpTask] = 0;
		dpstart = dpend;
		dpTask = numtasks+1;
		break;
	      case 1:		/* fifo */
		TimePerFrameDp[1] = 
		  1000.0*(float)(dpend - dpstart) /((OSTime) (TicksPerSec));
		for (i=2; i<=numtasks; i++)
		  TimePerFrameDp[i] = 0;
		TimePerFrame[dpTask] = 0;
		dpstart = dpend;
		dpTask = numtasks+1;
		break;
	    }
	}

    }

    TimePerFrame[0] = 
      1000.0*(float)(endalldp - startallsp) /((OSTime) (TicksPerSec));
}

static void CleanupAndSendDisplayList(void)
{
    gDPFullSync(glistp++);
    gSPEndDisplayList(glistp++);

#ifdef DEBUG
#ifndef __MWERKS__
    assert((glistp-dynamicp->glist) < GLIST_LEN);
#endif
#endif
	
    /*tlistp[1]->t.data_size = (u32)((glistp - dynamicp->glist) * sizeof(Gfx));*/
    tlistp[1]->t.data_size = 0; /* ignored */
	
    /* Write back dirty cache lines that need to be read by the RCP */
    osWritebackDCache(&dynamic, sizeof(dynamic));
	
    /* start up the RSP task */
    CreateTaskStructure(1);
  
    osSpTaskLoad(tlistp[1]);

    t1 = osGetTime(); 
    osSpTaskStartGo(tlistp[1]);

    (void) osRecvMesg(&rspMessageQ, NULL, OS_MESG_BLOCK);
    tm = osGetTime();

    (void) osRecvMesg(&rdpMessageQ, NULL, OS_MESG_BLOCK);

    t2 = osGetTime();
}


#ifdef DEBUG

static void PrintStatNumbers(int DisplayListLength)
{
    NumCubePolys  = 12 * NumSubCubes * NumCubesPerAxis * NumCubesPerAxis * NumCubesPerAxis;
    NumPlanePolys =  2 * NumSubPlanes + 4*2;

    PRINTF("%s %3s ZB %3s\t\t", 
	   (UseAAMode==1) ? "RA" : "AA",
	   (UseAAMode<2) ? "on" : "off",
	   UseZMode  ? "on" : "off");
  
    PRINTF("Cubes Per Row %d\t\tCube Divisions {%d,%d,%d} \n",
	   NumCubesPerAxis,
	   CubeDivisions[0], CubeDivisions[1], CubeDivisions[2]);

    PRINTF("Cube Width %3d\t\tCube tris %d\t\tPlane tris %d\n",
	   BlockWidth, NumCubePolys, NumPlanePolys);
  
    PRINTF("\t\t\ttasks %2d\t\ttris per task %d\n\n",
	   numtasks, (numtasks==1)?
	   (NumCubePolys+NumPlanePolys):(NumCubePolys/(numtasks-1)));
  
    PRINTF("Plane Dimensions X -> {%4d,%4d} \n",
	   (int) PlaneMinMax[0][0], (int) PlaneMinMax[0][1]);
  
    PRINTF("Plane Dimensions Z -> {%4d,%3d} \n",
	   (int) PlaneMinMax[1][0], (int) PlaneMinMax[1][1]);
  
    PRINTF("Plane Divisions    -> {%d, %d} \n",
	   PlaneDivisions[0], PlaneDivisions[1]);

    PRINTF("Horizontal offset  -> %3d\tVertical offset -> %3d \n",
	   HorizontalOffset, VerticalOffset);
  
    PRINTF("Current Vi Mode    -> ");
    if (UseViModeX) {
	PRINTF("Xpn1 (%dx240)      ", viModeXpn1.comRegs.width);
    }
    else {
	PRINTF("Lpn1/Lan1 (320x240)");
    }

    PRINTF(" Ucode: %s  ",(rdp_flag) ? "fifo" : "xbus");
    PRINTF("\n");

    if (UseViModeX)
      {
	  PRINTF(
		 "\t\tXpn1: (%dx240), width=0x%x, x_scale=0x%x, origin=0x%x\n",
		 viModeXpn1.comRegs.width,
		 viModeXpn1.comRegs.width,
		 viModeXpn1.comRegs.xScale,
		 viModeXpn1.fldRegs[0].origin);
      }

    PRINTF("\nDisplay List Len %d\tLighting %s\tTexture %3s\n", 
	   DisplayListLength, 
	   ShadingMode == 0 ? "PRIMCOLOR" :
	   ShadingMode == 1 ? "GOURAUD" : "LIGHTING",
	   (UseTextureMode == 0) ? "NONE" : (UseTextureMode == 1) ? "MIPMAP OFF" : "MIPMAP ON");      

    PRINTF("Controller Mode %d\tGlobal Transforms %3s\tClip Ratio %d\n", 
	   ControllerMode,
	   UseGlobalTransforms ? "on" : "off",
	   ClipRatio);
}


static void PrintController0Instructions(void)
{
    PRINTF("----------------------------------------------------------  \n");
    PRINTF("Up Pad       -> blockwidths     ++                   \n");
    PRINTF("Down Pad     -> blockwidths     --                   \n");
    PRINTF("Right Pad    -> planesize       ++                   \n");
    PRINTF("Left Pad     -> planesize       --                   \n");
    PRINTF("A Button     -> toggle AA                            \n");
    PRINTF("B Button     -> toggle ZB                            \n");
    PRINTF("Arrow Right  -> Cube Divisions  ++                   \n");
    PRINTF("Arrow Left   -> Cube Divisions  --                   \n");
    PRINTF("Arrow Up     -> Plane Divisions ++                   \n");
    PRINTF("Arrow Down   -> Plane Divisions --                   \n");
    PRINTF("Right        -> Cubes Per Axis  ++                   \n");
    PRINTF("Left         -> Cubes Per Axis  --                   \n");
    PRINTF("G button     -> toggle controller mode (0,1,2,3)     \n");
    PRINTF("START button -> reset parameters                     \n");
}

static void PrintController1Instructions(void)
{

    PRINTF("----------------------------------------------------------  \n");

    PRINTF("stick        -> translate objects (%s)               \n",
	   UseGlobalTransforms ? "Enabled" : "Disabled");

    PRINTF("Up Pad       -> move eye in                          \n");
    PRINTF("Down Pad     -> move eye out                         \n");
    PRINTF("A Button     -> toggle primcolor/Gouraud/lighting    \n");
    PRINTF("B Button     -> toggle texture off/on/mipmap         \n");

    PRINTF("Arrow Right  -> positive rotation about Y axis (%s)  \n",
	   UseGlobalTransforms ? "Enabled" : "Disabled");

    PRINTF("Arrow Left   -> negative rotation about Y axis (%s)  \n",
	   UseGlobalTransforms ? "Enabled" : "Disabled");

    PRINTF("Arrow Up     -> positive rotation about X axis (%s)  \n",
	   UseGlobalTransforms ? "Enabled" : "Disabled");

    PRINTF("Arrow Down   -> negative rotation about X axis (%s)  \n",
	   UseGlobalTransforms ? "Enabled" : "Disabled");

    PRINTF("Right        -> clip ratio ++                        \n");
    PRINTF("Left         -> clip ratio --                        \n");
    PRINTF("G button     -> toggle controller mode (0,1,2,3)     \n");
    PRINTF("START button -> reset parameters                     \n");
}

static void PrintController2Instructions(void)
{
    PRINTF("----------------------------------------------------------  \n");

    PRINTF("Arrow Up     -> Xpn1 screen width +4                 \n");
    PRINTF("Arrow Down   -> Xpn1 screen width -4                 \n");
    PRINTF("Down Pad     -> increment first screen scanline      \n");
    PRINTF("Up Pad       -> decrement first screen scanline      \n");
    PRINTF("Left Pad     -> decrement first scanline pixel       \n");
    PRINTF("Right Pad    -> increment first scanline pixel       \n");
    PRINTF("A button     -> toggle VI mode (Lpn1 and Xpn1)       \n");
    PRINTF("B Button     -> toggle global transformations        \n");
    PRINTF("G button     -> toggle controller mode (0,1,2,3)     \n");
    PRINTF("START button -> reset parameters                     \n");
}

static void PrintController3Instructions(void)
{
    PRINTF("----------------------------------------------------------  \n");

    PRINTF("A button     -> Toggle ucode                                \n");
    PRINTF("G button     -> toggle controller mode (0,1,2,3)            \n");
    PRINTF("Arrow Up     -> increase FIFO buffer size                   \n");
    PRINTF("Arrow Down   -> decrease FIFO buffer size                   \n");
    PRINTF("Arrow Right  -> increase FIFO buffer size fast              \n");
    PRINTF("Arrow Left   -> decrease FIFO buffer size fast              \n");
    PRINTF("Right        -> increase variation in size among cubes      \n");
    PRINTF("Left         -> decrease variation in size among cubes      \n");
    PRINTF("Pad Up       -> increase number of tasks used to render     \n");
    PRINTF("Pad Down     -> decrease number of tasks used to render     \n");
    PRINTF("START button -> reset parameters                            \n");
}

static void PrintStats(int DisplayListLength)
{
    PRINTF("%c%c%c%c%c%c\n", 27, 91, 72, 27, 91, 74); 

    PrintStatNumbers(DisplayListLength);
      
    switch(ControllerMode) {
      case 0:
	PrintController0Instructions();
	break;
      case 1:
	PrintController1Instructions();
	break;
      case 2:
	PrintController2Instructions();
	break;
      case 3:
      default:
	PrintController3Instructions();
	break;
    }

    ControllerInput = 0;
}

#endif


static void doPoly(void *arg)
{
    /* Main game loop */
  
    ActiveController = initControllers();
    
    osThreadProfileInit();               /* <== Added for profiler  */
    osThreadProfileStart();              /* <== Added for profiler */

    while (1) 
      {
	  int DisplayListLength=0, i;

	  osContStartReadData(&controllerMsgQ);
      
	  ReadController();

	  UpdateViMode(); 

	  if (CreateNewCubes)
	    {
		CreateSubCubes();
		CreateNewCubes = 0;
	    }

	  if (CreateNewPlanes)
	    {
		CreateSubPlanes();
		CreateNewPlanes = 0;
	    }


	  /* pointers to build the display list */
	  glistp = dynamicp->glist;
	  InitDisplayLists();

	  if (UseZMode) ClearZBuffer(); 
	  ClearFrameBuffer();      

	  if (numtasks == 1) {
	      doPolyFuncs(1); 
	      CleanupAndSendDisplayList();	

	      DisplayListLength = glistp - dynamicp->glist;

	      TimePerFrame[0]  = 1000.0 * (float) (t2 - t1) /((OSTime) (TicksPerSec));
	      TimePerFrameSp[0]= 1000.0 * (float) (tm - t1) /((OSTime) (TicksPerSec));
	      TimePerFrameDp[0]= 1000.0 * (float) (t2 - tm2)/((OSTime) (TicksPerSec));

	  } else {		/* use more than 1 task */
	      TimePerFrame[0]   = 0.0;
	      TimePerFrameSp[0] = 0.0;
	      TimePerFrameDp[0] = 0.0;
	      /* pointers to build the display list */
	      for (i=1; i<=numtasks; i++) {
		  if (i>1) {
		      gDPPipeSync(glistp++);
		      InitDisplayLists();
		  }
		  doPolyFuncs(i); 
		  CleanupDisplayList(i);	
	      }

	      DisplayListLength = glistp - dynamicp->glist;

	      SendDisplayLists();

	  }
 
	  /* Now write out timer bar */

	  /* pointers to build the display list */
	  glistp = dynamicp->glist;
	  InitDisplayLists();
	  WriteTimerBar();
	  CleanupAndSendDisplayList();		

	  if (ControllerInput && !Quiet)
#ifdef DEBUG
	    {
		PrintStats(DisplayListLength);

		/*
		   Profile values can be read without executing osThreadProfileStop. If threads are switched when values are read one after another, values may be changed. So execution of profiler is stopped at the moment here.
		    		 */
		osThreadProfileStop();                   /* <== Added for profiler(from here) */
		{
		    int i;
		    u64 temp_time;
		    osSyncPrintf("-------------------\n");
		    for(i=0;i<THPROF_IDMAX;i++){
			if (temp_time = osThreadProfileReadTime(i)){
			    osSyncPrintf("id = %3d , count = %8d , time = %12lluusec\n",i,
					 osThreadProfileReadCount(i),OS_CYCLES_TO_USEC(temp_time));
			}
		    }
		    osSyncPrintf("-------------------\n");
		}
		osThreadProfileStart();                 /* <== Added for profiler (up to here)*/ 
	    }
#else
	  ;
#endif
      
	  SwapViBuffer();
      }
}

static void ComputeClockSpeed(void)
{
    u32 count0, count1;

    osRecvMesg(&retraceMessageQ, 0, OS_MESG_BLOCK);
    count0 = osGetCount();
    osRecvMesg(&retraceMessageQ, 0, OS_MESG_BLOCK);
    count1 = osGetCount();

    TicksPerSec = ((osTvType == 0) ? 50.0 : 60.0) * (count1 - count0);
}


static void mainproc(void *arg)
{
    int i;

    for (i=0; i<MAXTASKS; i++) {
	tlistp[i] = &tlist[i];
	tlistp[i]->t.type			= M_GFXTASK;
	tlistp[i]->t.flags			= OS_TASK_DP_WAIT;
	tlistp[i]->t.ucode_size		= SP_UCODE_SIZE;
	tlistp[i]->t.ucode_data_size	= SP_UCODE_DATA_SIZE;
	tlistp[i]->t.dram_stack		= 
	  (u64 *) (((int) &(dram_stack[0]) +0xf) & 0xfffffff0);
	tlistp[i]->t.dram_stack_size	= SP_DRAM_STACK_SIZE8;
	tlistp[i]->t.output_buff		= (u64 *) RDP_OUTPUT_BUF;
	tlistp[i]->t.output_buff_size	= &rdp_output[RDP_OUTPUT_LEN/sizeof(u64)];
    }
    dynamicp = &dynamic;
    cfb_ptrs[0] = cfb_16_a; 
    cfb_ptrs[1] = cfb_16_b; 

    CreateMessageQueues();
    InitCubeRotations();

    ComputeClockSpeed();

    doPoly( NULL );
}
