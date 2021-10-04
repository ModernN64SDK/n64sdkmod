/*---------------------------------------------------------------------
	Copyright (C) 1997, Nintendo.
	
	File		main.c
	Coded    by	Yoshitaka Yasumoto.	Feb  6, 1997.
	Modified by	
	Comments	
	
	$Id: main.c,v 1.24 1998/12/24 17:21:45 n64os Exp $
  ---------------------------------------------------------------------*/
#include	<ultra64.h>
#include	<PR/gs2dex.h>
#include	"system.h"
#include	"texture.h"
#include	"action.h"
#include	"uc_assert.h"
#include	"sprite.h"

extern	Gfx	rdpInit_dl[];
extern	Gfx	clearCfb_dl[];
extern	Gfx	spriteInit_dl[];

void	setBg(void);
void	setObject(void);
void    loadSegment(u32, u32);

/*---------------------------------------------------------------------*
 *	Settings when outputting the DL process log
 *--------------------------------------------------------------------*/
# define	S2DEX_UCODE(x)	gspS2DEX_fifo##x##Start
# define	GfxLog		0

/*---------------------------------------------------------------------*
 * Task data definitions
 *--------------------------------------------------------------------*/
OSTask	tlist = {
  M_GFXTASK,					/* task type                */
  OS_TASK_DP_WAIT|OS_TASK_LOADABLE,		/* task flags               */
  (u64 *)&rspbootTextStart,			/* boot ucode ptr           */
  SP_BOOT_UCODE_SIZE,				/* boot ucode size          */
  (u64 *)&S2DEX_UCODE(Text),			/* ucode ptr                */
  SP_UCODE_SIZE,			      	/* ucode size               */
  (u64 *)&S2DEX_UCODE(Data),			/* ucode data ptr           */
  SP_UCODE_DATA_SIZE,				/* ucode data size          */
  NULL,						/* dram stack    (Not used) */
  0,					      	/* dram stack size (Not used) */
  (u64 *)system_rdpfifo,			/* fifo buffer top            */
  (u64 *)system_rdpfifo+RDPFIFO_SIZE,	/* fifo buffer bottom         */
  NULL,						/* data ptr   (set later)     */
  (u32)GfxLog,	/* Specify dl log ptr for S2DEX.  data size  (No need to set) */
  (u64 *)system_rspyield,			/* yield data ptr           */
  OS_YIELD_DATA_SIZE,				/* yield data size          */
};

/*---------------------------------------------------------------------*
 * Graphics list area
 *--------------------------------------------------------------------*/
Gfx	glist[2][GLIST_LEN];

/*---------------------------------------------------------------------*
 * Render mode parameter table
 *--------------------------------------------------------------------*/
static	struct	{
  u32	antiAlias;
  u32	txtrFilter;
  u32	cycleType;
  u32	objRMode;
} RMmodeTable[] = {
{ 0,G_TF_POINT, G_CYC_1CYCLE,0 },                      /* NonAA+Point+1cycle */
{ 0,G_TF_POINT, G_CYC_COPY,  0 },                      /* NonAA+Point+Copy   */
{ 0,G_TF_BILERP,G_CYC_1CYCLE,G_OBJRM_BILERP},          /* NonAA+Bilerp       */
{ 1,G_TF_BILERP,G_CYC_1CYCLE,G_OBJRM_BILERP}	       /* AA+Bilerp          */
};

u32	ShrinkTable[] = { 0, G_OBJRM_SHRINKSIZE_1, G_OBJRM_SHRINKSIZE_2 };
u32	bg16seg, bg8seg;
OSPiHandle      *handler;

/*---------------------------------------------------------------------*
 * M A I N 
 *--------------------------------------------------------------------*/
