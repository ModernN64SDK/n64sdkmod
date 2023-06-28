/*******************************************************************
 *
 *  gfx9lod.c
 *    LOD texture sample program
 *
 *      This sample shows the method for normal MIP mapping, as 
 *      well as the method for using detail textures. 
 *      The minimum standard operations for texture tiles are also 
 *      performed. 
 *
 *******************************************************************/

#include  <nusys.h>

#include  "define.h"

/************************** Customize ***************************/

/*
 *  Note: When __DETAIL__ and __SHARPEN__ are both defined, the 
 *        definition for __DETAIL__ takes priority. 
 *        If neither one is defined, normal 
 *        MIP mapping is performed. 
 */

/*
 *  When __DETAIL__ is defined, detail texture is used. 
 */
#undef    __DETAIL__

/*
 *  When __SHARPEN__ is defined, the sharp mode is used. 
 */
#undef    __SHARPEN__

/* Texture data (array format) */
#include    "rgba16brickmm.h"
#include    "rgba16brick.h"

#if        defined(__DETAIL__)
#include    "circle.h"
#endif /*  defined(__DETAIL__) */

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

/* Projection conversion matrix */
static Mtx    projection;
/* Common model conversion matrix */
static Mtx    common;
/* Individual model conversion matrix */
static Mtx    modellocal[2];

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
  static int    offset = 50;                /* Depth at which model is situated */
  static int    delta = 1;                  /* Change depth each frame */
  float         fmat1[4][4], fmat2[4][4];   /* Structure to temporarily hold the conversion matrix */
  
  /* Function to create projection conversion matrix */
  guPerspective(&projection,
                &perspNorm,
                45.0,                                 /* Perspective angle */
                (float)SCREEN_WD / (float)SCREEN_HT,  /* Aspect ratio */
                10.0, 1000.0,                         /* Distance to clip plane */
                1.0);
  
  /* Create rotation matrix */
  guRotateF(fmat1, theta, 1.0, 1.0, 0.5);
  /* Parallel motion matrix to remove from POV */
  guTranslateF(fmat2, 0.0, 0.0, -(offset + 30.0));
  /* Multiply two matrixes (put result in fmat1) */
  guMtxCatF(fmat1, fmat2, fmat1);
  /* Convert matrix from floating point to fixed point */
  guMtxF2L(fmat1, &common);
  
  /* Create local conversion matrix */
  guTranslate(&modellocal[0], -30.0, 0.0, 0.0);
  guTranslate(&modellocal[1],  30.0, 0.0, 0.0);
  
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
  osWritebackDCache(&common, sizeof(common));
  osWritebackDCache(&modellocal, sizeof(modellocal));
  
  /* Start task */
  nuGfxTaskStart(glist,
                 (s32)(glistp - glist) * sizeof(Gfx),
                 NU_GFX_UCODE_F3DEX, NU_SC_SWAPBUFFER);
  
  /* On one rotation, return angle to 0 */
  if(++theta >= 360)
    theta = 0;
  /* Change depth */
  offset += delta;
  if(offset < 0)
  {
    offset = 0;
    delta = 1;
  }
  else if(offset > 500)
  {
    offset = 500;
    delta = -1;
  }
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
  int       i;
  
  for(i = 0; i < 2; i++)
  {
    gDPPipeSync(glistp++);
    /* Specify projection conversion matrix */
    gSPMatrix(glistp++,
              &projection,
              G_MTX_PROJECTION |
              G_MTX_LOAD |
              G_MTX_NOPUSH);
    /* Specify model conversion matrix */
    if(i == 0)
    {
      gSPMatrix(glistp++,
                &common,
                G_MTX_MODELVIEW |
                G_MTX_LOAD |
                G_MTX_NOPUSH);
    }
    else
    {
      gSPPopMatrix(glistp++, G_MTX_MODELVIEW);
    }
    gSPMatrix(glistp++,
              &modellocal[i],
              G_MTX_MODELVIEW |
              G_MTX_MUL |
              G_MTX_PUSH);
    /* Scaling of same dimension coordinates */
    gSPPerspNormalize(glistp++, &perspNorm);
    
    /*
     *  Display list to make drawing related settings 
     */
    
    if(i == 0)      /* MIP map */
    {
      /* Set RDP cycle type */
      gDPSetCycleType(glistp++, G_CYC_2CYCLE);
      
      /* Enable texture, set LOD parameters */
#if       !defined(__DETAIL__)
      /* When not using detail texture, empty tile 0 */
      gSPTexture(glistp++, 0x8000, 0x8000,
                 5,                         /* MIP map label */
                 G_TX_RENDERTILE + 1,       /* Tile number of maximum resolution MIP map */
                 G_ON);
#else  /*  defined(__DETAIL__) */
      /* When using detail texture, set detail texture in tile 0  */
      gSPTexture(glistp++, 0x8000, 0x8000,
                 5,                         /* MIP map label */
                 G_TX_RENDERTILE,           /* Tile number of detail texture */
                 G_ON);
#endif /* !defined(__DETAIL__) */
      
      /* Set primitive color (LOD coefficient value) */
      gDPSetPrimColor(glistp++,
                      128,          /* Minimum clamp value for blending detail texture */
                      128,          /* Blend ratio with different LOD textures */
                      0, 0, 0, 0);  /* Primitive color (not used in this sample) */
      
      /* Get display list for loading texture */
      gSPDisplayList(glistp++, rgba16brickmm_dl);
      
      /* Set mode */
      gSPSetGeometryMode(glistp++, G_ZBUFFER | G_SHADE | G_CULL_BACK);
      gDPSetRenderMode(glistp++, G_RM_PASS, G_RM_AA_ZB_OPA_SURF2);
      /*
       *  G_CC_TRILERP = TEXEL1, TEXEL0, LOD_FRACTION, TEXEL0,
       *                 TEXEL1, TEXEL0, LOD_FRACTION, TEXEL0
       */
      gDPSetCombineMode(glistp++, G_CC_TRILERP, G_CC_DECALRGB2);
      
      /* Texture perspective correction is turned on during mapping */
      gDPSetTexturePersp(glistp++, G_TP_PERSP);
      /* Specify texture selection according to LOD */
      gDPSetTextureLOD(glistp++, G_TL_LOD);
#if       !defined(__DETAIL__) && !defined(__SHARPEN__)
      /* Normal MIP mapping */
      gDPSetTextureDetail(glistp++, G_TD_CLAMP);
#elif      defined(__DETAIL__)
      /* Use detail texture */
      gDPSetTextureDetail(glistp++, G_TD_DETAIL);
      /* Load detail texture */
      gDPLoadTextureTile(glistp++,
                         circle,
                         G_IM_FMT_I,
                         G_IM_SIZ_8b,
                         32, 32,
                         0, 0,
                         31, 31,
                         0,
                         G_TX_WRAP, G_TX_WRAP,
                         5, 5,
                         15, 14);   /* << 1 and << 2 */
#else  /*  defined(__SHARPEN__) */
      /* Use sharp mode */
      gDPSetTextureDetail(glistp++, G_TD_SHARPEN);
#endif /* !defined(__DETAIL__) && ! defined(__SHARPEN__) */
      
      /* Set texture filter */
      gDPSetTextureFilter(glistp++, G_TF_BILERP);
      gDPSetTextureConvert(glistp++, G_TC_FILT);
      /* Texture palette (not used) */
      gDPSetTextureLUT(glistp++, G_TT_NONE);
    }
    else            /* Normal texture mapping */
    {
      /* Set RDP cycle type */
      gDPSetCycleType(glistp++, G_CYC_1CYCLE);
      
      /* Enable texture, set scaling parameters */
      gSPTexture(glistp++, 0x8000, 0x8000,
                 0,
                 G_TX_RENDERTILE, G_ON);
      /* Load texture (image rgba16brick) */
      gDPLoadTextureBlock(glistp++,
                          rgba16brick,              /* Pointer to texture image */
                          G_IM_FMT_RGBA,            /* Texel format */
                          G_IM_SIZ_16b,             /* Texel size */
                          32, 32,                   /* Image width and height */
                          0,                        /* LUT (palette) index (not used here) */
                          G_TX_WRAP, G_TX_WRAP,     /* Clamp, wrap, mirror frag in s, t direction */
                          31, 31,                   /* s, t masks */
                          G_TX_NOLOD, G_TX_NOLOD);  /* Shift (not shifted here) */
      
      /* Set mode */
      gSPSetGeometryMode(glistp++, G_ZBUFFER | G_SHADE | G_CULL_BACK);
      gDPSetRenderMode(glistp++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
      gDPSetCombineMode(glistp++, G_CC_DECALRGB, G_CC_DECALRGB);
      
      /* Texture perspective correction is turned on during mapping */
      gDPSetTexturePersp(glistp++, G_TP_PERSP);
      /* Specify texture selection according to LOD */
      gDPSetTextureLOD(glistp++, G_TL_TILE);
      /* Detail texture is not used  */
      gDPSetTextureDetail(glistp++, G_TD_CLAMP);
      /* Set texture filter */
      gDPSetTextureFilter(glistp++, G_TF_BILERP);
      gDPSetTextureConvert(glistp++, G_TC_FILT);
      /* Texture palette (not used) */
      gDPSetTextureLUT(glistp++, G_TT_NONE);
    }
    
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
}
