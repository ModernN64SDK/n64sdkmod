/*******************************************************************
 *
 *  srammenu.c
 *
 *******************************************************************/

#include  <nusys.h>

#include  "common.h"
#include  "srammenu.h"

/* Hit point and magic point default values */
#define   DEFAULT_HIT_POINT       200
#define   DEFAULT_MAGIC_POINT     100

/* Game data offset and size */
#define   GAME_DATA_ADDRESS       0x80000000
#define   GAME_DATA_SIZE          8

/********************* Static Variables & Prototypes **********************/

/* Buffers used for SRAM I/O */
#if       !defined(SGI)
u8            ioBuffer[GAME_DATA_SIZE] __attribute__((aligned(8)));
#else  /*  defined(SGI) */
u8            ioBuffer[GAME_DATA_SIZE];
#endif /* !defined(SGI) */

/* Menu title and subtitle */
static s8 *   menuTitle    = "PI Device Test";
static s8 *   menuSubTitle = "Sram Menu";

/* RumblePak menu text strings */
static s8 *   menuStrings[] = {"Save",
                               "Load"};

/* Hit point and magic point */
u8            hitPoint = DEFAULT_HIT_POINT,
              magicPoint = DEFAULT_MAGIC_POINT;

/* Function to perform special SRAM menu processing */
void          UpdateSramParameters(void);

/* Function to perform special SRAM menu drawing */
void          ShowSramParameters(void);

/************************** Source Code ***************************/
/*
 *  SRAM submenu functions 
 */
void    SramMenu(void)
{
  static u8       first = 1;
  static u8       index = 0;
  
  if(first)
  {
    nuPiInitSram();
    first = 0;
  }
  
  /* Display title */
  DrawTitle(menuTitle, menuSubTitle);
  
  /* Display menu and processing to move between items */
  index = DrawAndProcessMenu(index, 2, menuStrings);
  
  /* Update SRAM menu parameters (hit point and magic point) */
  UpdateSramParameters();
  
  /* Display SRAM menu parameters (hit point and magic point) */
  ShowSramParameters();
  
  if(contData.trigger & A_BUTTON)
  {
    /* Perform processing for current menu item while A button is pressed */
    switch(index)
    {
    case 0:     /* Save parameters */
      /* Copy parameters to buffer */
      ioBuffer[0] = hitPoint;
      ioBuffer[1] = magicPoint;
      /* Write from buffer to SRAM */
      nuPiWriteSram(GAME_DATA_ADDRESS, ioBuffer, GAME_DATA_SIZE);
      break;
    case 1:     /* Load parameters */
      /* Read from SRAM to buffer */
      nuPiReadSram(GAME_DATA_ADDRESS, ioBuffer, GAME_DATA_SIZE);
      /* Copy parameters from buffer */
      hitPoint = ioBuffer[0];
      magicPoint = ioBuffer[1];
      break;
    }
  }
  
  /* Return to top menu if START button is pressed */
  if(contData.trigger & START_BUTTON)
    menuID = MENU_ID_TOP;
}

/* Functions to perform special SRAM menu processing */
void            UpdateSramParameters(void)
{
  u8 *    param;
  
  /* Change magic point if Z trigger is being pressed, hit point if it is not */
  if(contData.button & Z_TRIG)
    param = &magicPoint;
  else /* if(!(contData.button & Z_TRIG)) */
    param = &hitPoint;
  
  if(contData.button & R_JPAD)
    /* R button (continuously change since button member is seen) */
    (*param)++;
  else if(contData.button & L_JPAD)
    /* L button (continuously change since button member is seen) */
    (*param)--;
  else if(contData.trigger & R_CBUTTONS)
    /* RC button (enable fine adjustment since trigger data are seen) */
    (*param)++;
  else if(contData.trigger & L_CBUTTONS)
    /* LC button (enable fine adjustment since trigger data are seen) */
    (*param)--;
}

/* Function to perform special SRAM menu drawing */
void            ShowSramParameters(void)
{
  /* Display parameters */
  nuDebConTextColor(NU_DEB_CON_WINDOW0, NU_DEB_CON_TEXT_LIGHTGREEN);
  sprintf(messageBuffer, "Hit Point   : %3d", hitPoint);
  nuDebConTextPos(NU_DEB_CON_WINDOW0, 12, 20);
  nuDebConCPuts(NU_DEB_CON_WINDOW0, messageBuffer);
  sprintf(messageBuffer, "Magic Point : %3d", magicPoint);
  nuDebConTextPos(NU_DEB_CON_WINDOW0, 12, 22);
  nuDebConCPuts(NU_DEB_CON_WINDOW0, messageBuffer);
}
