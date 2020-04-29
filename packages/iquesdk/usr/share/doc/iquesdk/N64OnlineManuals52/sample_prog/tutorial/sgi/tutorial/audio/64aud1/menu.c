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

/********************* Static variables and prototypes **********************/

/* Viewport information */
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

/* Menu text string */
static char * menuString[] = {"SONG", "SE  ", "F.O."};

/* Function prototype */
static void   ProcessController(u8 * index);
static void   ClearBackground(u8 r, u8 g, u8 b);

/************************** Source Code ***************************/

/*
 *  Function responsible for drawing the menu
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
  /* Clear background*/
  ClearBackground(0, 0, 0);
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
  nuDebConCPuts(NU_DEB_CON_WINDOW0, "N64 Sound Tools Sample 1");
  
  /* Draw menu items */
  nuDebConTextColor(NU_DEB_CON_WINDOW0,
                    menuIndex != MENU_SONG ? NU_DEB_CON_TEXT_LIGHTWHITE :
                                             NU_DEB_CON_TEXT_LIGHTRED);
  nuDebConTextPos(NU_DEB_CON_WINDOW0, 13, 6);
  sprintf(msgBuf, "%s : %5d", menuString[MENU_SONG], songNo);
  nuDebConCPuts(NU_DEB_CON_WINDOW0, msgBuf);
  nuDebConTextColor(NU_DEB_CON_WINDOW0,
                    menuIndex != MENU_SE ? NU_DEB_CON_TEXT_LIGHTWHITE :
                                           NU_DEB_CON_TEXT_LIGHTRED);
  nuDebConTextPos(NU_DEB_CON_WINDOW0, 13, 8);
  sprintf(msgBuf, "%s : %5d", menuString[MENU_SE], seNo);
  nuDebConCPuts(NU_DEB_CON_WINDOW0, msgBuf);
  nuDebConTextColor(NU_DEB_CON_WINDOW0,
                    menuIndex != MENU_FO ? NU_DEB_CON_TEXT_LIGHTWHITE :
                                           NU_DEB_CON_TEXT_LIGHTRED);
  nuDebConTextPos(NU_DEB_CON_WINDOW0, 13, 10);
  sprintf(msgBuf, "%s : %5d", menuString[MENU_FO], fadeFrames);
  nuDebConCPuts(NU_DEB_CON_WINDOW0, msgBuf);
  
  /* Display real time information */
  nuDebConTextColor(NU_DEB_CON_WINDOW0, NU_DEB_CON_TEXT_LIGHTWHITE);
  nuDebConTextPos(NU_DEB_CON_WINDOW0, 10, 13);
  sprintf(msgBuf, "SONG CH.  : %5d", songChannels);
  nuDebConCPuts(NU_DEB_CON_WINDOW0, msgBuf);
  nuDebConTextPos(NU_DEB_CON_WINDOW0, 10, 15);
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
 *  Main menu controller processing
 */
void   ProcessController(u8 * index)
{
  /* Get controller input status */
  nuContDataGetEx(&contData, 0);
  
  if(contData.trigger & D_JPAD)
  {
    if((*index)++ == MENU_FO)
      *index = MENU_SONG;
  }
  else if(contData.trigger & U_JPAD)
  {
    if((*index)-- == MENU_SONG)
      *index = MENU_FO;
  }
  
  /* Perform processing according to menu item */
  switch(*index)
  {
  case MENU_SONG:
    if(contData.trigger & L_JPAD || contData.trigger & R_JPAD)
    {
      songNo++;
      songNo %= 2;
    }
    break;
  case MENU_SE:
    if(contData.trigger & L_JPAD)
      if(seNo > 0)
        seNo--;
    if(contData.trigger & R_JPAD)
      if(seNo < GetNumberOfEffects() - 1)
        seNo++;
    break;
  case MENU_FO:
    if(contData.trigger & L_JPAD)
      if(fadeFrames > 0)
        fadeFrames--;
    if(contData.trigger & R_JPAD)
      if(fadeFrames < 65534)
        fadeFrames++;
    if(contData.trigger & L_CBUTTONS)
      if(fadeFrames > 9)
        fadeFrames -= 10;
    if(contData.trigger & R_CBUTTONS)
      if((fadeFrames += 10) > 65535)
        fadeFrames = 65535;
    break;
  }
  
  /* Perform MUS library related menu processing */
  SoundMenu(*index);
}

/*
 *  Fill background with specified color (Add display list command)
 */
void  ClearBackground(u8 r, u8 g, u8 b)
{
  gDPPipeSync(glistp++);
  /* Set fill mode to (G_CYC_FILL) */
  gDPSetCycleType(glistp++, G_CYC_FILL);
  /* Specify object of fill (frame buffer to be displayed next) */
  gDPSetColorImage(glistp++, G_IM_FMT_RGBA, G_IM_SIZ_16b, SCREEN_WD,
                   nuGfxCfb_ptr);
  /* Specify fill color (background color) */
  gDPSetFillColor(glistp++,
                  (GPACK_RGBA5551(r, g, b, 1) << 16 |
                   GPACK_RGBA5551(r, g, b, 1)));
  /* Size of fill rectangle (full screen) */
  gDPFillRectangle(glistp++, 0, 0, SCREEN_WD - 1, SCREEN_HT - 1);
  gDPPipeSync(glistp++);
}
