/*******************************************************************
 *
 *  menu.c
 *
 *******************************************************************/

#include  <nusys.h>

#include  "define.h"
#include  "menu.h"
#include  "sound.h"

NUContData    contData;

/********************* Static Variables and Prototypes **********************/

/* Viewport Information */
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
  gsDPSetScissor(G_SC_NON_INTERLACE, 0, 0, SCREEN_WD, SCREEN_HT),
  gsDPSetCombineKey(G_CK_NONE),
  gsDPSetAlphaCompare(G_AC_NONE),
  gsDPSetRenderMode(G_RM_NOOP, G_RM_NOOP2),
  gsDPSetColorDither(G_CD_DISABLE),
  gsDPPipeSync(),
  gsSPEndDisplayList(),
};

/* Display list */
static Gfx    glist[GLIST_LENGTH];
static Gfx *  glistp;

/* Menu text strings */
static s8   * menuString[] = {"SONG1 (%s)",
                              "SONG2 (Marker : %d)",
                              "SE1   (X = %3d, Z = %3d)",
                              "SE2   (Random 3D )",
                              "SE3   (Engine : %5d rpm)"};

static s8   * effectName[] = {"AL_FX_NONE",
                              "AL_FX_SMALLROOM",
                              "AL_FX_BIGROOM",
                              "AL_FX_CHORUS",
                              "AL_FX_FLANGE",
                              "AL_FX_ECHO"};

/* Function prototypes */
static void   ProcessController(u8 * index);
static void   ClearBackground(u8 r, u8 g, u8 b);

/************************** Source Code ***************************/

/*
 *  Menu drawing functions
 */
