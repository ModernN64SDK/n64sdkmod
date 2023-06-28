/*******************************************************************
 *
 *  gfx2.c
 *    Shading (coloring) methods tutorial 
 *
 *      Coloring utilizing vertex color and primitive color, and
 *      lighting, are performed on a cube model.  You can also 
 *      see the differences between flat shading and smooth 
 *      shading with respect to coloring by vertex color or by 
 *      lighting. 
 *
 *******************************************************************/

#include  <nusys.h>

#include  "define.h"

/************************** Customize ***************************/

/*
 *  Note: If both __LIGHTING__ and __PRIMITIVE__ are defined, 
 *        the definition for __LIGHTING__ takes priority. 
 *        If neither is defined, shading is performed using 
 *        the vertex color. 
 */

/*
 *  When __LIGHTING__ is defined, lighting processing is performed. 
 */
#undef    __LIGHTING__

/*
 *  When __PRIMITIVE__ is defined, coloring is performed with the 
 *  primitive color. 
 */
#undef    __PRIMITIVE__

/*
 *  When __SMOOTH__ is defined, smooth shading is used as the 
 *  algorithm for shading (including lighting). 
 */
#undef    __SMOOTH__

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
  
  /* Add display list command to draw a cube */
  DrawCube();
  
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
 *  Cube model vertex data 
 */
static Vtx  cube_vtx[] = {
#if        defined(__LIGHTING__)
#if        defined(__SMOOTH__)
  /*
   *  Vertex data for when lighting is ON. 
   *  Normal vector, not vertex color, is stored. 
   *  For smooth shading. 
   */
  {-10, -10, -10, 0, 0, 0, -73, -73, -73, 255},
  { 10, -10, -10, 0, 0, 0,  73, -73, -73, 255},
  { 10,  10, -10, 0, 0, 0,  73,  73, -73, 255},
  {-10,  10, -10, 0, 0, 0, -73,  73, -73, 255},
  {-10, -10,  10, 0, 0, 0, -73, -73,  73, 255},
  { 10, -10,  10, 0, 0, 0,  73, -73,  73, 255},
  { 10,  10,  10, 0, 0, 0,  73,  73,  73, 255},
  {-10,  10,  10, 0, 0, 0, -73,  73,  73, 255},
#else  /* !defined(__SMOOTH__) */
  /*
   *  Vertex data for when lighting is ON. 
   *  Normal vector, not vertex color, is stored. 
   *  For flat shading. 
   */
  {-10, -10, -10, 0, 0, 0,   0,-127,   0, 255},
  { 10, -10, -10, 0, 0, 0,   0,   0,-127, 255},
  { 10,  10, -10, 0, 0, 0,   0, 127,   0, 255},
  {-10,  10, -10, 0, 0, 0,   0,   0,   0, 255},
  {-10, -10,  10, 0, 0, 0,   0,   0, 127, 255},
  { 10, -10,  10, 0, 0, 0, 127,   0,   0, 255},
  { 10,  10,  10, 0, 0, 0,   0,   0,   0, 255},
  {-10,  10,  10, 0, 0, 0,-127,   0,   0, 255},
#endif /*  defined(__SMOOTH__) */
#else  /* !defined(__LIGHTING__) */
  /*
   *  Vertex data for when lighting is OFF. 
   *  Vertex color, not normal vector, is stored. 
   *  Now, when coloring with primitive color, 
   *  vertex color data are not used. 
   */
  {-10, -10, -10, 0, 0, 0, 255,   0,   0, 255},
  { 10, -10, -10, 0, 0, 0,   0, 255,   0, 255},
  { 10,  10, -10, 0, 0, 0,   0,   0, 255, 255},
  {-10,  10, -10, 0, 0, 0, 255, 255,   0, 255},
  {-10, -10,  10, 0, 0, 0, 255,   0, 255, 255},
  { 10, -10,  10, 0, 0, 0,   0, 255, 255, 255},
  { 10,  10,  10, 0, 0, 0, 255, 255, 255, 255},
  {-10,  10,  10, 0, 0, 0,   0,   0,   0, 255},
#endif /*  defined(__LIGHTING__) */
};

#if        defined(__LIGHTING__)
/* Structure defining light (diffused light + ambient light) */
Lights1       light = gdSPDefLights1(  0,   0, 100,   /* Blue ambient light */
                                     255,   0,   0,   /* Red diffused light */
                                       0, 127,   0);  /* Direction toward diffused light */
#endif /*  defined(__LIGHTING__) */

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
  
#if        defined(__LIGHTING__)
  /* Perform lighting */
  
#if         !defined(__SMOOTH__)
  /* Perform flat shading */
  gSPSetGeometryMode(glistp++, G_SHADE | G_LIGHTING | G_CULL_BACK);
