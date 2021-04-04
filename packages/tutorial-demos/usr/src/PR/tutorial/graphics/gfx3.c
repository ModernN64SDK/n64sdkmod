/*******************************************************************
 *
 *  gfx3.c
 *    Texture mapping tutorial
 *
 *      Small texture is mapped onto a cube model. 
 *      This allows you to experience texture filters and 
 *      texture wrapping. 
 *
 *******************************************************************/

#include  <nusys.h>

#include  "define.h"

/* Texture data (in matrix format) */
#include  "rgba16sign1.h"

/************************** Customize ***************************/

/*
 *  Texture filters 
 *    G_TF_POINT, G_TF_AVERAGE, G_TF_BILERP can be specified. 
 */
#define   __TEXTURE_FILTER__      G_TF_POINT

/*
 *  You can toggle between clamp, wrap, and mirror when mapping 
 *  texture in the s and t directions. 
 *  Try different combinations of G_TX_CLAMP, G_TX_WRAP, G_TX_MIRROR, 
 *  and G_TX_NOMIRROR to get a feel the differences. 
 */
#define   __CWR_S__               (G_TX_WRAP)
#define   __CWR_T__               (G_TX_WRAP)

/*
 *  These are the maximum s, t values for the model faces. 
 *  Because the texture width (height) is 32, 
 *  the default is 31. 
 *  By changing these settings together with the mask bit values, 
 *  you can realize the effect of repeatedly mapping a texture. 
 */
#define   __MAX_S__               (31)
#define   __MAX_T__               (31)

/*
 *  This is the number of mask bits in the s (t) direction. 
 *  Since the width (height) of the texture is 32=2^5, 
 *  the default is 5.
 */
#define   __MASK_S__              (5)
#define   __MASK_T__              (5)

/********************* Static Variables & Prototypes **********************/

/* Viewpoint information */
static  Vp  viewport = {
  SCREEN_WD * 2, SCREEN_HT * 2, G_MAXZ / 2, 0,
  SCREEN_WD * 2, SCREEN_HT * 2, G_MAXZ / 2, 0,
};

/* RSP initialization display list */
static Gfx rspinit_dl[] = {
  gsSPViewport(&viewport),
  gsSPClearGeometryMode(G_SHADE | G_SHADING_SMOOTH | G_CULL_BOTH |
                        G_FOG | G_LIGHTING | G_TEXTURE_GEN |
                        G_TEXTURE_GEN_LINEAR | G_LOD),
  gsSPTexture(0, 0, 0, 0, G_OFF),
  gsSPEndDisplayList(),
};

/* RDP initialization display list */
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
  
  /* Add display list command to draw a cube */
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
 *  Cube model vertex data 
 */
static Vtx  cube_vtx[] = {
  {-10, -10, -10, 0, __MAX_S__ << 6, __MAX_T__ << 6, 0, 0, 0, 255},
  { 10, -10, -10, 0,         0 << 6, __MAX_T__ << 6, 0, 0, 0, 255},
  { 10,  10, -10, 0,         0 << 6,         0 << 6, 0, 0, 0, 255},
  {-10,  10, -10, 0, __MAX_S__ << 6,         0 << 6, 0, 0, 0, 255},
  {-10, -10,  10, 0,         0 << 6, __MAX_T__ << 6, 0, 0, 0, 255},
  { 10, -10,  10, 0, __MAX_S__ << 6, __MAX_T__ << 6, 0, 0, 0, 255},
  { 10,  10,  10, 0, __MAX_S__ << 6,         0 << 6, 0, 0, 0, 255},
  {-10,  10,  10, 0,         0 << 6,         0 << 6, 0, 0, 0, 255},
  /* Vertexes to properly texture map top and bottom faces of cube */
  {-10, -10, -10, 0,         0 << 6, __MAX_T__ << 6, 0, 0, 0, 255},
  { 10, -10, -10, 0, __MAX_S__ << 6, __MAX_T__ << 6, 0, 0, 0, 255},
  { 10,  10, -10, 0, __MAX_S__ << 6,         0 << 6, 0, 0, 0, 255},
  {-10,  10, -10, 0,         0 << 6,         0 << 6, 0, 0, 0, 255},
  {-10, -10,  10, 0,         0 << 6,         0 << 6, 0, 0, 0, 255},
  { 10, -10,  10, 0, __MAX_S__ << 6,         0 << 6, 0, 0, 0, 255},
  { 10,  10,  10, 0, __MAX_S__ << 6, __MAX_T__ << 6, 0, 0, 0, 255},
  {-10,  10,  10, 0,         0 << 6, __MAX_T__ << 6, 0, 0, 0, 255},
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
  /* Scaling of same-dimension coordinates */
  gSPPerspNormalize(glistp++, &perspNorm);
  
  /*
   *  Display list to make drawing related settings 
   */
  
  /* Set RDP cycle type */
  gDPSetCycleType(glistp++, G_CYC_1CYCLE);
  
  /* Enable texture, set scaling parameters */
  gSPTexture(glistp++, 0x8000, 0x8000, 0, G_TX_RENDERTILE, G_ON);
  
  /* Specify back surface culling */
  gSPSetGeometryMode(glistp++, G_CULL_BACK);
  
  /* Switch to combine mode using texture color */
  gDPSetCombineMode(glistp++, G_CC_DECALRGBA, G_CC_DECALRGBA);
  
  /* Load texture (image rgba16sign1) */
  gDPLoadTextureBlock(glistp++,
                      rgba16sign1,              /* Pointer to texture image */
                      G_IM_FMT_RGBA,            /* Texel format */
                      G_IM_SIZ_16b,             /* Texel size */
                      32, 32,                   /* Image width and height */
                      0,                        /* LUT (palette) index (not used here) */
                      __CWR_S__, __CWR_T__,     /* Clamp, wrap, mirror frag in s direction */
                      __MASK_S__, __MASK_T__,   /* s, t masks */
                      G_TX_NOLOD, G_TX_NOLOD);  /* Shift (not shifted here) */
  
  /* Texture perspective correction is turned on during mapping */
  gDPSetTexturePersp(glistp++, G_TP_PERSP);
  /* Set texture filter */
  gDPSetTextureFilter(glistp++, __TEXTURE_FILTER__);
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
