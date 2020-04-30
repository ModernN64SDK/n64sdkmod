/*******************************************************************
 *
 *  gfx7.c
 *    Z buffer tutorial 
 *
 *      This is a sample of the Z buffer, the most important subject
 *      related to the blender.  It is also a demonstration of the 
 *      method for defining multiple objects from one set of model 
 *      data and for using matrix stacks. 
 *
 *      In addition, as samples of the VI mode and special VI 
 *      function settings, you can change the resolution and the 
 *      frame buffer size (16 bit, 32 bit), and you can switch 
 *      gamma correction ON and OFF. 
 *
 *******************************************************************/

#include  <nusys.h>

#include  "define.h"

/************************** Customize ***************************/

/*
 *  Note: When __HIRES__ and __32BIT__ are both defined, the 
 *        definition of __HIRES__ takes priority. 
 *        If neither of them is defined, drawing will be 
 *        done in low-resolution 16-bit mode. 
 */

/*
 *  When __Z_BUFFER__ is defined, rendering is done using the Z buffer. 
 */
#undef    __Z_BUFFER__

/*
 *  When __HIRES__ is defined, operation switches to high-resolution mode.
 */
#undef    __HIRES__

/*
 *  When __32BIT__ is defined 32-bit CFB is used. 
 */
#undef    __32BIT__

/*
 *  When __GAMMA__ is changed, it is reflected in the special VI 
 *  function settings. 
 *  Define this as either OS_VI_GAMMA_ON or OS_VI_GAMMA_OFF. 
 *    Note: This macro has no effect unless __HIRES__ or __32BIT__ 
 *          is defined.  If neither of these is defined, the default
 *          NuSystem setting is gamma OFF. 
 */
#define    __GAMMA__        OS_VI_GAMMA_ON

/*
 *  When __SHOW_COVERAGE__ is defined the coverage is made visible. 
 */
#undef    __SHOW_COVERAGE__

/* Change screen height and width */
#if        defined(__HIRES__)
#undef      SCREEN_WD
#define     SCREEN_WD     640
#undef      SCREEN_HT
#define     SCREEN_HT     480
#endif /*  defined(__HIRES__) */

/* Texture data (array format) */
#include    "circle.h"

/********************* Static Variables & Prototypes **********************/

/* Pointer array to color frame buffers */
static u16 *  cfbPtr[2] = {
#if        defined(__32BIT__) && !defined(__HIRES__)
  (u16 *)(0x80400000 - (SCREEN_WD * SCREEN_HT * sizeof(u32) * 2)),
  (u16 *)(0x80400000 - (SCREEN_WD * SCREEN_HT * sizeof(u32) * 1)),
#else  /* !defined(__32BIT__) ||  defined(__HIRES__) */
  (u16 *)(0x80400000 - (SCREEN_WD * SCREEN_HT * sizeof(u16) * 2)),
  (u16 *)(0x80400000 - (SCREEN_WD * SCREEN_HT * sizeof(u16) * 1)),
#endif /*  defined(__32BIT__) && !defined(__HIRES__) */
};

/* Pointer to Z buffer */
static u16 *  zbufPtr = (u16 *)0x80200000;

/* Viewpoint information */
static  Vp  viewport = {
  SCREEN_WD * 2, SCREEN_HT * 2, G_MAXZ / 2, 0,
  SCREEN_WD * 2, SCREEN_HT * 2, G_MAXZ / 2, 0,
};

/* RSP intialization display list */
static Gfx rspinit_dl[] = {
  gsSPViewport(&viewport),
  gsSPClearGeometryMode(G_SHADE | G_SHADING_SMOOTH | G_CULL_BOTH |
                        G_FOG | G_LIGHTING | G_TEXTURE_GEN |
                        G_TEXTURE_GEN_LINEAR | G_LOD),
  gsSPTexture(0, 0, 0, 0, G_OFF),
  gsSPEndDisplayList(),
};