void  Menu(void)
{
  static u8       menuIndex = 0;
  s8              msgBuf[64];
  
  glistp = glist;
  /* Initialize RCP */
  gSPSegment(glistp++, 0, 0);
  gSPDisplayList(glistp++, rspinit_dl);
  gSPDisplayList(glistp++, rdpinit_dl);
  /* Clear background while changing color according to synchronization marker */
  switch(markerDone)
  {
  case 0:
    ClearBackground(0, 0, 0);
    break;
  case 1:
    ClearBackground(64, 32, 32);
    break;
  case 2:
    ClearBackground(32, 64, 32);
    break;
  case 3:
    ClearBackground(32, 32, 64);
    break;
  default:  /* This case is impossible */
    ClearBackground(255, 255, 255);
    break;
  }
  gDPFullSync(glistp++);
  gSPEndDisplayList(glistp++);
  /* Start task */
  nuGfxTaskStart(glist,
                 (s32)(glistp - glist) * sizeof(Gfx),
                 NU_GFX_UCODE_F3DEX, NU_SC_NOSWAPBUFFER);
  
  /* Clear debugging console */
  nuDebConClear(NU_DEB_CON_WINDOW0);
  
  /* Display title */
  nuDebConTextColor(NU_DEB_CON_WINDOW0, NU_DEB_CON_TEXT_LIGHTYELLOW);
  nuDebConTextPos(NU_DEB_CON_WINDOW0, 8, 2);
  nuDebConCPuts(NU_DEB_CON_WINDOW0, "N64 Sound Tools Sample 2");
  
  /* Draw menu items */
  nuDebConTextColor(NU_DEB_CON_WINDOW0,
                    menuIndex != MENU_SONG1 ? NU_DEB_CON_TEXT_LIGHTWHITE :
                                              NU_DEB_CON_TEXT_LIGHTRED);
  nuDebConTextPos(NU_DEB_CON_WINDOW0, 7, 6);
  sprintf(msgBuf, menuString[MENU_SONG1], effectName[effectType]);
  nuDebConCPuts(NU_DEB_CON_WINDOW0, msgBuf);
  nuDebConTextColor(NU_DEB_CON_WINDOW0,
                    menuIndex != MENU_SONG2 ? NU_DEB_CON_TEXT_LIGHTWHITE :
                                              NU_DEB_CON_TEXT_LIGHTRED);
  nuDebConTextPos(NU_DEB_CON_WINDOW0, 7, 7);
  sprintf(msgBuf, menuString[MENU_SONG2], markerNo);
  nuDebConCPuts(NU_DEB_CON_WINDOW0, msgBuf);
  nuDebConTextColor(NU_DEB_CON_WINDOW0,
                    menuIndex != MENU_SE1 ? NU_DEB_CON_TEXT_LIGHTWHITE :
                                            NU_DEB_CON_TEXT_LIGHTRED);
  nuDebConTextPos(NU_DEB_CON_WINDOW0, 7, 8);
  sprintf(msgBuf, menuString[MENU_SE1], effectX, effectZ);
  nuDebConCPuts(NU_DEB_CON_WINDOW0, msgBuf);
  nuDebConTextColor(NU_DEB_CON_WINDOW0,
                    menuIndex != MENU_SE2 ? NU_DEB_CON_TEXT_LIGHTWHITE :
                                            NU_DEB_CON_TEXT_LIGHTRED);
  nuDebConTextPos(NU_DEB_CON_WINDOW0, 7, 9);
  nuDebConCPuts(NU_DEB_CON_WINDOW0, menuString[MENU_SE2]);
  nuDebConTextColor(NU_DEB_CON_WINDOW0,
                    menuIndex != MENU_SE3 ? NU_DEB_CON_TEXT_LIGHTWHITE :
                                            NU_DEB_CON_TEXT_LIGHTRED);
  nuDebConTextPos(NU_DEB_CON_WINDOW0, 7, 10);
  sprintf(msgBuf, menuString[MENU_SE3], engine);
  nuDebConCPuts(NU_DEB_CON_WINDOW0, msgBuf);
  
  /* Display real time information */
  nuDebConTextColor(NU_DEB_CON_WINDOW0, NU_DEB_CON_TEXT_LIGHTWHITE);
  nuDebConTextPos(NU_DEB_CON_WINDOW0, 10, 15);
  sprintf(msgBuf, "SONG CH.  : %5d", songChannels);
  nuDebConCPuts(NU_DEB_CON_WINDOW0, msgBuf);
  nuDebConTextPos(NU_DEB_CON_WINDOW0, 10, 16);
  sprintf(msgBuf, "SE CH.    : %5d", seChannels);
  nuDebConCPuts(NU_DEB_CON_WINDOW0, msgBuf);
  nuDebConTextPos(NU_DEB_CON_WINDOW0, 10, 17);
  sprintf(msgBuf, "TOTAL CH. : %5d", totalChannels);
  nuDebConCPuts(NU_DEB_CON_WINDOW0, msgBuf);
  
  /* Display heap information */
  nuDebConTextPos(NU_DEB_CON_WINDOW0, 11, 24);
  sprintf(msgBuf, "TOTAL  : %6d", totalHeapSize);
  nuDebConCPuts(NU_DEB_CON_WINDOW0, msgBuf);
  nuDebConTextPos(NU_DEB_CON_WINDOW0, 11, 25);
  sprintf(msgBuf, "USED   : %6d", usedHeapSize);
  nuDebConCPuts(NU_DEB_CON_WINDOW0, msgBuf);
  nuDebConTextPos(NU_DEB_CON_WINDOW0, 11, 26);
  sprintf(msgBuf, "INIT   : %6d", initHeapSize);
  nuDebConCPuts(NU_DEB_CON_WINDOW0, msgBuf);
  nuDebConTextPos(NU_DEB_CON_WINDOW0, 11, 27);
  sprintf(msgBuf, "DATA   : %6d", dataHeapSize);
  nuDebConCPuts(NU_DEB_CON_WINDOW0, msgBuf);
  
  ProcessController(&menuIndex);
  
  /* Display debugging console */
  nuDebConDisp(NU_SC_SWAPBUFFER);
}

/*
 *  Main Menu controller processing 
 */
void   ProcessController(u8 * index)
{
  /* Get controller input status */
  nuContDataGetEx(&contData, 0);
  
  if(contData.trigger & D_JPAD)
  {
    if((*index)++ == MENU_SE3)
      *index = MENU_SONG1;
  }
  else if(contData.trigger & U_JPAD)
  {
    if((*index)-- == MENU_SONG1)
      *index = MENU_SE3;
  }
  
  /* Perform processing according to menu item */
  switch(*index)
  {
  case MENU_SONG1:
    /* Nothing specific */
    break;
  case MENU_SONG2:
    if(contData.trigger & L_JPAD)
      if(markerNo > 0)
        markerNo--;
    if(contData.trigger & R_JPAD)
      if(markerNo < MARK_COUNT - 1)
        markerNo++;
    break;
  case MENU_SE1:
    /* Nothing specific */
    break;
  case MENU_SE2:
    /* Nothing specific */
    break;
  case MENU_SE3:
    /* Nothing specific */
    break;
  }
  
  /* Perform MUS library related menu processing */
  SoundMenu(*index);
}

/*
 *  Fill background with specified color (add display list command)
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
  /* Size of fill rectangle (entire screen) */
  gDPFillRectangle(glistp++, 0, 0, SCREEN_WD - 1, SCREEN_HT - 1);
  gDPPipeSync(glistp++);
}
