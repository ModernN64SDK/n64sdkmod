/*******************************************************************
 *
 *  mainmenu.c
 *
 *******************************************************************/

#include  <nusys.h>

#include  "common.h"
#include  "mainmenu.h"

#include  "rommenu.h"
#include  "srammenu.h"

/********************* Static variables & prototypes **********************/

/* Main title and subtitle */
static s8 *   menuTitle    = "PI Device Test";
static s8 *   menuSubTitle = "Main Menu";

/* Main menu text strings */
static s8 *   menuStrings[] = {"ROM (DMA)",
                               "SRAM"};

/************************** Source code ***************************/

/*
 *  Functions responsible for drawing menus 
 */
void  MainMenu(void)
{
  static u8       index = MENU_ID_ROM;
  
  /* Clear display screen */
  ClearDisplay();
  
  /* Read controller data */
  ReadController();
  
  if(menuID == MENU_ID_TOP) /* Main menu processing */
  {
    /* Display title */
    DrawTitle(menuTitle, menuSubTitle);
    
    /* Display menu and processing to move between items */
    index = DrawAndProcessMenu(index, 2, menuStrings);
    
    if(contData.trigger & A_BUTTON)   /* Use A button to move the submenu */
      menuID = index;
  }
  else /* if(menuID != MENU_ID_TOP) */
  {
    /* Branch to submenu processing */
    switch(menuID)
    {
    case MENU_ID_ROM:
      RomMenu();
      break;
    case MENU_ID_SRAM:
      SramMenu();
      break;
    }
  }
}