/* RDP intialization display list */
static Gfx rdpinit_dl[] = {
  gsDPSetCycleType(G_CYC_1CYCLE),
  gsDPPipelineMode(G_PM_1PRIMITIVE),
  gsDPSetScissor(G_SC_NON_INTERLACE, 0, 0, SCREEN_WD, SCREEN_HT),
  gsDPSetCombineMode(G_CC_PRIMITIVE, G_CC_PRIMITIVE),
  gsDPSetCombineKey(G_CK_NONE),
  gsDPSetAlphaCompare(G_AC_NONE),
  gsDPSetRenderMode(G_RM_NOOP, G_RM_NOOP2),
  gsDPSetColorDither(G_CD_DISABLE),
  gsDPSetAlphaDither(G_AD_DISABLE),
  gsDPSetTextureFilter(G_TF_POINT),
  gsDPSetTextureConvert(G_TC_FILT),
  gsDPSetTexturePersp(G_TP_NONE),
  gsDPPipeSync(),
  gsSPEndDisplayList(),
};

/* Display list */
static Gfx    glist[GLIST_LENGTH];
static Gfx *  glistp;

/* Projection conversion matrix */
static Mtx    projection;
/* Common model conversion matrix */
static Mtx    common1, common2;
/* Individual model conversion matrix */
static Mtx    modellocal[3];

/* Scale to normalize same-dimension coordinates */
static u16    perspNorm;

/* Function prototypes */
static void   ClearBackground(u8 r, u8 g, u8 b);
static void   DrawCube(int index);
static void   SetupResolution(void);

/*
 *  Functions responsible for drawing
 */
void  Draw(void)
{
  static int    theta = 0;                  /* Rotational angle of model */
  static u8     setupRes = 1;               /* Resolution setting on first call */
  float         fmat1[4][4], fmat2[4][4];   /* Structure to temporarily hold the conversion matrix */
  int           i;
  
  if(setupRes)
  {
    SetupResolution();
    setupRes = 0;
  }
  
  /* Function to create projection conversion matrix */
  guPerspective(&projection,
                &perspNorm,
                45.0,                                 /* Perspective angle */
                (float)SCREEN_WD / (float)SCREEN_HT,  /* Aspect ratio */
                10.0, 100.0,                          /* Distance to clip plane */
                1.0);
  
  /* Matrix to convert cube to rod model */
  guScaleF(fmat1, 1.5, 0.1, 0.1);
  guTranslateF(fmat2, 10.0, 0.0, 0.0);
  guMtxCatF(fmat1, fmat2, fmat1);
  guRotateF(fmat2, 6.875, 0.0, 1.0, 0.0);
  guMtxCatF(fmat1, fmat2, fmat1);
  guTranslateF(fmat2, -10.0, -5.7735, 0.0);
  guMtxCatF(fmat1, fmat2, fmat1);
  guMtxF2L(fmat1, &common2);
  
  /* Conversion matrix for situating rod model in triangle format */
  guMtxIdent(&modellocal[0]);
  guRotate(&modellocal[1], 120.0, 0.0, 0.0, 1.0);
  guRotate(&modellocal[2], 240.0, 0.0, 0.0, 1.0);
  
  /* Conversion matrix to rotate model, then shift it toward back */
  guRotateF(fmat1, theta, 1.0, 1.0, 0.5);
  guTranslateF(fmat2, 0.0, 0.0, -50.0);
  guMtxCatF(fmat1, fmat2, fmat1);
  guMtxF2L(fmat1, &common1);
  
  /* Set start address of display list */
  glistp = glist;
  
  /*
   *  Initialize RCP 
   */
  /* Hand KSEG0 address to RSP */
  gSPSegment(glistp++, 0, 0);
  gSPDisplayList(glistp++, rspinit_dl);
  gSPDisplayList(glistp++, rdpinit_dl);
  
  /* Insert display list command to clear background */
  ClearBackground(50, 50, 50);
  
  /* Add display list command to draw cube */
  for(i = 0; i < 3; i++)
    DrawCube(i);
  
  /* End display list creation */
  gDPFullSync(glistp++);
  gSPEndDisplayList(glistp++);
  
  /* Write matrix back from cache to memory */
  osWritebackDCache(&projection, sizeof(projection));
  osWritebackDCache(&common1, sizeof(common1));
  osWritebackDCache(&common2, sizeof(common2));
  osWritebackDCache(modellocal, sizeof(modellocal));
  
  /* Start task */
  nuGfxTaskStart(glist,
                 (s32)(glistp - glist) * sizeof(Gfx),
                 NU_GFX_UCODE_F3DEX, NU_SC_SWAPBUFFER);
  
  /* On one rotation, return angle to 0 */
  if(++theta >= 360)
    theta = 0;
}

