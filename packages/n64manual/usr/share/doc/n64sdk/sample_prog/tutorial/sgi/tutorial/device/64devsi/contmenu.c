/*******************************************************************
 *
 *  contmenu.c
 *
 *******************************************************************/

#include  <nusys.h>

#include  "common.h"

/* Button status display start row */
#define   PRESSED_LEFT        5

/* Trigger status display start row */
#define   TRIGGER_LEFT        23

/* Button, trigger status display start row */
#define   BUTTONS_TOP         10

/* Macros to display button status in debugging console */
#define   SHOW_BUTTON(but, y)  \
  nuDebConTextColor(NU_DEB_CON_WINDOW0, \
                    (contData.button & but) ? NU_DEB_CON_TEXT_LIGHTRED :\
                                              NU_DEB_CON_TEXT_WHITE);\
  nuDebConTextPos(NU_DEB_CON_WINDOW0, PRESSED_LEFT, y);\
  nuDebConCPuts(NU_DEB_CON_WINDOW0, #but);

/* Macros to display trigger status in debugging console */
#define   SHOW_TRIGGER(but, y)  \
  nuDebConTextColor(NU_DEB_CON_WINDOW0, \
                    (contData.trigger & but) ? NU_DEB_CON_TEXT_LIGHTRED :\
                                               NU_DEB_CON_TEXT_WHITE);\
  nuDebConTextPos(NU_DEB_CON_WINDOW0, TRIGGER_LEFT, y);\
  nuDebConCPuts(NU_DEB_CON_WINDOW0, #but);

/********************* Static Variables & Prototypes **********************/

/* Menu title and subtitle */
static s8 *   menuTitle    = "SI Device Test";
static s8 *   menuSubTitle = "Controller Menu";

/************************** Source Code ***************************/

/*
 *  Controller submenu (actually not in menu format) 
 */
void    ContMenu(void)
{
  s8                  msgBuf[128];
  
  /* Display title */
  DrawTitle(menuTitle, menuSubTitle);
  
  /*
   *  Display current button status 
   */
  nuDebConTextColor(NU_DEB_CON_WINDOW0, NU_DEB_CON_TEXT_LIGHTGREEN);
  nuDebConTextPos(NU_DEB_CON_WINDOW0, 3, 8);
  nuDebConCPuts(NU_DEB_CON_WINDOW0, "PRESSED");
  
  SHOW_BUTTON(L_TRIG, BUTTONS_TOP + 0);       /* L trigger */
  SHOW_BUTTON(R_TRIG, BUTTONS_TOP + 1);       /* R trigger */
  SHOW_BUTTON(Z_TRIG, BUTTONS_TOP + 2);       /* Z trigger */
  
  SHOW_BUTTON(A_BUTTON,   BUTTONS_TOP + 4);   /* A button */
  SHOW_BUTTON(B_BUTTON,   BUTTONS_TOP + 5);   /* B button */
  SHOW_BUTTON(U_CBUTTONS, BUTTONS_TOP + 6);   /* UC button */
  SHOW_BUTTON(L_CBUTTONS, BUTTONS_TOP + 7);   /* LC button */
  SHOW_BUTTON(D_CBUTTONS, BUTTONS_TOP + 8);   /* DC button */
  SHOW_BUTTON(R_CBUTTONS, BUTTONS_TOP + 9);   /* RC button */
  
  SHOW_BUTTON(U_JPAD, BUTTONS_TOP + 11);      /* U_JPAD button */
  SHOW_BUTTON(L_JPAD, BUTTONS_TOP + 12);      /* L_JPAD button */
  SHOW_BUTTON(D_JPAD, BUTTONS_TOP + 13);      /* D_JPAD button */
  SHOW_BUTTON(R_JPAD, BUTTONS_TOP + 14);      /* R_JPAD button */
  
  /*
   *  Trigger status 
   *    Since these are trigger data, they will only be displyed for an instant 
   *    when the button is pressed 
   */
  nuDebConTextColor(NU_DEB_CON_WINDOW0, NU_DEB_CON_TEXT_LIGHTGREEN);
  nuDebConTextPos(NU_DEB_CON_WINDOW0, 21, 8);
  nuDebConCPuts(NU_DEB_CON_WINDOW0, "TRIGGER");
  
  SHOW_TRIGGER(L_TRIG, BUTTONS_TOP + 0);     /* L trigger */
  SHOW_TRIGGER(R_TRIG, BUTTONS_TOP + 1);     /* R trigger */
  SHOW_TRIGGER(Z_TRIG, BUTTONS_TOP + 2);     /* Z trigger */
  
  SHOW_TRIGGER(A_BUTTON,   BUTTONS_TOP + 4); /* A button */
  SHOW_TRIGGER(B_BUTTON,   BUTTONS_TOP + 5); /* B button */
  SHOW_TRIGGER(U_CBUTTONS, BUTTONS_TOP + 6); /* UC button */
  SHOW_TRIGGER(L_CBUTTONS, BUTTONS_TOP + 7); /* LC button */
  SHOW_TRIGGER(D_CBUTTONS, BUTTONS_TOP + 8); /* DC button */
  SHOW_TRIGGER(R_CBUTTONS, BUTTONS_TOP + 9); /* RC button */
  
  SHOW_TRIGGER(U_JPAD, BUTTONS_TOP + 11);    /* U_JPAD button */
  SHOW_TRIGGER(L_JPAD, BUTTONS_TOP + 12);    /* L_JPAD button */
  SHOW_TRIGGER(D_JPAD, BUTTONS_TOP + 13);    /* D_JPAD button */
  SHOW_TRIGGER(R_JPAD, BUTTONS_TOP + 14);    /* R_JPAD button */
  
  /* Also display 3D stick status */
  nuDebConTextColor(NU_DEB_CON_WINDOW0, NU_DEB_CON_TEXT_WHITE);
  sprintf(msgBuf, "Stick X : %d", contData.stick_x);  /* x (horizontal) */
  nuDebConTextPos(NU_DEB_CON_WINDOW0, 14, 26);
  nuDebConCPuts(NU_DEB_CON_WINDOW0, msgBuf);
  sprintf(msgBuf, "Stick Y : %d", contData.stick_y);  /* y (vertical) */
  nuDebConTextPos(NU_DEB_CON_WINDOW0, 14, 27);
  nuDebConCPuts(NU_DEB_CON_WINDOW0, msgBuf);
  
  /* Return to top menu if START button is pressed */
  if(contData.trigger & START_BUTTON)
    menuID = MENU_ID_TOP;
}
