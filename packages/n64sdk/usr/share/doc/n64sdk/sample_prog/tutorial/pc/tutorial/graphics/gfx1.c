/*******************************************************************
 *
 *  gfx1.c
 *    Minimal 3D graphics display tutorial 
 *
 *      Quadrangles (two triangles) are displayed colored with smooth
 *      shading. Simple animation is also realized by changing the 
 *      rotational angle of the model. 
 *
 *******************************************************************/

#include  <nusys.h>

#include  "define.h"

/************************** Customize ***************************/

/*
 * The gSP1Triangle command is used when __ONE_TRIANGLE__ is defined. 
 * Use gSP2Triangles when it is not defined. 
 */
#undef    __ONE_TRIANGLE__

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
static void   DrawSquare(void);

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
  ClearBackground(255, 255, 255);
  
  /* Add display list command to draw a square */
  DrawSquare();
  
  /* End display list creation */
  gDPFullSync(glistp++);
  gSPEndDisplayList(glistp++);
  
  /* Write matrix back from cache to memory (crucial) */
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
 * Square model vertex data 
 */
static Vtx  square_vtx[] = {
  {-10, -10, 0, 0, 0, 0, 255,   0,   0, 255},
  { 10, -10, 0, 0, 0, 0,   0, 255,   0, 255},
  { 10,  10, 0, 0, 0, 0,   0,   0, 255, 255},
  {-10,  10, 0, 0, 0, 0, 255, 255, 255, 255},
};

/*
 * Function to create display list to display square model
 */
void  DrawSquare(void)
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
  /* Perform smooth shading */
  gSPSetGeometryMode(glistp++, G_SHADE | G_SHADING_SMOOTH);
  /* Color model using shade color */
  gDPSetCombineMode(glistp++, G_CC_SHADE, G_CC_SHADE);
  /* Set rendering mode */
  gDPSetRenderMode(glistp++, G_RM_AA_OPA_SURF, G_RM_AA_OPA_SURF2);
  
  /*
   *  Display list to define model 
   */
  
  /* Load model vertexes */
  gSPVertex(glistp++, square_vtx, 4, 0);
  
  /* Create one square from loaded vertexes */
#if        defined(__ONE_TRIANGLE__)
  gSP1Triangle(glistp++, 0, 1, 2, 0);
  gSP1Triangle(glistp++, 0, 2, 3, 0);
#else  /* !defined(__ONE_TRIANGLE__) */
  gSP2Triangles(glistp++, 0, 1, 2, 0, 0, 2, 3, 0);
#endif /*  defined(__ONE_TRIANGLE__) */
  
  gDPPipeSync(glistp++);
}
