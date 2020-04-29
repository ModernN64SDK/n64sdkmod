/*******************************************************************
 *
 *  graphic.c
 *
 *******************************************************************/

#include  <nusys.h>

#include  "define.h"

/* Viewpoint information */
static  Vp  viewport = {
  SCREEN_WD * 2, SCREEN_HT * 2, G_MAXZ / 2, 0,
  SCREEN_WD * 2, SCREEN_HT * 2, G_MAXZ / 2, 0,
};

/* RSP initialization display list */
Gfx rspinit_dl[] = {
  gsSPViewport(&viewport),
  gsSPClearGeometryMode(G_SHADE | G_SHADING_SMOOTH | G_CULL_BOTH |
    G_FOG | G_LIGHTING | G_TEXTURE_GEN |
    G_TEXTURE_GEN_LINEAR | G_LOD),
  gsSPTexture(0, 0, 0, 0, G_OFF),
  gsSPEndDisplayList(),
};

/* RDP initialization display list */
Gfx rdpinit_dl[] = {
  gsDPSetCycleType(G_CYC_1CYCLE),
  gsDPSetScissor(G_SC_NON_INTERLACE, 0, 0, SCREEN_WD, SCREEN_HT),
  gsDPSetCombineKey(G_CK_NONE),
  gsDPSetAlphaCompare(G_AC_NONE),
  gsDPSetRenderMode(G_RM_NOOP, G_RM_NOOP2),
  gsDPSetColorDither(G_CD_DISABLE),
  gsDPPipeSync(),
  gsSPEndDisplayList(),
};

/* Display list */
Gfx           glist[GLIST_LENGTH];
Gfx *         glistp;

/* Flag indicating whether or not Controller 1 is connected */
u8            IsPlugged = 0;

static void   ClearBackground(u8 r, u8 g, u8 b);
static void   DrawRectangle(int Color);
static void   ShowMenu(int Color);
static void   HandleController(u8 * IsMenuMode, int * Color);

/* Functions responsible for drawing */
void  Draw(void)
{
  /* Flag indicating whether or not system is in Menu mode */
  static u8     IsMenuMode = 1;
  /* Color of rectangle to be drawn (0 -> R, 1 -> G, 2 -> B) */
  static int    RectColor = 0;
  
  glistp = glist;
  
  /* Initialize RCP */
  gSPSegment(glistp++, 0, 0);
  gSPDisplayList(glistp++, OS_K0_TO_PHYSICAL(rspinit_dl));
  gSPDisplayList(glistp++, OS_K0_TO_PHYSICAL(rdpinit_dl));
  
  /* Insert display list command to clear background */
  ClearBackground(0, 0, 0);
  
  /* Add display list command to draw menu display rectangle */
  if(!IsMenuMode)
    DrawRectangle(RectColor);
  
  /* End display list creation */
  gDPFullSync(glistp++);
  gSPEndDisplayList(glistp++);
  
  /* Start task to draw rectangle (no swapping) */
  nuGfxTaskStart(glist,
                 (s32)(glistp - glist) * sizeof(Gfx),
                 NU_GFX_UCODE_F3DEX, NU_SC_NOSWAPBUFFER);
  
  /* If not Menu mode, display menu */
  if(IsMenuMode)
  {
    ShowMenu(RectColor);
    nuDebConDisp(NU_SC_NOSWAPBUFFER);
  }
  
  /* Controller processing */
  HandleController(&IsMenuMode, &RectColor);
  
  /* Display performance meter */
  nuDebTaskPerfBar1(1, 210, NU_SC_SWAPBUFFER);
}

/*
 * Fill background with specified color (Add display list command) 
 */
void  ClearBackground(u8 r, u8 g, u8 b)
{
  gDPPipeSync(glistp++);
  /* Set fill mode to (G_CYC_FILL) */
  gDPSetCycleType(glistp++, G_CYC_FILL);
  /* Specify object of fill (frame buffer to be displayed next) */
  gDPSetColorImage(glistp++, G_IM_FMT_RGBA, G_IM_SIZ_16b, SCREEN_WD,
                   osVirtualToPhysical(nuGfxCfb_ptr));
  /* Specify fill color (background color) */
  gDPSetFillColor(glistp++,
                  (GPACK_RGBA5551(r, g, b, 1) << 16 |
                   GPACK_RGBA5551(r, g, b, 1)));
  /* Size of rectangle to be filled (entire screen) */
  gDPFillRectangle(glistp++, 0, 0, SCREEN_WD - 1, SCREEN_HT - 1);
  gDPPipeSync(glistp++);
}

/*
 * Function to draw blue rectangle.  Drawing position is gradually changed.
 */
