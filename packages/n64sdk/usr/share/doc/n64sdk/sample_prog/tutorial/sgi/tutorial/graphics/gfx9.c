/*******************************************************************
 *
 *  gfx9.c
 *    Highlighting and reflection mapping tutorial
 *
 *      This sample shows the methods for achieving highlighting 
 *      and reflection mapping. 
 *
 *******************************************************************/

#include  <nusys.h>

#include  "define.h"

/************************** Customize ***************************/

/*
 *  Note: When __HILITE__ and __REFLECTION__ are both defined, the 
 *        definition for __HILITE__ takes priority. 
 *        If neither is defined, normal lighting is performed with 
 *        ambient light and diffused light. 
 */

/*
 *  When __HILITE__ is defined, highlighting is performed. 
 */
#undef    __HILITE__

/*
 *  When __REFLECTION__ is defined, reflection mapping is performed. 
 */
#undef    __REFLECTION__

/* Definition of texture-related macro constants */
#if        defined(__HILITE__)

/* Texture height and width */
#define     __TEXTURE_WIDTH__     (64)
#define     __TEXTURE_HEIGHT__    (64)
#define     __TEXTURE_BITS_X__    (6)
#define     __TEXTURE_BITS_Y__    (6)
/* Texture data (array format) */
#include    "i8hilite.h"
/* Model data (Gfx array format converted from NIFF data) */
#include    "h_torus.h"

#elif      defined(__REFLECTION__)

/* Texture height and width */
#define     __TEXTURE_WIDTH__     (32)
#define     __TEXTURE_HEIGHT__    (32)
#define     __TEXTURE_BITS_X__    (5)
#define     __TEXTURE_BITS_Y__    (5)
/* Texture data (array format) */
#include    "rgba16reflection.h"
/* Model data (Gfx array format converted from NIFF data) */
#include    "r_torus.h"

#else  /* !defined(__HILITE__) && !defined(__REFLECTION__) */

/* Model data (Gfx array format converted from NIFF data) */
#include    "h_torus.h"

#endif /*  defined(__HILITE__) */

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

#if        defined(__HILITE__)
/* Highlight structure */
static Hilite   hilite;
#endif /*  defined(__HILITE__) */

#if        defined(__HILITE__) ||  defined(__REFLECTION__)
/* View structure  */
static LookAt   lookat;
#endif /*  defined(__HILITE__) ||  defined(__REFLECTION__) */

/* Function prototypes */
static void   ClearBackground(u8 r, u8 g, u8 b);
static void   DrawModel(void);

/************************** Source Code ***************************/

/*
 *   Functions responsible for drawing
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
  
  /* Create rotation matrix (put result in fmat1) */
  guScaleF(fmat1, 0.1, 0.1, 0.1);
  guRotateF(fmat2, theta, 1.0, 1.0, 0.5);
  guMtxCatF(fmat1, fmat2, fmat1);
  
#if        defined(__HILITE__)
  
  /*
   *  Set up highlight structure and view structure 
   *    Even when a second highlight light is not being used, 
   *    the direction vector of the light must be set to something other than 0.
   *    See /usr/src/PR/libsrc/libultra/gu/lookathil.c for details. 
   */
  guLookAtHiliteF(fmat2, &lookat, &hilite,
                 0.0, 0.0, 50.0,
                 0.0, 0.0, 0.0,
                 0.0, 1.0, 0.0,
                 0.0, 1.0, 0.0,   /* Direction of highlight light source */
                 0.0, 0.0, 1.0,   /* Set to something other than 0, even when not in use */
                 __TEXTURE_WIDTH__,
                 __TEXTURE_HEIGHT__);
  
#elif      defined(__REFLECTION__)
  
  /*
   *  Set up view structure 
   */
  guLookAtReflectF(fmat2, &lookat,
                  0.0, 0.0, 50.0,
                  0.0, 0.0, 0.0,
                  0.0, 1.0, 0.0);
  
#else  /* !defined(__HILITE__) && !defined(__REFLECTION__) */
  
  guTranslateF(fmat2, 0.0, 0.0, -50.0);
  
#endif /*  defined(__HILITE__) */
  
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
 *  Cube model vertex data 
 */
static Vtx  cube_vtx[] = {
  {-100, -100, -100, 0, 31 << 6, 31 << 6, -73, -73, -73, 255},
  { 100, -100, -100, 0,  0 << 6, 31 << 6,  73, -73, -73, 255},
  { 100,  100, -100, 0,  0 << 6,  0 << 6,  73,  73, -73, 255},
  {-100,  100, -100, 0, 31 << 6,  0 << 6, -73,  73, -73, 255},
  {-100, -100,  100, 0,  0 << 6, 31 << 6, -73, -73,  73, 255},
  { 100, -100,  100, 0, 31 << 6, 31 << 6,  73, -73,  73, 255},
  { 100,  100,  100, 0, 31 << 6,  0 << 6,  73,  73,  73, 255},
  {-100,  100,  100, 0,  0 << 6,  0 << 6, -73,  73,  73, 255},
};

#if        defined(__REFLECTION__) && !defined(__HILITE__)
/*
 *  If diffused light is intensified while performing reflection mapping, 
 *  the color in the illuminated areas will be clamped. 
 */
Lights1       light = gdSPDefLights1(  0,   0, 50,   /* Blue ambient light */
                                       0,   0,  0,   /* Diffused light is colorless */
                                       0, 127,  0);  /* Direction to diffused light */
