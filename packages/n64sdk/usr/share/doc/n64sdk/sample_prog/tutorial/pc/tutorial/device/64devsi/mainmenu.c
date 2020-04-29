/*******************************************************************
 *
 *  mainmenu.c
 *
 *******************************************************************/

#include  <nusys.h>

#include  "common.h"
#include  "mainmenu.h"

#include  "contmenu.h"
#include  "cpakmenu.h"
#include  "rmbmenu.h"

/********************* Static variables & prototypes **********************/

/* Main title and subtitle */
static s8 *   menuTitle    = "SI Device Test";
static s8 *   menuSubTitle = "Main Menu";

/* Main menu text strings */
static s8 *   menuStrings[] = {"Controller",
                               "Controller Pack",
                               "Motor"};

/************************** Source code ***************************/

/*
 *  Functions responsible for drawing menus 
 */
void  MainMenu(void)
{
  static u8       index = MENU_ID_CONT;
  
  /* Clear display screen */
  ClearDisplay();
  
  /* Read controller data */
  ReadController();
  
  if(menuID == MENU_ID_TOP) /* Main menu processing */
  {
    /* Display title */
    DrawTitle(menuTitle, menuSubTitle);
    
    /* Display menu and processing to move between items */
    index = DrawAndProcessMenu(index, 3, menuStrings);
    
    if(contData.trigger & A_BUTTON)   /* Use A button to move the submenu */
      menuID = index;
  }
  else /* if(menuID != MENU_ID_TOP) */
  {
    /* Branch to submenu processing */
    switch(menuID)
    {
    case MENU_ID_CONT:
      ContMenu();
      break;
    case MENU_ID_CPAK:
      CpakMenu();
      break;
    case MENU_ID_RMB:
      RmbMenu();
      break;
    }
  }
}