void	Main(void *arg)
{
  u8	draw_frame = 0;
  u32	objRM;
  Gfx	*gp, *gtop;
  OSTime rspstart;
  u32	 rsptime, rdptime;
  
  bg16seg = (u32)_codeSegmentBssEnd 
          + (u32)_staticSegmentEnd - (u32)_staticSegmentStart;
  bg8seg  = bg16seg + 
          + (u32)_bg_rgbaSegmentRomEnd - (u32)_bg_rgbaSegmentRomStart;

  handler = osCartRomInit();  
  loadSegment(bg16seg, bg8seg);

#ifdef DEBUG
  menuInit();
#endif

  actionInit();
  rsptime = rdptime = 0;
  
  while(1){

    /*------ Controller start to read ------*/
    osContStartReadData(&siMessageQ);
    
    /*------ Retrace wait ------*/
    osRecvMesg(&retraceMessageQ, NULL, OS_MESG_BLOCK);

    /*------ Set Bg structure ------*/
    setBg();
    
    /*------ Set Object structure ------*/
    setObject();
    
    /*------ Create Gfx list ------*/
    gtop = gp = glist[draw_frame];

    /*------ RSP initial settings ------*/
    gSPSegment(gp ++, 0, 0x0);
    gSPSegment(gp ++, STATIC_SEGMENT, _codeSegmentBssEnd);
    if (aMenu[MENU_BG_TX_FORMAT]){
      gSPSegment(gp ++, BG_SEGMENT, bg8seg);
    }
    gDPSetColorImage(gp ++, G_IM_FMT_RGBA,
		     G_IM_SIZ_16b, SCREEN_WD, system_cfb[draw_frame]);
    gSPDisplayList(gp ++, rdpInit_dl);
    gSPDisplayList(gp ++, clearCfb_dl);
    gSPDisplayList(gp ++, spriteInit_dl);

    /*------ Bg output ------*/
    if (aMenu[MENU_BG_TX_FORMAT]){
      gDPSetTextureLUT(gp ++, G_TT_RGBA16);
      gSPObjLoadTxtr(gp ++, &objBgTLUT);
    }
    if (aMenu[MENU_BG_SCALABLE] == 0){

      /* Unscalable BG plane */
      gDPSetRenderMode(gp ++, G_RM_NOOP, G_RM_NOOP2);
      gDPSetCycleType(gp ++, G_CYC_COPY);
      gSPBgRectCopy(gp ++, &objBg);

    } else {
      /* Scalable BG plane */
      gDPSetRenderMode(gp ++, G_RM_SPRITE, G_RM_SPRITE2);
      gDPSetCycleType(gp ++, G_CYC_1CYCLE);
      gDPSetTextureFilter(gp ++,
			  RMmodeTable[aMenu[MENU_RENDERMODE]].txtrFilter);
      if (aMenu[MENU_BG_SCALABLE] == 1){
	/* Emulated by CPU */
	guS2DEmuSetScissor(0, 0, SCREEN_WD, SCREEN_HT, 
			   (RMmodeTable[aMenu[MENU_RENDERMODE]].txtrFilter
			    == G_TF_BILERP));
	guS2DEmuBgRect1Cyc(&gp, &objBg);
      } else {
	/* GBI command */
	gSPObjRenderMode(gp ++, RMmodeTable[aMenu[MENU_RENDERMODE]].objRMode);
	gSPBgRect1Cyc(gp ++, &objBg);
      }
    }
    gDPPipeSync(gp ++);

    /*------ Render Mode settings ------*/
    objRM = RMmodeTable[aMenu[MENU_RENDERMODE]].objRMode;
    if (RMmodeTable[aMenu[MENU_RENDERMODE]].cycleType != G_CYC_COPY){
      if (!aMenu[MENU_RENDERMODE_2]){
	/* Opaque */
	if (RMmodeTable[aMenu[MENU_RENDERMODE]].antiAlias){
	  gDPSetRenderMode(gp ++, G_RM_AA_SPRITE, G_RM_AA_SPRITE2);
	} else {
	  gDPSetRenderMode(gp ++, G_RM_SPRITE, G_RM_SPRITE2);
	}
      } else {
	/* Translucent */
	if (RMmodeTable[aMenu[MENU_RENDERMODE]].antiAlias){
	  gDPSetRenderMode(gp ++, G_RM_AA_XLU_SPRITE, G_RM_AA_XLU_SPRITE2);
	} else {
	  gDPSetRenderMode(gp ++, G_RM_XLU_SPRITE, G_RM_XLU_SPRITE2);
	}
	gDPSetCombineMode(gp ++, G_CC_MODULATEIA_PRIM, G_CC_MODULATEIA_PRIM);
	gDPSetPrimColor(gp ++, 0,0,255,255,255,128);
      }
    }
    
    /*------ Set Texture Filter and CycleType ------*/
    gDPSetTextureFilter(gp ++, RMmodeTable[aMenu[MENU_RENDERMODE]].txtrFilter);
    gDPSetCycleType    (gp ++, RMmodeTable[aMenu[MENU_RENDERMODE]].cycleType );
    
    /*------ Set Texture Window ------*/
    if (aMenu[MENU_OBJ_TX_WINDOW]) objRM |= G_OBJRM_NOTXCLAMP;

    /*------ Set Shrink ------*/
    objRM |= ShrinkTable[aMenu[MENU_OBJ_SHRINK]];
    
    /*------ Set Object Render Mode ------*/    
    gSPObjRenderMode(gp ++, objRM);
    
    /*------ Texture load settings ------*/
    if (!aMenu[MENU_OBJ_TX_TYPE]){
      gDPSetTextureLUT(gp ++, G_TT_NONE);
      gSPObjLoadTxtr(gp ++, (aMenu[MENU_OBJ_TX_LOAD_BY]
			     ? &objTxtrTile_RGBA16 : &objTxtrBlock_RGBA16));
    } else {
      gDPSetTextureLUT(gp ++, G_TT_RGBA16);
      gSPObjLoadTxtr(gp ++, (aMenu[MENU_OBJ_TX_LOAD_BY]
			     ? &objTxtrTile_CI4 : &objTxtrBlock_CI4));
      gSPObjLoadTxtr(gp ++, &objTLUT_CI4);
    }
    
    /*------ Output Object:Rectangle1 ------*/
    gSPObjRectangle(gp ++, &(objRect[0]));

    if (RMmodeTable[aMenu[MENU_RENDERMODE]].cycleType != G_CYC_COPY){

      /*------ Output Object:Rectangle2 ------*/
      gSPObjMatrix(gp ++, &(objMtx[0]));
      gSPObjSprite(gp ++, &(objRect[1]));

      /*------ Output Object:Ball ------*/      
      /* 
	 Ball is displayed by combining two sprites.  For this reason, 
       the processing procedure must be changed in accordance with 
       the Texture Filter settings.

	 When Texture Filter is PointSample, SHRINKSIZE does not need to
	 be specified.  But when Texture Filter is Bilerp, SHRINKSIZE_1 
       must be specified.  When SHRINKSIZE_1 is specified, a 0.5 texel area
       around the Sprite is clamped.  The area excluded by this clamp is not
       effected by the Bilerp process.

	 In Bilerp, the part where Sprites meet needs to be loaded twice, so   
	 for Ball only 64x63 Texels are rendered.  It is important to understand
       the difference between objBall[1] and objBall[2].
      */
      if (!aMenu[MENU_RENDERMODE_2]){
	/* Render one-size larger to hide the joint.  (Only when 
	   opaque)  Unnecessary since S2DEX 1.05.  */
	/* objRM |= G_OBJRM_WIDEN; */
      }
      gDPPipeSync(gp ++);
      gDPSetTextureLUT(gp ++, G_TT_RGBA16);
      gSPObjLoadTxtr(gp ++, &objBallTLUT);
      if (RMmodeTable[aMenu[MENU_RENDERMODE]].txtrFilter == G_TF_POINT){
	gSPObjRenderMode(gp ++, objRM);
	gSPObjMatrix(gp ++, &(objMtx[2]));
	gSPObjLoadTxRectR(gp ++, &(objBall[0]));
	gSPObjLoadTxRectR(gp ++, &(objBall[1]));
	gSPObjMatrix(gp ++, &(objMtx[1]));
	gSPObjLoadTxSprite(gp ++, &(objBall[0]));
	gSPObjLoadTxSprite(gp ++, &(objBall[1]));
      } else {
	gSPObjRenderMode(gp ++, objRM|G_OBJRM_SHRINKSIZE_1);
	gSPObjMatrix(gp ++, &(objMtx[2]));
	gSPObjLoadTxRectR(gp ++, &(objBall[0]));
	gSPObjLoadTxRectR(gp ++, &(objBall[2]));
	gSPObjMatrix(gp ++, &(objMtx[1]));
	gSPObjLoadTxSprite(gp ++, &(objBall[0]));
	gSPObjLoadTxSprite(gp ++, &(objBall[2]));
      }
    }
    
    /*------ Output the process meter ------*/
    if (rsptime){
      gDPPipeSync(gp ++);
      gDPSetCycleType(gp ++, G_CYC_FILL);
      gDPSetRenderMode(gp ++, G_RM_OPA_SURF, G_RM_OPA_SURF2);
      gDPSetFillColor(gp ++, (GPACK_RGBA5551(128,128,255,1) << 16 | 
			      GPACK_RGBA5551(128,128,255,1)));
      gDPFillRectangle(gp ++, 30, 201, 30+rsptime/100, 202);
      gDPFillRectangle(gp ++, 30, 205, 30+rdptime/100, 206);
      gDPSetFillColor(gp ++, (GPACK_RGBA5551(255,255,255,1) << 16 | 
			      GPACK_RGBA5551(255,255,255,1)));
      gDPFillRectangle(gp ++, 30,     200, 30,     207);
      gDPFillRectangle(gp ++, 30+166, 200, 30+166, 207);
    }
    gDPFullSync(gp ++);
    gSPEndDisplayList(gp ++);
    
    /*------ Execute Gfx task ------*/
    tlist.t.data_ptr = (u64 *)gtop;
    osWritebackDCache(gtop, ((u32)gp)-((u32)gtop));
    rspstart = osGetTime();
    osSpTaskStart(&tlist);
    
    /*------ Wait for end ------*/
    osRecvMesg(&rspMessageQ, NULL, OS_MESG_BLOCK);
    rsptime = OS_CYCLES_TO_NSEC(osGetTime() - rspstart) / 1000;

#if 0
    /*------ Output DEBUG information ------*/
    if (Ac.pad[0].push & L_TRIG){
      ucDebugRdpFifoPrint(&tlist); 	/* Outputs the RDP fifo buffer    */
      ucDebugInfoPrint();		/* Outputs the work area for debugging  */
    }
#endif

    osRecvMesg(&rdpMessageQ, NULL, OS_MESG_BLOCK);
    rdptime = OS_CYCLES_TO_NSEC(osGetTime() - rspstart) / 1000;
    
    /* Swap frames */
    osViSwapBuffer(system_cfb[draw_frame]);
    draw_frame ^= 1;

    /* Receive controller data */
    osRecvMesg(&siMessageQ, NULL, OS_MESG_BLOCK);
    osContGetReadData(contPad);
    
    /* Input process */
    actionUpdate();
  }
}