#else  /* !defined(__REFLECTION__) ||  defined(__HILITE__) */
Lights1       light = gdSPDefLights1(  0,   0, 50,   /* Blue ambient light */
                                     250, 150, 30,   /* Gold diffused light */
                                       0, 127,  0);  /* Direction to diffused light */
#endif /*  defined(__REFLECTION__) && !defined(__HILITE__) */

/*
 *  Function to create display list to display cube model 
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
  
#if        defined(__HILITE__) ||  defined(__REFLECTION__)
  /* Enable texture, set scaling parameters */
  gSPTexture(glistp++,
             (__TEXTURE_WIDTH__ - 1) << 6,
             (__TEXTURE_HEIGHT__ - 1) << 6,
             0, G_TX_RENDERTILE, G_ON);
#endif /*  defined(__HILITE__) ||  defined(__REFLECTION__) */
  
#if        defined(__HILITE__)
  gSPLookAt(glistp++, &lookat);
  /* Set G_TEXTURE_GEN in the geometry mode */
  gSPSetGeometryMode(glistp++, G_TEXTURE_GEN | G_LIGHTING | G_ZBUFFER | G_SHADE | G_SHADING_SMOOTH | G_CULL_BACK);
  /* G_CC_HILITERGB = PRIMITIVE, SHADE, TEXEL0, SHADE, 0, 0, 0, SHADE */
  gDPSetCombineMode(glistp++, G_CC_HILITERGB, G_CC_HILITERGB);
#elif      defined(__REFLECTION__)
  gSPLookAt(glistp++, &lookat);
  /* Set G_TEXTURE_GEN (and G_TEXTURE_GEN_LINEAR) in the geometry mode */
  gSPSetGeometryMode(glistp++, G_TEXTURE_GEN | G_LIGHTING | G_ZBUFFER | G_SHADE | G_SHADING_SMOOTH | G_CULL_BACK);
  /* G_CC_REFLECTRGB = ENVIRONMENT, 0, TEXEL0, SHADE, 0, 0, 0, SHADE */
  gDPSetCombineMode(glistp++, G_CC_REFLECTRGB, G_CC_REFLECTRGB);
#else  /* !defined(__HILITE__) && !defined(__REFLECTION__) */
  gSPSetGeometryMode(glistp++, G_LIGHTING | G_ZBUFFER | G_SHADE | G_SHADING_SMOOTH | G_CULL_BACK);
  gDPSetCombineMode(glistp++, G_CC_SHADE, G_CC_SHADE);
#endif /*  defined(__HILITE__) */
  
  /* Light positioning */
  gSPSetLights1(glistp++, light);
  
#if        defined(__HILITE__)
  /*
   *  Load highlight texture (image i8hilite)
   *    Turn wrapping ON
   */
  gDPLoadTextureBlock(glistp++,
                      i8hilite,                  /* Pointer to texture image */
                      G_IM_FMT_I,                /* Texel format */
                      G_IM_SIZ_8b,               /* Texel size */
                      __TEXTURE_WIDTH__,         /* Image width */
                      __TEXTURE_HEIGHT__,        /* Image height */
                      0,                         /* LUT (palette) index (not used here) */
                      G_TX_WRAP | G_TX_NOMIRROR, /* Clamp, wrap, mirror frag in s direction */
                      G_TX_WRAP | G_TX_NOMIRROR, /* Clamp, wrap, mirror frag in t direction */
                      __TEXTURE_BITS_X__,        /* s mask */
                      __TEXTURE_BITS_Y__,        /* t mask */
                      G_TX_NOLOD, G_TX_NOLOD);   /* Shift (not shifted here) */
  /* Primitive color becomes color of highlight light */
  gDPSetPrimColor(glistp++, 0, 0, 255, 255, 255, 255);
#elif      defined(__REFLECTION__)
  /*
   *  Load reflection texture (image rgba16reflection)
   *    Turn wrapping ON
   */
  gDPLoadTextureBlock(glistp++,
                      rgba16reflection,          /* Pointer to texture image */
                      G_IM_FMT_RGBA,             /* Texel format */
                      G_IM_SIZ_16b,              /* Texel size */
                      __TEXTURE_WIDTH__,         /* Image width */
                      __TEXTURE_HEIGHT__,        /* Image height */
                      0,                         /* LUT (palette) index (not used here) */
                      G_TX_WRAP | G_TX_NOMIRROR, /* Clamp, wrap, mirror frag in s direction */
                      G_TX_WRAP | G_TX_NOMIRROR, /* Clamp, wrap, mirror frag in t direction */
                      __TEXTURE_BITS_X__,        /* s mask */
                      __TEXTURE_BITS_Y__,        /* t mask */
                      G_TX_NOLOD, G_TX_NOLOD);   /* Shift (not shifted here) */
  /* Ambient color becomes color of reflected light */
  gDPSetEnvColor(glistp++, 250, 160, 50, 255);
#endif /* !defined(__HILITE__) && !defined(__REFLECTION__) */
  
#if        defined(__HILITE__)
  /* Set highlight texture tile  */
  gDPSetHilite1Tile(glistp++, G_TX_RENDERTILE, &hilite, __TEXTURE_WIDTH__, __TEXTURE_HEIGHT__);
#endif /*  defined(__HILITE__) */
  
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
  /*   Fog, opaque, antialiasing, Z buffer */
  gDPSetRenderMode(glistp++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
  
  /*
   *  Display list to define model 
   */
  gSPSegment(glistp++, 5, vtx_torus);
  gSPDisplayList(glistp++, gfx_torus);
}
