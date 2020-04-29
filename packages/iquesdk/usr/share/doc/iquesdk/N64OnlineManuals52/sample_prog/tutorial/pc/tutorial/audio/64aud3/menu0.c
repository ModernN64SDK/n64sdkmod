/*******************************************************************
 *
 *  menu0.c
 *
 *******************************************************************/

#include  "menu.h"
#include  "sound.h"

/********************* Static variables and prototypes **********************/

/* Menu text string */
static s8   * menuString[] = {"START GAME",
                              "OPTIONS"};

/* Function prototype */
static void   ProcessController(u8 * index);

/************************** Source code ***************************/

/*
 *  Functions responsible for drawing menu 
 */
void  Menu0(void)
{
  static s8       msgBuf[64];
  static u8       menuIndex = 0;
  
  glistp = glist;
  /* Initialize RCP */
  gSPSegment(glistp++, 0, 0);
  gSPDisplayList(glistp++, rspinit_dl);
  gSPDisplayList(glistp++, rdpinit_dl);
  ClearBackground(0, 0, 0);
  gDPFullSync(glistp++);
  gSPEndDisplayList(glistp++);
  /* Start task */
  nuGfxTaskStart(glist,
                 (s32)(glistp - glist) * sizeof(Gfx),
                 NU_GFX_UCODE_F3DEX, NU_SC_NOSWAPBUFFER);
  
  /* Clear debuggin console */
  nuDebConClear(NU_DEB_CON_WINDOW0);
  
  /* Display title  */
  nuDebConTextColor(NU_DEB_CON_WINDOW0, NU_DEB_CON_TEXT_LIGHTYELLOW);
  nuDebConTextPos(NU_DEB_CON_WINDOW0, 7, 2);
  nuDebConCPuts(NU_DEB_CON_WINDOW0, "N64 Sound Tools Sample 3-0");
  
  /* Draw menu items */
  nuDebConTextColor(NU_DEB_CON_WINDOW0,
                    menuIndex != MENU0_START ? NU_DEB_CON_TEXT_LIGHTWHITE :
                                               NU_DEB_CON_TEXT_LIGHTRED);
  nuDebConTextPos(NU_DEB_CON_WINDOW0, 12, 6);
  nuDebConCPuts(NU_DEB_CON_WINDOW0, menuString[MENU0_START]);
  nuDebConTextColor(NU_DEB_CON_WINDOW0,
                    menuIndex != MENU0_OPTIONS ? NU_DEB_CON_TEXT_LIGHTWHITE :
                                                 NU_DEB_CON_TEXT_LIGHTRED);
  nuDebConTextPos(NU_DEB_CON_WINDOW0, 12, 7);
  nuDebConCPuts(NU_DEB_CON_WINDOW0, menuString[MENU0_OPTIONS]);
  
  if(optionsMode)
  {
    nuDebConTextColor(NU_DEB_CON_WINDOW0, NU_DEB_CON_TEXT_LIGHTWHITE);
    nuDebConTextPos(NU_DEB_CON_WINDOW0, 12, 8);
    nuDebConCPuts(NU_DEB_CON_WINDOW0, "SE 1 : L BUTTON");
    nuDebConTextPos(NU_DEB_CON_WINDOW0, 12, 9);
    nuDebConCPuts(NU_DEB_CON_WINDOW0, "SE 2 : R BUTTON");
  }
  
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
 *  Main menu controller processing 
 */
void   ProcessController(u8 * index)
{
  /* Get controller input status */
  nuContDataGetEx(&contData, 0);
  
  if(contData.trigger & D_JPAD)
  {
    if((*index)++ == MENU0_OPTIONS)
      *index = MENU0_START;
  }
  else if(contData.trigger & U_JPAD)
  {
    if((*index)-- == MENU0_START)
      *index = MENU0_OPTIONS;
  }
  
  /* Perform processing according to menu item */
  switch(*index)
  {
  case MENU0_START:
    /* Nothing specific */
    break;
  case MENU0_OPTIONS:
    /* Nothing specific */
    break;
  }
  
  /* Perform MUS library related menu processing */
  SoundMenu0(*index);
}