/*
 * Fill background with specified color (add display list command) 
 */
void  ClearBackground(u8 r, u8 g, u8 b)
{
#if        defined(__Z_BUFFER__)
  gDPPipeSync(glistp++);
  gDPSetCycleType(glistp++, G_CYC_FILL);
  /* Set Z buffer */
  gDPSetDepthImage(glistp++, nuGfxZBuffer);
  /* Specify fill object (Z buffer) */
  gDPSetColorImage(glistp++, G_IM_FMT_RGBA, G_IM_SIZ_16b, SCREEN_WD, nuGfxZBuffer);
  /* Specify fill color (Z value of background, maximum Z value) */
  gDPSetFillColor(glistp++, (GPACK_ZDZ(G_MAXFBZ, 0) << 16 | GPACK_ZDZ(G_MAXFBZ, 0)));
  /* Clear Z buffer */
  gDPFillRectangle(glistp++, 0, 0, SCREEN_WD - 1, SCREEN_HT - 1);
#endif /*  defined(__Z_BUFFER__) */
  
  gDPPipeSync(glistp++);
  /* Set fill mode to (G_CYC_FILL) */
  gDPSetCycleType(glistp++, G_CYC_FILL);
#if        defined(__32BIT__) && !defined(__HIRES__)
  /* Specify fill object (frame buffer to be displayed next) */
  gDPSetColorImage(glistp++, G_IM_FMT_RGBA, G_IM_SIZ_32b, SCREEN_WD, nuGfxCfb_ptr);
  /* Specify fill color (background color) */
  gDPSetFillColor(glistp++,
                  (r << 24) | (g << 16) | (b << 8) | 255);
#else  /* !defined(__32BIT__) ||  defined(__HIRES__) */
  /* Specify fill object (frame buffer to be displayed next) */
  gDPSetColorImage(glistp++, G_IM_FMT_RGBA, G_IM_SIZ_16b, SCREEN_WD, nuGfxCfb_ptr);
  /* Specify fill color (background color) */
  gDPSetFillColor(glistp++,
                  (GPACK_RGBA5551(r, g, b, 1) << 16 |
                   GPACK_RGBA5551(r, g, b, 1)));
#endif /*  defined(__32BIT__) && !defined(__HIRES__) */
  /* Size of rectangle to be filled (entire screen) */
  gDPFillRectangle(glistp++, 0, 0, SCREEN_WD - 1, SCREEN_HT - 1);
  gDPPipeSync(glistp++);
}

/*
 *  Cube model vertex data 
 */
static Vtx  cube_vtx[] = {
  {-10, -10, -10, 0, 31 << 6, 31 << 6, 130, 70, 50, 255},
  { 10, -10, -10, 0,  0 << 6, 31 << 6, 130, 70, 50, 255},
  { 10,  10, -10, 0,  0 << 6,  0 << 6, 100, 40, 40, 255},
  {-10,  10, -10, 0, 31 << 6,  0 << 6, 110, 40, 40, 255},
  {-10, -10,  10, 0,  0 << 6, 31 << 6, 120, 70, 50, 255},
  { 10, -10,  10, 0, 31 << 6, 31 << 6,  90, 30, 20, 255},
  { 10,  10,  10, 0, 31 << 6,  0 << 6, 100, 50, 20, 255},
  {-10,  10,  10, 0,  0 << 6,  0 << 6, 120, 60, 40, 255},
  /* Vertexes for properly texture mapping top and bottom surfaces of cube */
  {-10, -10, -10, 0,  0 << 6, 31 << 6, 130, 70, 50, 255},
  { 10, -10, -10, 0, 31 << 6, 31 << 6, 130, 70, 50, 255},
  { 10,  10, -10, 0, 31 << 6,  0 << 6, 100, 40, 40, 255},
  {-10,  10, -10, 0,  0 << 6,  0 << 6, 110, 40, 40, 255},
  {-10, -10,  10, 0,  0 << 6,  0 << 6, 120, 70, 50, 255},
  { 10, -10,  10, 0, 31 << 6,  0 << 6,  90, 30, 20, 255},
  { 10,  10,  10, 0, 31 << 6, 31 << 6, 100, 50, 20, 255},
  {-10,  10,  10, 0,  0 << 6, 31 << 6, 120, 60, 40, 255},
};

