/*******************************************************************
 *
 *  gfx10.c
 *    Texture rendering tutorial
 *
 *      In addition to showing a sample of texture rendering, 
 *      this sample also uses multi-tile textures acquired from 
 *      gfx9.c, gfx9lod.c in texture rendering. 
 *
 *******************************************************************/

#include  <nusys.h>

#include  "define.h"

/************************** Customize ***************************/

/*
 *  Note: When both __PANORAMA__ and __MORPHING__ are defined, 
 *        the definition for __PANORAMA__ takes priority. 
 *        If neither is defined, no picture will 
 *        be displayed in the background. 
 */

/*
 *  When __PANORAMA__ is defined, texture that is horizontally long 
 *  will be displayed in the background.
 */
#define    __PANORAMA__

/*
 *  When __MORPHING__  is defined, two pictures will be displayed in 
 *  the background while gradually changing the mixture ratio beteen 
 *  the two pictures.
 */
#undef    __MORPHING__

/*
 *  When __COPY_MODE__ is defined, a small texture rectangle will be 
 *  drawn in copy mode. 
 */
#undef    __COPY_MODE__

/* Texture data (matrix format) */
#define   RECT_WD         32
#define   RECT_HT         32
#include  "rgba16sign1_a.h"

#if        defined(__PANORAMA__)

#define     BACK_WD       720
#define     BACK_HT       240
#define     ROWS          2
#define     DELTA         1
#include    "panorama.h"

#elif      defined(__MORPHING__)

#define     BACK_WD       320
#define     BACK_HT       240
#define     ROWS          3
#define     DELTA         128
#include    "tree1.h"
#include    "tree2.h"

#endif /*  defined(__PANORAMA__) */

/********************* Static Variables & Prototypes **********************/

/* Viewport information */
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
static Gfx      glist[GLIST_LENGTH];
static Gfx *    glistp;

/* Projection conversion matrix and model conversion matrix */
static Mtx      projection, modeling;

/* Scale to normalize same-dimension coordinates */
static u16      perspNorm;

/* Function prototypes */
static void   ClearBackground(u8 r, u8 g, u8 b);
static void   DrawBackground(void);
static void   DrawSmallRectangle(void);
static void   DrawModel(void);

/************************** Source Code ***************************/

/*
 * Functions responsible for drawing 
 */
void  Draw(void)
{
  static int    theta = 0;                  /* Rotational angle of model */
  float         fmat1[4][4], fmat2[4][4];   /* Structure to temporarily hold the conversion matrix */
  
  /* Function to create projection conversion matrix */
  guPerspective(&projection,
                &perspNorm,
                45.0,                                 /* Perspective angle */
                (float)SCREEN_WD / (float)SCREEN_HT,  /* Aspect ratio */
                10.0, 100.0,                          /* Distance to clip plane */
                1.0);
  
  /* Create rotation matrix */
  guRotateF(fmat1, theta, 1.0, 1.0, 0.5);
  /* Parallel motion matrix to remove from POV */
  guTranslateF(fmat2, 0.0, 0.0, -50.0);
  /* Multiply two matrixes (put result in fmat1) */
  guMtxCatF(fmat1, fmat2, fmat1);
  /* Convert matrix from floating point to fixed point */
  guMtxF2L(fmat1, &modeling);
  
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
  
  /* Draw background if necessary (and according to macro definition) */
  DrawBackground();
  
  /* Draw small texture rectangle in foreground */
  DrawSmallRectangle();
  
  /* Add display list command to draw model */
  DrawModel();
  
  /* End display list creation */
  gDPFullSync(glistp++);
  gSPEndDisplayList(glistp++);
  
  /* Write matrix back from cache to memory */
  osWritebackDCache(&projection, sizeof(projection));
  osWritebackDCache(&modeling, sizeof(modeling));
  
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
  
  gDPPipeSync(glistp++);
  /* Set fill mode to (G_CYC_FILL) */
  gDPSetCycleType(glistp++, G_CYC_FILL);
  /* Specify fill object (frame buffer to be displayed next) */
  gDPSetColorImage(glistp++, G_IM_FMT_RGBA, G_IM_SIZ_16b, SCREEN_WD,
                   nuGfxCfb_ptr);
  /* Specify fill color (background color) */
  gDPSetFillColor(glistp++,
                  (GPACK_RGBA5551(r, g, b, 1) << 16 |
                   GPACK_RGBA5551(r, g, b, 1)));
  /* Size of rectangle to be filled (entire screen) */
  gDPFillRectangle(glistp++, 0, 0, SCREEN_WD - 1, SCREEN_HT - 1);
  gDPPipeSync(glistp++);
}