#else  /*    defined(__SMOOTH__) */
  /* Perform smooth shading */
  gSPSetGeometryMode(glistp++, G_SHADE | G_SHADING_SMOOTH | G_LIGHTING | G_CULL_BACK);
#endif /*   !defined(__SMOOTH__) */
  
  /* Color model using shade color */
  gDPSetCombineMode(glistp++, G_CC_SHADE, G_CC_SHADE);
  /* Light configuration */
  gSPSetLights1(glistp++, light);
  
#elif      defined(__PRIMITIVE__)
  /* Color with primitive color */
  
  /*
   *  Turn shading OFF for primitive color. 
   *  (This isn't actually necessary in this sample.) 
   */
  gSPClearGeometryMode(glistp++, G_SHADE);
  /* Back culling */
  gSPSetGeometryMode(glistp++, G_CULL_BACK);
  /* Color model using primitive color */
  gDPSetCombineMode(glistp++, G_CC_PRIMITIVE, G_CC_PRIMITIVE);
  
#else  /* !defined(__LIGHTING__) && !defined(__PRIMITIVE__) */
  /* Shading using vertex color */
  
#if         !defined(__SMOOTH__)
  /* Perform flat shading */
  gSPSetGeometryMode(glistp++, G_SHADE | G_CULL_BACK);
#else  /*    defined(__SMOOTH__) */
  /* Perform smooth shading */
  gSPSetGeometryMode(glistp++, G_SHADE | G_SHADING_SMOOTH | G_CULL_BACK);
#endif /*   !defined(__SMOOTH__) */
  
  /* Color model using shade color */
  gDPSetCombineMode(glistp++, G_CC_SHADE, G_CC_SHADE);
  
#endif /*  defined(__LIGHTING__)  */
  
  /* Set rendering mode */
  gDPSetRenderMode(glistp++, G_RM_OPA_SURF, G_RM_OPA_SURF2);
  
  /*
   *  Display list to define model 
   */
  
  /* Load model vertexes */
  gSPVertex(glistp++, cube_vtx, 8, 0);
  /* Create cube from loaded vertexes */
#if       !defined(__LIGHTING__) &&  defined(__PRIMITIVE__)
  /*
   *  When coloring with the primitive color, try assigning 
   *  a different primitive color to each face
   */
  
  /* Create face 1 */
  gDPSetPrimColor(glistp++, 0, 0, 255, 0, 0, 255);
  gSP2Triangles(glistp++, 4, 6, 7, 0, 4, 5, 6, 0);
  gDPPipeSync(glistp++);
  /* Create face 2 */
  gDPSetPrimColor(glistp++, 0, 0, 0, 255, 0, 255);
  gSP2Triangles(glistp++, 6, 5, 2, 1, 2, 5, 1, 1);
  gDPPipeSync(glistp++);
  /* Create face 3 */
  gDPSetPrimColor(glistp++, 0, 0, 0, 0, 255, 255);
  gSP2Triangles(glistp++, 1, 3, 2, 0, 0, 3, 1, 2);
  gDPPipeSync(glistp++);
  /* Create face 4 */
  gDPSetPrimColor(glistp++, 0, 0, 255, 255, 0, 255);
  gSP2Triangles(glistp++, 3, 0, 7, 1, 0, 4, 7, 0);
  gDPPipeSync(glistp++);
  /* Create face 5 */
  gDPSetPrimColor(glistp++, 0, 0, 255, 0, 255, 255);
  gSP2Triangles(glistp++, 2, 7, 6, 1, 2, 3, 7, 2);
  gDPPipeSync(glistp++);
  /* Create face 6 */
  gDPSetPrimColor(glistp++, 0, 0, 0, 255, 255, 255);
  gSP2Triangles(glistp++, 0, 5, 4, 1, 0, 1, 5, 2);
  gDPPipeSync(glistp++);
  
#else  /*  defined(__LIGHTING__) || !defined(__PRIMITIVE__) */
  /*
   *  When performing shading with the vertex color or light, 
   *  the color of the model is calculated from the vertex data. 
   */
  
  /* Create face 1 */
  gSP2Triangles(glistp++, 4, 6, 7, 0, 4, 5, 6, 0);
  /* Create face 2 */
  gSP2Triangles(glistp++, 6, 5, 2, 1, 2, 5, 1, 1);
  /* Create face 3 */
  gSP2Triangles(glistp++, 1, 3, 2, 0, 0, 3, 1, 2);
  /* Create face 4 */
  gSP2Triangles(glistp++, 3, 0, 7, 2, 0, 4, 7, 2);
  /* Create face 5 */
  gSP2Triangles(glistp++, 2, 7, 6, 0, 2, 3, 7, 0);
  /* Create face 6 */
  gSP2Triangles(glistp++, 0, 5, 4, 0, 0, 1, 5, 0);
  
#endif /*  defined(__LIGHTING__) */
}