/*
 *  Function to create display list to display cube model 
 */
void  DrawCube(int index)
{
  gDPPipeSync(glistp++);
  
  /* Specify projection conversion matrix */
  gSPMatrix(glistp++,
            &projection,
            G_MTX_PROJECTION |
            G_MTX_LOAD |
            G_MTX_NOPUSH);
  /* Specify model conversion matrix */
  switch(index)
  {
  case 0:   /* Load common conversion matrix 1 */
    gSPMatrix(glistp++, &common1, G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
    break;
  case 1:
  case 2:   /* Pop common conversion matrix 1 from matrix stack */
    gSPPopMatrix(glistp++, G_MTX_MODELVIEW);
    break;
  }
  gSPMatrix(glistp++,
            &modellocal[index],
            G_MTX_MODELVIEW |
            G_MTX_MUL |
            G_MTX_PUSH);        /* Push matrix stack to use during display of next model */
  gSPMatrix(glistp++,
            &common2,
            G_MTX_MODELVIEW |
            G_MTX_MUL |
            G_MTX_NOPUSH);      /* Common conversion matrix 2 */
  gSPPerspNormalize(glistp++, &perspNorm);
  
  /*
   *  Display list to make drawing related settings 
   */
  
  /* Set RDP cycle type */
  gDPSetCycleType(glistp++, G_CYC_1CYCLE);
  
  /* Enable texture, set scaling parameters */
  gSPTexture(glistp++, 0x8000, 0x8000, 0, G_TX_RENDERTILE, G_ON);
  
#if        defined(__Z_BUFFER__)
  gSPSetGeometryMode(glistp++, G_ZBUFFER | G_SHADE | G_SHADING_SMOOTH | G_CULL_BACK);
#else  /* !defined(__Z_BUFFER__) */
  gSPSetGeometryMode(glistp++,             G_SHADE | G_SHADING_SMOOTH | G_CULL_BACK);
#endif /*  defined(__Z_BUFFER__) */
  
  /* Interpolate ambient color and texture color using shade color */
  /* This mode is the same as G_CC_BLENDI */
  gDPSetCombineLERP(glistp++, ENVIRONMENT, SHADE, TEXEL0, SHADE,
                                        0,     0,      0, SHADE,
                              ENVIRONMENT, SHADE, TEXEL0, SHADE,
                                        0,     0,      0, SHADE);
  /* Change the ambient color according to the model (this allows the entire color 
   * scheme to be changed) */
  switch(index)
  {
  case 0:
    gDPSetEnvColor(glistp++, 60, 30, 30, 255);
    break;
  case 1:
    gDPSetEnvColor(glistp++, 30, 60, 30, 255);
    break;
  case 2:
    gDPSetEnvColor(glistp++, 30, 30, 60, 255);
    break;
  }
  
  /* Load gradation texture (image circle) */
  gDPLoadTextureBlock(glistp++,
                      circle,                   /* Pointer to texture image */
                      G_IM_FMT_I,               /* Texel format */
                      G_IM_SIZ_8b,              /* Texel size */
                      32, 32,                   /* Image width and height */
                      0,                        /* LUT (palette) index (not used here) */
                      G_TX_CLAMP,               /* Clamp, wrap, mirror frag in s direction */
                      G_TX_CLAMP,               /* Clamp, wrap, mirror frag in t direction */
                      5, 5,                     /* s, t masks */
                      G_TX_NOLOD, G_TX_NOLOD);  /* Shift (not shifted here) */
  
  /* Texture perspective correction is turned on during mapping */
  gDPSetTexturePersp(glistp++, G_TP_PERSP);
  /* Set texture filter */
  gDPSetTextureFilter(glistp++, G_TF_BILERP);
  gDPSetTextureConvert(glistp++, G_TC_FILT);
  /* This can be ignored until LOD or detail texture is explained */
  gDPSetTextureLOD(glistp++, G_TL_TILE);
  gDPSetTextureDetail(glistp++, G_TD_CLAMP);
  /* Texture palette (not used) */
  gDPSetTextureLUT(glistp++, G_TT_NONE);
  
  /* Set rendering mode */
#if        defined(__Z_BUFFER__)
  /*   Opaque, antialising, Z buffer */
  gDPSetRenderMode(glistp++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
#else  /* !defined(__Z_BUFFER__) */
  /*   Opaque, antialising */
  gDPSetRenderMode(glistp++, G_RM_AA_OPA_SURF, G_RM_AA_OPA_SURF2);
#endif /*  defined(__Z_BUFFER__) */
  
  /*
   *  Display list to define model 
   */
  
  /* Load model vertexes */
  gSPVertex(glistp++, cube_vtx, 16, 0);
  
  /* Create cube from loaded vertexes */
  /* Create face 1 */
  gSP2Triangles(glistp++, 4, 6, 7, 0, 4, 5, 6, 0);
  /* Create face 2 */
  gSP2Triangles(glistp++, 6, 5, 2, 1, 2, 5, 1, 1);
  /* Create face 3 */
  gSP2Triangles(glistp++, 1, 3, 2, 0, 0, 3, 1, 2);
  /* Create face 4 */
  gSP2Triangles(glistp++, 3, 0, 7, 2, 0, 4, 7, 2);
  
  /* Use latter half of vertexes to properly display texture */
  /* Create face 5 */
  gSP2Triangles(glistp++, 15, 14, 10, 0, 15, 10, 11, 0);
  /* Create face 6 */
  gSP2Triangles(glistp++, 8, 9, 13, 0, 8, 13, 12, 0);
  
#if        defined(__SHOW_COVERAGE__)
  if(index == 2)
  {
    /* Make coverage visible */
    gDPPipeSync(glistp++);
    gDPSetCycleType(glistp++, G_CYC_1CYCLE);
    gDPSetBlendColor(glistp++, 255, 255, 255, 255);
    gDPSetPrimDepth(glistp++, 0xFFFF, 0xFFFF);
    gDPSetDepthSource(glistp++, G_ZS_PRIM);
    gDPSetRenderMode(glistp++, G_RM_VISCVG, G_RM_VISCVG2);
    gDPFillRectangle(glistp++, 0, 0, SCREEN_WD - 1, SCREEN_HT - 1);
  }
#endif /*  defined(__SHOW_COVERAGE__) */
}

/*
 *  Functions to make resolution and VI mode settings, etc. (call only once after Draw function)
 */
void  SetupResolution(void)
{
  /* Set VI mode */
#if        defined(__HIRES__)
  osViSetMode(&osViModeTable[OS_VI_NTSC_HAN1]);
#elif      defined(__32BIT__)
  osViSetMode(&osViModeTable[OS_VI_NTSC_LAN2]);
#else  /* !defined(__HIRES__) && !defined(__32BIT__) */
  /* Return if not high-resolution or 32-bit mode */
  return;
#endif /*  defined(__HIRES__) */
  
  /* Change special VI function settings */
  osViSetSpecialFeatures(__GAMMA__ |
                         OS_VI_GAMMA_DITHER_ON |
                         OS_VI_DIVOT_OFF |
                         OS_VI_DITHER_FILTER_ON);
  
  /* Set frame buffer address */
  nuGfxSetCfb(cfbPtr, 2);
  
#if        defined(__Z_BUFFER__)
  /* Set Z buffer address */
  nuGfxSetZBuffer((u16 *)zbufPtr);
#endif /*  defined(__Z_BUFFER__) */
}