/*
 *  Draw background if necessary (and according to macro definition)
 */
void    DrawBackground(void)
{
#if        defined(__PANORAMA__)
  
  int           i;
  static int    posx = 0, delta = DELTA;
  
  gDPSetCycleType(glistp++, G_CYC_COPY);
  gDPSetCombineMode(glistp++, G_CC_DECALRGBA, G_CC_DECALRGBA);
  gDPSetRenderMode(glistp++, G_RM_NOOP, G_RM_NOOP2);
  gDPSetTexturePersp(glistp++, G_TP_NONE);
  
  for(i = 0; i < (BACK_HT / ROWS); i++)
  {
    gDPLoadTextureTile(glistp++,
                       panorama,
                       G_IM_FMT_RGBA,
                       G_IM_SIZ_16b,
                       BACK_WD,
                       BACK_HT,
                       posx,
                       i * ROWS,
                       posx + (SCREEN_WD - 1),
                       i * ROWS + (ROWS - 1),
                       0,
                       G_TX_WRAP, G_TX_WRAP,
                       0, 0,
                       G_TX_NOLOD, G_TX_NOLOD);
    gSPTextureRectangle(glistp++,
                        0 << 2,
                        (i * ROWS) << 2,
                        (SCREEN_WD - 1) << 2,
                        (i * ROWS + (ROWS - 1)) << 2,
                        G_TX_RENDERTILE,
                        posx << 5, (i * ROWS) << 5,
                        4 << 10, 1 << 10);
  }
  posx += delta;
  if(posx + SCREEN_WD >= BACK_WD)
  {
    posx = BACK_WD - SCREEN_WD;
    delta = -DELTA;
  }
  else if(posx <= 0)
  {
    posx = 0;
    delta = DELTA;
  }
  gDPPipeSync(glistp++);
  
#elif      defined(__MORPHING__)
  
  int           i;
  static int    blend = 0, delta = DELTA;
  
  gDPSetCycleType(glistp++, G_CYC_2CYCLE);
  gDPSetCombineLERP(glistp++, TEXEL1, TEXEL0, ENV_ALPHA,  TEXEL0,
                                   0,      0,         0,  TEXEL0,
                                   0,      0,         0, COMBINED,
                                   0,      0,         0, COMBINED);
  gDPSetRenderMode(glistp++, G_RM_PASS, G_RM_PASS);
  gDPSetTexturePersp(glistp++, G_TP_NONE);
  
  gDPSetEnvColor(glistp++, 0, 0, 0, (blend >> 8) & 0xFF);
  
  for(i = 0; i < (BACK_HT / ROWS); i++)
  {
    gDPLoadMultiTile(glistp++,
                     tree1,
                     0,
                     G_TX_RENDERTILE,
                     G_IM_FMT_RGBA,
                     G_IM_SIZ_16b,
                     BACK_WD,
                     BACK_HT,
                     0,
                     i * ROWS,
                     SCREEN_WD - 1,
                     i * ROWS + (ROWS - 1),
                     0,
                     G_TX_WRAP, G_TX_WRAP,
                     0, 0,
                     G_TX_NOLOD, G_TX_NOLOD);
    gDPLoadMultiTile(glistp++,
                     tree2,
                     240,
                     G_TX_RENDERTILE + 1,
                     G_IM_FMT_RGBA,
                     G_IM_SIZ_16b,
                     BACK_WD,
                     BACK_HT,
                     0,
                     i * ROWS,
                     SCREEN_WD - 1,
                     i * ROWS + (ROWS - 1),
                     0,
                     G_TX_WRAP, G_TX_WRAP,
                     0, 0,
                     G_TX_NOLOD, G_TX_NOLOD);
    gSPTextureRectangle(glistp++,
                        0 << 2,
                        (i * ROWS) << 2,
                        (SCREEN_WD - 1) << 2,
                        (i * ROWS + ROWS) << 2,   /* No different from copy mode (ROWS-1) */
                        G_TX_RENDERTILE,
                        0 << 5, (i * ROWS) << 5,
                        1 << 10, 1 << 10);
  }
  
  blend += delta;
  if(blend >= 0x10000)
  {
    blend = 0xFFFF;
    delta = -DELTA;
  }
  else if(blend <= 0)
  {
    blend = 0;
    delta = DELTA;
  }
#else  /* !defined(__PANORAMA__) && !defined(__MORPHING__) */
  /* Does nothing */
#endif /*  defined(__PANORAMA__) */
}

