/*******************************************************************
 *
 *  common.c
 *
 *******************************************************************/

#include  <nusys.h>

#include  "common.h"

/* Structure shared by modules requiring controller information */
NUContData      contData;

/* ID to identify current menu */
s32             menuID = MENU_ID_TOP;

/* Shared buffer used by sprintf, etc. */
s8              messageBuffer[128];

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
  gsSPEndDisplayList(),
};

/* Display list */
static Gfx    glist[GLIST_LENGTH];
static Gfx *  glistp;

/* Function prototype */
static void   ClearBackground(u8 r, u8 g, u8 b);

/************************** Source Code ***************************/

/*
 *  Function to clear display screen
 */
void  ClearDisplay(void)
{
  glistp = glist;
  gSPSegment(glistp++, 0, 0);
  gSPDisplayList(glistp++, rspinit_dl); /* Initialize RSP */
  gSPDisplayList(glistp++, rdpinit_dl); /* Initialize RDP */
  ClearBackground(0, 0, 0); /* Clear background */
  gDPFullSync(glistp++);
  gSPEndDisplayList(glistp++);
  /* Start task */
  nuGfxTaskStart(glist,
                 (s32)(glistp - glist) * sizeof(Gfx),
                 NU_GFX_UCODE_F3DEX, NU_SC_NOSWAPBUFFER);
  /* Clear debugginer console */
  nuDebConClear(NU_DEB_CON_WINDOW0);
}

/*
 *  Function to read the controller status 
 */
void  ReadController(void)
{
  /* Get controller input status */
  nuContDataGetEx(&contData, 0);
}

/*
 *  Draw menu items and controller processing to move between items
 */
s32   DrawAndProcessMenu(s32 index, u32 count, s8 ** items)
{
  s32     i;
  
  /* Move between menu items */
  if(contData.trigger & D_JPAD)
  {
    if(index++ == count - 1)
      index = 0;
  }
  else if(contData.trigger & U_JPAD)
  {
    if(index-- == 0)
      index = count - 1;
  }
  
  /* Draw menu */
  for(i = 0; i < count; i++)
  {
    nuDebConTextPos(NU_DEB_CON_WINDOW0, MENU_LEFT_COLUMN, MENU_TOP_ROW + (i << 1));
    /* Selected menu is red, all others are white */
    nuDebConTextColor(NU_DEB_CON_WINDOW0,
                      index == i ? NU_DEB_CON_TEXT_LIGHTRED :
                                   NU_DEB_CON_TEXT_LIGHTWHITE);
    nuDebConCPuts(NU_DEB_CON_WINDOW0, items[i]);
  }
  
  return index;
}

/*
 *  Function to display menu title and sub-titles 
 *  do centering
 */
void  DrawTitle(s8 * title, s8 * subtitle)
{
  s32     column;
  
  if(title)       /*  Display if not NULL */
  {
    /* Please note that NU_DEB_CON_ROW_MAX means the maximum value for row numbers */
    column = (NU_DEB_CON_ROW_MAX - strlen(title)) >> 1;
    nuDebConTextColor(NU_DEB_CON_WINDOW0, NU_DEB_CON_TEXT_LIGHTYELLOW);
    nuDebConTextPos(NU_DEB_CON_WINDOW0, column, TITLE_ROW);
    nuDebConCPuts(NU_DEB_CON_WINDOW0, title);
  }
  
  if(subtitle)    /* Display if not NULL */
  {
    /* Please note that NU_DEB_CON_ROW_MAX means the maximum value for row numbers */
    column = (NU_DEB_CON_ROW_MAX - strlen(subtitle)) >> 1;
    nuDebConTextColor(NU_DEB_CON_WINDOW0, NU_DEB_CON_TEXT_LIGHTBLUE);
    nuDebConTextPos(NU_DEB_CON_WINDOW0, column, SUBTITLE_ROW);
    nuDebConCPuts(NU_DEB_CON_WINDOW0, subtitle);
  }
}

/*
 *  Fill background with specified color (add display list command)
 */
void  ClearBackground(u8 r, u8 g, u8 b)
{
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
}
