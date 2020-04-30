/*******************************************************************
 *
 *  rmbmenu.c
 *
 *******************************************************************/

#include  <nusys.h>

#include  "common.h"

/* RumblePak check interval (frame count) */
#define   RMB_CHECK_INTERVAL        180

/********************* Static Variables & Prototypes **********************/

/* Main title and subtitle  */
static s8 *   menuTitle    = "SI Device Test";
static s8 *   menuSubTitle = "Motor Menu";

/* RumblePak menu text strings */
static s8 *   menuStrings[] = {"Start",
                               "Stop"};

/* RumblePak frequency */
static u16    rmbFrequency = 256;

/* RumblePak duration (frame count) */
static s16    rmbDuration  = 300;

/* Function to perform special RumblePak menu processing */
void          UpdateRmbParameters(void);

/* Function to perform special RumblePak menu drawing */
void          ShowRmbParameters(void);

/************************** Source Code ***************************/

/* RumblePak related initialization functions */
void    RmbInit(void)
{
  /* Set RumblePak (port 0) to auto-recognize mode */
  nuContRmbModeSet(0, NU_CONT_RMB_MODE_AUTORUN);
  
  /* Set RumblePak check interval */
  nuContRmbSearchTimeSet(RMB_CHECK_INTERVAL);
}

/*
 *  RumblePak submenu functions 
 */
void    RmbMenu(void)
{
  static u8       index = 0;
  static s8       stopped = 0;  /* Rumble flag */
  
  /* Display title */
  DrawTitle(menuTitle, menuSubTitle);
  
  /* Display menu and processing to move between items */
  index = DrawAndProcessMenu(index, 2, menuStrings);
  
  /* Perform special RumblePak menu processing */
  UpdateRmbParameters();
  
  /* Perform special RumblePak menu drawing */
  ShowRmbParameters();
  
  /* Cancel if RumblePak is forcibly stopped */
  if(stopped)
  {
    nuContRmbForceStopEnd();
    stopped = 0;
  }
  
  /* Perform processing for each item if A button is pressed */
  if(contData.trigger & A_BUTTON)
    switch(index)
    {
    case 0:
      nuContRmbStart(0, rmbFrequency, (u16)rmbDuration);
      break;
    case 1:
      nuContRmbStop(0);
      break;
    }
  
  /* Use Z trigger to return to top menu */
  if(contData.trigger & START_BUTTON)
  {
    menuID = MENU_ID_TOP;
    nuContRmbForceStop();
    stopped = 1;
  }
  
  nuDebConTextColor(NU_DEB_CON_WINDOW0, NU_DEB_CON_TEXT_WHITE);
}

/* Functions to perform special RumblePak menu processing */
void          UpdateRmbParameters(void)
{
  u16 *     param;
  
  /* Change vibration time if Z trigger is being pressed, frequency if it is not */
  if(contData.button & Z_TRIG)
    param = (u16 *)&rmbDuration;
  else /* if(!(contData.button & Z_TRIG)) */
    param = &rmbFrequency;
  
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
  
  /* Limit frequency to values from 0 to 256 */
  rmbFrequency %= 257;
  
  /* Limit vibration time to greater than 0 */
  if(rmbDuration < 0)
    rmbDuration = 0;
}

/* Functions to perform special RumblePak menu drawing */
void          ShowRmbParameters(void)
{
  /* Display parameters */
  nuDebConTextColor(NU_DEB_CON_WINDOW0, NU_DEB_CON_TEXT_LIGHTGREEN);
  sprintf(messageBuffer, "Frequency : %3d", rmbFrequency);
  nuDebConTextPos(NU_DEB_CON_WINDOW0, 12, 20);
  nuDebConCPuts(NU_DEB_CON_WINDOW0, messageBuffer);
  sprintf(messageBuffer, "Duration  : %3d", rmbDuration);
  nuDebConTextPos(NU_DEB_CON_WINDOW0, 12, 22);
  nuDebConCPuts(NU_DEB_CON_WINDOW0, messageBuffer);
}