void  DrawRectangle(int Color)
{
  /* Coordinates of upper-left corner of blue rectangle */
  static int      x = 0, y = 0;
  
  /* Specify fill color (moving rectangle, blue) */
  switch(Color)
  {
  case 0:
    gDPSetFillColor(glistp++,
                    (GPACK_RGBA5551(255, 0, 0, 1) << 16 |
                     GPACK_RGBA5551(255, 0, 0, 1)));
    break;
  case 1:
    gDPSetFillColor(glistp++,
                    (GPACK_RGBA5551(0, 255, 0, 1) << 16 |
                     GPACK_RGBA5551(0, 255, 0, 1)));
    break;
  case 2:
    gDPSetFillColor(glistp++,
                    (GPACK_RGBA5551(0, 0, 255, 1) << 16 |
                     GPACK_RGBA5551(0, 0, 255, 1)));
    break;
  }
  
  /* Processing when blue rectangle goes outside of screen */
  if(x >= SCREEN_WD)
    x = 0;
  if(y >= SCREEN_HT)
    y = 0;
    
  /*
   * Display blue rectangle split vertically when it runs off bottom of screen. 
   * This will make the rectangle look like it is vertically linked. 
   * Horizontal splitting is not performed as the code becomes very complex. 
   */
  if(y + 100 >= SCREEN_HT)
  {
    /* Draw two rectangles when the rectangle is split */
    gDPFillRectangle(glistp++, x, y, x + 100, SCREEN_HT - 1);
    gDPPipeSync(glistp++);
    gDPFillRectangle(glistp++, x++, 0, x + 100, (y++) + 100 - SCREEN_HT);
  }
  else
  {
    /* One rectangle is sufficient if the rectangle does not run off */
    gDPFillRectangle(glistp++, x++, y++, x + 100, y + 100);
  }
  gDPPipeSync(glistp++);
}

void  ShowMenu(int Color)
{
  /* Clear debugging console */
  nuDebConClear(NU_DEB_CON_WINDOW0);
  
  if(!IsPlugged)  /* Display message to connect Controller 1 */
  {
    /* Specify blinking text attributes */
    nuDebConTextAttr(NU_DEB_CON_WINDOW0, NU_DEB_CON_ATTR_BLINK);
    /* Set text color (white) */
    nuDebConTextColor(NU_DEB_CON_WINDOW0, NU_DEB_CON_TEXT_WHITE);
    /* Set text position */
    nuDebConTextPos(NU_DEB_CON_WINDOW0, 10, 10);
    /* Display text (Display by swapping next frame buffer) */
    nuDebConCPuts(NU_DEB_CON_WINDOW0, "CONNECT CONTROLLER 1");
    return;
  }
  
  /* Set text color (white) */
  nuDebConTextColor(NU_DEB_CON_WINDOW0, NU_DEB_CON_TEXT_WHITE);
  /* Set text position */
  nuDebConTextPos(NU_DEB_CON_WINDOW0, 13, 14);
  /* Display text (Display by swapping next frame buffer) */
  nuDebConCPuts(NU_DEB_CON_WINDOW0, "COLOR : ");
  
  switch(Color)
  {
  case 0:
    /* Set text color (red) */
    nuDebConTextColor(NU_DEB_CON_WINDOW0, NU_DEB_CON_TEXT_RED);
    /* Display text (Display by swapping next frame buffer) */
    nuDebConCPuts(NU_DEB_CON_WINDOW0, "RED");
    break;
  case 1:
    /* Set text color (green) */
    nuDebConTextColor(NU_DEB_CON_WINDOW0, NU_DEB_CON_TEXT_GREEN);
    /* Display text (Display by swapping next frame buffer) */
    nuDebConCPuts(NU_DEB_CON_WINDOW0, "GREEN");
    break;
  case 2:
    /* Set text color (blue) */
    nuDebConTextColor(NU_DEB_CON_WINDOW0, NU_DEB_CON_TEXT_BLUE);
    /* Display text (Display by swapping next frame buffer) */
    nuDebConCPuts(NU_DEB_CON_WINDOW0, "BLUE");
    break;
  }
}

void  HandleController(u8 * IsMenuMode, int * Color)
{
  static NUContData   contData;
  
  /* Get controller input status */
  nuContDataGetEx(&contData, 0);
  
  if(*IsMenuMode)
  {
    /* Select color of rectangle (select with left/right of cross-key button) */
    if(contData.trigger & L_JPAD)
    {
      if(--*Color < 0)
        *Color = 0;
    }
    else if(contData.trigger & R_JPAD)
    {
      if(++*Color > 2)
        *Color = 2;
    }
  }
  
  /* Toggle Menu mode when START button is pressed */
  if(contData.trigger & START_BUTTON)
    *IsMenuMode ^= 1;
}