/*
 *  Draw small texture rectangle in foreground
 */
void    DrawSmallRectangle(void)
{
  static int      posx, posy;
  
  gDPPipeSync(glistp++);
#if        defined(__COPY_MODE__)
  /* Set cycle type to copy mode */
  gDPSetCycleType(glistp++, G_CYC_COPY);
  /* Set coefficient 0 for combine mode */
  gDPSetCombineLERP(glistp++, 0, 0, 0, 0,
                              0, 0, 0, 0,
                              0, 0, 0, 0,
                              0, 0, 0, 0);
  /* Rendering mode is G_RM_NOOP, G_RM_NOOP2 (nothing is done by the blender) */
  gDPSetRenderMode(glistp++, G_RM_NOOP, G_RM_NOOP2);
  /*
   *  Compare alpha values, and set blend color alpha to 1
   *  in order to "Remove background."
   *  At this time, only those areas where alpha is 0 will become transparent pixels.
   *  This can be used in the G_RM_NOOP mode, which passes blender processing. 
   */
  gDPSetBlendColor(glistp++, 0, 0, 0, 1);
  gDPSetAlphaCompare(glistp++, G_AC_THRESHOLD);
#else  /* !defined(__COPY_MODE__) */
  /* Set cycle type to 1 cycle mode */
  gDPSetCycleType(glistp++, G_CYC_1CYCLE);
  /* The combiner mode uses the color alpha of the texture */
  gDPSetCombineMode(glistp++, G_CC_DECALRGBA, G_CC_DECALRGBA);
  /* Rendering mode */
  gDPSetRenderMode(glistp++, G_RM_AA_ZB_TEX_EDGE, G_RM_AA_ZB_TEX_EDGE2);
  /* Set Z value source */
  gDPSetDepthSource(glistp++, G_ZS_PRIM);
  /* Set Z value of texture rectangle */
  gDPSetPrimDepth(glistp++, 0, 0);
#endif /*  defined(__COPY_MODE__) */
  
  /* Texture perspective correction is turned OFF because texture rectangle is being drawn */
  gDPSetTexturePersp(glistp++, G_TP_NONE);
  
  /* Load texture */
  gDPLoadTextureBlock(glistp++,
                      rgba16sign1_a,
                      G_IM_FMT_RGBA,
                      G_IM_SIZ_16b,
                      32, 32,
                      0,
                      G_TX_WRAP,
                      G_TX_WRAP,
                      5, 5,
                      G_TX_NOLOD, G_TX_NOLOD);
  
  /* Draw texture rectangle */
#if        defined(__COPY_MODE__)
  gSPTextureRectangle(glistp++,
                      posx << 2, posy << 2,
                      (posx + RECT_WD - 1) << 2, (posy + RECT_HT - 1) << 2,
                      G_TX_RENDERTILE,
                      0 << 5, 0 << 5,
                      4 << 10, 1 << 10);
  gDPPipeSync(glistp++);
  /* Return alpha value comparison setting to OFF */
  gDPSetAlphaCompare(glistp++, G_AC_NONE);
#else  /* !defined(__COPY_MODE__) */
  gSPTextureRectangle(glistp++,
                      posx << 2, posy << 2,
                      (posx + RECT_WD) << 2, (posy + RECT_HT) << 2,
                      G_TX_RENDERTILE,
                      0 << 5, 0 << 5,
                      1 << 10, 1 << 10);
#endif /*  defined(__COPY_MODE__) */
  
  /* Update rectangle drawing position */
  if((posx += 2) >= SCREEN_WD)
  {
    if((posy += RECT_HT) >= SCREEN_HT)
      posy = 0;
    posx = 0;
  }
}