/*---------------------------------------------------------------------*
 * BG structure settings
 *--------------------------------------------------------------------*/
void	setBg(void)
{
  /* BG structure settings */
  objBg.b.imageX = Ac.imageX;
  objBg.b.imageY = Ac.imageY;
  objBg.b.frameX = Ac.frameX;
  objBg.b.frameY = Ac.frameY;
  objBg.b.frameW = Ac.frameW;
  objBg.b.frameH = Ac.frameH; 
  objBg.b.imageFlip = (aMenu[MENU_BG_FLIPS] ? G_BG_FLAG_FLIPS : 0);
  objBg.b.imageLoad =
    (aMenu[MENU_BG_TX_LOAD_BY] ? G_BGLT_LOADTILE : G_BGLT_LOADBLOCK);
  if (aMenu[MENU_BG_TX_FORMAT]){
    objBg.b.imagePtr = (u64 *)L2_TV;
    objBg.b.imageFmt = G_IM_FMT_CI;
    objBg.b.imageSiz = G_IM_SIZ_8b;
  } else {
    objBg.b.imagePtr = (u64 *)bg16seg;
    objBg.b.imageFmt = G_IM_FMT_RGBA;
    objBg.b.imageSiz = G_IM_SIZ_16b;
  }

  if (!aMenu[MENU_BG_SCALABLE]){
    /*
     *	guS2DInitBg is called once at the time of initialization, 
     *	and after that it only needs to be called again when either
     *	imageFmt, imageSiz, imageLoad, imageW or frameW is changed.     
     *	Here it is called every time because imageLoad can change.  
     */
    guS2DInitBg(&objBg);
  } else {
    objBg.s.imageYorig = Ac.imageYorig;
    objBg.s.scaleW     = Ac.frameScaleW;
    objBg.s.scaleH     = Ac.frameScaleH; 
  }
  osWritebackDCache(&objBg, sizeof(uObjBg));

  return;
}


