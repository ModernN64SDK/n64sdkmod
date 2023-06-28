/*******************************************************************
 *
 *  gfx4.c
 *    Combiner explantion sample 
 *
 *      This sample defines the custom combiner mode and tests 
 *      several expression methods in order to understand the combiner
 *
 *******************************************************************/

#include  <nusys.h>

#include  "define.h"

/************************** Customize ***************************/

/*
 * Notes: When __INVERT__ and __NOISE__ are both defined, 
 *        the definition of __INVERT__ takes priority.
 *        If neither one is defined, the basic 
 *        single-color surface is colored with round gradation.
 */

/*
 *  When __INVERT__ is defined, the texture colors are displayed inverted. 
 */
#undef    __INVERT__

/*
 *  When __NOISE__ is defined, the pattern is displayed using noise. 
 */
#undef    __NOISE__

/* Texture data (array format) */
#if        defined(__INVERT__)
#include    "rgba16sign1.h"
#elif      defined(__NOISE__)
            /* Since the pattern is created with noise, texture is not used */
#else  /* !defined(__INVERT__) && !defined(__NOISE__) */
#include    "circle.h"
#endif /*  defined(__INVERT__) */

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
static Gfx    glist[GLIST_LENGTH];
static Gfx *  glistp;

/* Projection conversion matrix and model conversion matrix */
static Mtx    projection, modeling;

/* Scale to normalize same-dimension coordinates */
static u16    perspNorm;

/* Function prototypes */
static void   ClearBackground(u8 r, u8 g, u8 b);
static void   DrawCube(void);

/************************** Source Code ***************************/

/*
 *  Functions responsible for drawing 
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
  
  /* Add display list command to draw cube */
  DrawCube();
  
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
 * Cube model vertex data 
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
 *  Function to create display list to display cube model 
 */
void  DrawCube(void)
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
  
  /*
   *  If colors are not combined by the combiner with flat shading, etc., 
   *  each face will be filled with a single color. 
   */
  gSPSetGeometryMode(glistp++, G_SHADE | G_CULL_BACK);
  
#if        defined(__INVERT__)
  
  /* Set to use primitive color as value 1 source */
  gDPSetPrimColor(glistp++, 0, 0, 255, 255, 255, 255);
  
  /*
   *  Output each color component of texture inverted from the combiner. 
   *    color = (1 - TEXEL0) * 1 + 0 = 1 - TEXEL0
   *  (Primitive color is simply used as input to provide the value 1.)
   */
  gDPSetCombineLERP(glistp++, 1, TEXEL0, PRIMITIVE,     0,
                              0,      0,         0, SHADE,
                              1, TEXEL0, PRIMITIVE,     0,
                              0,      0,         0, SHADE);
  
  /* Load texture (image rgba16sign1) */
  gDPLoadTextureBlock(glistp++,
                      rgba16sign1,              /* Pointer to texture image */
                      G_IM_FMT_RGBA,            /* Texel format */
                      G_IM_SIZ_16b,             /* Texel size */
                      32, 32,                   /* Image width and height */
                      0,                        /* LUT (palette) index (not used here) */
                      G_TX_CLAMP, G_TX_CLAMP,   /* Clamp, wrap, mirror frag in s direction */
                      5, 5,                     /* s, t mask */
                      G_TX_NOLOD, G_TX_NOLOD);  /* Shift (not shifted here) */
  
#elif      defined(__NOISE__)
  
  /*
   *  Output from combiner appyling shade color (monochrome) to random numbers (noise). 
   *    color = (NOISE - 0) * SHADE + 0 = NOISE * SHADE
   *  The only source of noise is the a in (a - b) * c + d. 
   */
  gDPSetCombineLERP(glistp++, NOISE, 0, SHADE,     0,
                              0,     0,     0, SHADE,
                              NOISE, 0, SHADE,     0,
                              0,     0,     0, SHADE);
  
#else  /* !defined(__INVERT__) && !defined(__NOISE__) */
  
  /*
   *  Output from combiner applying luminance of texture to shade color (monochrome)
   *    color = (SHADE - 0) * TEXEL0 + 0 = SHADE * TEXEL0
   */
  gDPSetCombineLERP(glistp++, SHADE, 0, TEXEL0,     0,
                              0,     0,      0, SHADE,
                              SHADE, 0, TEXEL0,     0,
                              0,     0,      0, SHADE);
  
  /* Load gradation texture (image circle) */
  gDPLoadTextureBlock(glistp++,
                      circle,                   /* Pointer to texture image */
                      G_IM_FMT_I,               /* Texel format */
                      G_IM_SIZ_8b,              /* Texel size */
                      32, 32,                   /* Image width and height */
                      0,                        /* LUT (palette) index (not used here) */
                      G_TX_CLAMP, G_TX_CLAMP,   /* Clamp, wrap, mirror frag in s direction */
                      5, 5,                     /* s, t masks */
                      G_TX_NOLOD, G_TX_NOLOD);  /* Shift (not shifted here) */
  
#endif /*  defined(__INVERT__) */
  
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
  gDPSetRenderMode(glistp++, G_RM_OPA_SURF, G_RM_OPA_SURF2);
  
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