/*
 *  Cube model vertex data 
 */
static Vtx  cube_vtx[] = {
  {-10, -10, -10, 0, 31 << 6, 31 << 6, 255,   0,   0, 255},
  { 10, -10, -10, 0,  0 << 6, 31 << 6,   0, 255,   0, 255},
  { 10,  10, -10, 0,  0 << 6,  0 << 6,   0,   0, 255, 255},
  {-10,  10, -10, 0, 31 << 6,  0 << 6, 255, 255,   0, 255},
  {-10, -10,  10, 0,  0 << 6, 31 << 6, 255,   0, 255, 255},
  { 10, -10,  10, 0, 31 << 6, 31 << 6,   0, 255, 255, 255},
  { 10,  10,  10, 0, 31 << 6,  0 << 6, 255, 255, 255, 255},
  {-10,  10,  10, 0,  0 << 6,  0 << 6, 255,   0,   0, 255},
  /* Vertexes for properly texture mapping top and bottom surfaces of cube */
  {-10, -10, -10, 0,  0 << 6, 31 << 6, 255,   0,   0, 255},
  { 10, -10, -10, 0, 31 << 6, 31 << 6,   0, 255,   0, 255},
  { 10,  10, -10, 0, 31 << 6,  0 << 6,   0,   0, 255, 255},
  {-10,  10, -10, 0,  0 << 6,  0 << 6, 255, 255,   0, 255},
  {-10, -10,  10, 0,  0 << 6,  0 << 6, 255,   0, 255, 255},
  { 10, -10,  10, 0, 31 << 6,  0 << 6,   0, 255, 255, 255},
  { 10,  10,  10, 0, 31 << 6, 31 << 6, 255, 255, 255, 255},
  {-10,  10,  10, 0,  0 << 6, 31 << 6,   0,   0,   0, 255},
};

/*
 *  Function to create display list to display model 
 */
void  DrawModel(void)
{
  gDPPipeSync(glistp++);
  
  /* Specify projection conversion matrix */
  gSPMatrix(glistp++,
            &projection,
            G_MTX_PROJECTION |
            G_MTX_LOAD |
            G_MTX_NOPUSH);
  /* Specify model conversion matrix */
  gSPMatrix(glistp++,
            &modeling,
            G_MTX_MODELVIEW |
            G_MTX_LOAD |
            G_MTX_NOPUSH);
  /* Scaling of same dimension coordinates */
  gSPPerspNormalize(glistp++, &perspNorm);
  
  /*
   *  Display list to make drawing related settings 
   */
  
  /* Set RDP cycle type */
  gDPSetCycleType(glistp++, G_CYC_1CYCLE);
  
  /* Enable texture, set scaling parameters */
  gSPTexture(glistp++, 0x8000, 0x8000, 0, G_TX_RENDERTILE, G_ON);
  
  /* Flat shading */
  gSPSetGeometryMode(glistp++, G_SHADE | G_CULL_BACK);
  
  /* Output raw texture color and alpha to combiner */
  gDPSetCombineLERP(glistp++, 0, 0, 0, TEXEL0,
                              0, 0, 0, TEXEL0,
                              0, 0, 0, TEXEL0,
                              0, 0, 0, TEXEL0);
  /*
   *  Load polygon texture 
   *    Since this was already loaded with DrawSmallRectangle, 
   *    it is actually enough simply to change the tile settings. 
   */
  gDPLoadTextureBlock(glistp++,
                      rgba16sign1_a,            /* Pointer to texture image */
                      G_IM_FMT_RGBA,            /* Texel format */
                      G_IM_SIZ_16b,             /* Texel size */
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
  gDPSetRenderMode(glistp++, G_RM_AA_ZB_XLU_SURF, G_RM_AA_ZB_XLU_SURF2);
  
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
}