/*---------------------------------------------------------------------*
 * Object structure settings
 *--------------------------------------------------------------------*/
void	setObject(void)
{
  s32	Sin, Cos;
  u8	flags = ((aMenu[MENU_OBJ_FLIPS] ? G_OBJ_FLAG_FLIPS : 0)|
		 (aMenu[MENU_OBJ_FLIPT] ? G_OBJ_FLAG_FLIPT : 0));

  objRect[0].s.objX   = Ac.objX;
  objRect[0].s.objY   = Ac.objY;
  objRect[0].s.scaleW = Ac.scaleW;
  objRect[0].s.scaleH = Ac.scaleH;
  objRect[0].s.imageFlags = flags;
  
  objMtx[0].m.X = objMtx[1].m.X = Ac.objX + (64<<2);
  objMtx[0].m.Y = objMtx[1].m.Y = Ac.objY;
  objRect[1].s.scaleW = Ac.scaleW;
  objRect[1].s.scaleH = Ac.scaleH;
  objRect[1].s.imageFlags = flags;
  
  Cos = (s32)coss(Ac.theta)*2;
  Sin = (s32)sins(Ac.theta)*2;
  if (Cos == 0xfffe) Cos = 0x10000;
  if (Sin == 0xfffe) Sin = 0x10000;
  if (Cos ==-0xfffe) Cos =-0x10000;
  if (Sin ==-0xfffe) Sin =-0x10000;
  objMtx[0].m.A =    objMtx[0].m.D = Cos;
  objMtx[0].m.B = - (objMtx[0].m.C = Sin);
  
  objMtx[1].m.A =  Cos * 1024 / Ac.scaleW;
  objMtx[1].m.B = -Sin * 1024 / Ac.scaleH;
  objMtx[1].m.C =  Sin * 1024 / Ac.scaleW;
  objMtx[1].m.D =  Cos * 1024 / Ac.scaleH;  
  
  objMtx[2].m.X = Ac.objX;
  objMtx[2].m.Y = Ac.objY;
  objMtx[2].m.BaseScaleX = Ac.scaleW;
  objMtx[2].m.BaseScaleY = Ac.scaleH;

  if (!aMenu[MENU_OBJ_TX_TYPE]){
    objRect[0].s.imageFmt = objRect[1].s.imageFmt = G_IM_FMT_RGBA;
    objRect[0].s.imageSiz = objRect[1].s.imageSiz = G_IM_SIZ_16b;
    objRect[0].s.imageStride =
      objRect[1].s.imageStride = GS_PIX2TMEM(32, G_IM_SIZ_16b);
  } else {
    objRect[0].s.imageFmt = objRect[1].s.imageFmt = G_IM_FMT_CI;
    objRect[0].s.imageSiz = objRect[1].s.imageSiz = G_IM_SIZ_4b;
    objRect[0].s.imageStride =
      objRect[1].s.imageStride = GS_PIX2TMEM(32, G_IM_SIZ_4b);
  }
  
  osWritebackDCache(objRect, sizeof(uObjSprite)*2);
  osWritebackDCache(objMtx,  sizeof(uObjMtx)*3);
  
  return;
}

/*---------------------------------------------------------------------*
 * Load segment DMA
 *--------------------------------------------------------------------*/
void	loadSegment(u32 bg16seg, u32 bg8seg)
{
  /* Load Static segment */
  dmaIOMessageBuf.hdr.pri      = OS_MESG_PRI_NORMAL;
  dmaIOMessageBuf.hdr.retQueue = &dmaMessageQ;
  dmaIOMessageBuf.dramAddr     = _codeSegmentBssEnd;
  dmaIOMessageBuf.devAddr      = (u32)_staticSegmentRomStart;
  dmaIOMessageBuf.size         = (u32)_staticSegmentRomEnd-(u32)_staticSegmentRomStart;
  osEPiStartDma(handler, &dmaIOMessageBuf, OS_READ);
  osRecvMesg(&dmaMessageQ, NULL, OS_MESG_BLOCK);  
  
  /* Load Bg RGBA segment */
  dmaIOMessageBuf.dramAddr     = (char *)bg16seg;
  dmaIOMessageBuf.devAddr      = (u32)_bg_rgbaSegmentRomStart;
  dmaIOMessageBuf.size         = (u32)_bg_rgbaSegmentRomEnd - (u32)_bg_rgbaSegmentRomStart;
  osEPiStartDma(handler, &dmaIOMessageBuf, OS_READ);
  osRecvMesg(&dmaMessageQ, NULL, OS_MESG_BLOCK);  
  
  /* Load Bg CI segment */
  dmaIOMessageBuf.dramAddr     = (char *)bg8seg;
  dmaIOMessageBuf.devAddr      = (u32)_bg_ciSegmentRomStart;
  dmaIOMessageBuf.size         = (u32)_bg_ciSegmentRomEnd-(u32)_bg_ciSegmentRomStart;
  osEPiStartDma(handler, &dmaIOMessageBuf, OS_READ);
  osRecvMesg(&dmaMessageQ, NULL, OS_MESG_BLOCK);  
}

/*======== End of main.c ========*/

