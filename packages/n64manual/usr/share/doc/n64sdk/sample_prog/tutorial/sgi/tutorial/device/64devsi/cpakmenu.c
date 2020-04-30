/*******************************************************************
 *
 *  cpakmenu.c
 *
 *******************************************************************/

#include  <nusys.h>

#include  "common.h"

/* Hit point and magic point defaults values */
#define   DEFAULT_HIT_POINT       200
#define   DEFAULT_MAGIC_POINT     100

/* Game Notes data offset and size */
#define   GAME_DATA_OFFSET        0
#define   GAME_DATA_SIZE          8

/********************* Static Variables & Prototypes **********************/

/* Buffers used for Game Notes I/O */
#if       !defined(SGI)
u8              ioBuffer[GAME_DATA_SIZE] __attribute__((aligned(8)));
#else  /*  defined(SGI) */
u8              ioBuffer[GAME_DATA_SIZE];
#endif /* !defined(SGI) */

/* Main title and subtitle */
static s8 *     menuTitle    = "SI Device Test";
static s8 *     menuSubTitle = "Cpak Menu";

/* Controller Pak menu text strings */
static s8 *     menuStrings[] = {"Save",
                                 "Load",
#if        __ENABLE_DELETION__
                                 "Delete"
#endif /*  __ENABLE_DELETION__ */
                                 };

/* Message if Controller Pak was not found */
static s8       errString[] = "CPAK not found on port 0...";

/* Controller Pak structure */
static NUContPakFile    file;

/* Game Notes name */
s8              gameNoteName[] = "CHARACTER";

/* Hit point and magic point */
u8              hitPoint = DEFAULT_HIT_POINT,
                magicPoint = DEFAULT_MAGIC_POINT;

/* Function for performing special Controller Pak menu processing */
void            UpdateCpakParameters(void);

/* Function for performing special Controller Pak menu drawing */
void            ShowCpakParameters(void);

/* Error processing function (does nothing in this sample) */
void            HandlePfsError(s32 errorCode);

/************************** Source Code ***************************/

/*
 *  Controller Pak submenu functions 
 */
void    CpakMenu(void)
{
  static u8     index = 0;
  static u8     first = 1, opened = 0, plugged = 0;
  u32           noteExtension = 0;  /* Note extension (0) */
  
  /* Set Controller Pak code first time only */
  if(first)
  {
    nuContPakCodeSet("64", "TEST");
    first = 0;
  }
  
  /*
   * First time CpakMenu is called, 
   * and when CpakMenu is called again after removal, 
   * opens Controller Pak
   */
  if(!opened)
  {
    nuContPakOpen(&file, 0);
    if(file.type == NU_CONT_PAK_TYPE_PAK)
      plugged = 1;    /* Controller Pak detected */
    else
    {
      plugged = 0;    /* Controller Pak not found or open error */
      osSyncPrintf("nuContPakOpen : ");
      HandlePfsError(file.error);
    }
    opened = 1;
  }
  
  if(plugged)   /* Menu processing when Controller Pak detected */
  {
    /* Display title */
    DrawTitle(menuTitle, menuSubTitle);
    
    /* Display menu and processing to move between items */
#if        __ENABLE_DELETION__
    index = DrawAndProcessMenu(index, 3, menuStrings);
#else  /* !__ENABLE_DELETION__ */
    index = DrawAndProcessMenu(index, 2, menuStrings);
#endif /*  __ENABLE_DELETION__ */
    
    /* Update Controller Pak parameters (hit point and magic point) */
    UpdateCpakParameters();
    
    /* Display Controller Pak parameters (hit point and magic point) */
    ShowCpakParameters();
    
    /* Controller processing */
    if(contData.trigger & A_BUTTON)
    {
      /* Perform processing for current menu item while A button is pressed */
      switch(index)
      {
      case 0:     /* Save parameters */
        /* Open Controller Pak */
        if(nuContPakOpen(&file, 0))
        {
          /* Error processing */
          osSyncPrintf("nuContPakOpen : ");
          HandlePfsError(file.error);
          break;
        }
        /* Open Game Notes */
        if(nuContPakFileOpenJis(&file, gameNoteName, (u8 *)&noteExtension, NU_CONT_PAK_MODE_CREATE, 256))
        {
          if(file.error != PFS_ERR_EXIST)   /* Since there is no problem if Game Notes already exist, error processing is not performed */
          {
            /* Error processing */
            osSyncPrintf("nuContPakFileOpenJis : ");
            HandlePfsError(file.error);
            break;
          }
        }
        /* Copy parameters to I/O buffer */
        ioBuffer[0] = hitPoint;
        ioBuffer[1] = magicPoint;
        /* Write Game Notes */
        if(nuContPakFileFwrite(&file, GAME_DATA_OFFSET, GAME_DATA_SIZE, ioBuffer))
        {
          /* Error processing */
          osSyncPrintf("nuContPakFileFwrite : ");
          HandlePfsError(file.error);
          break;
        }
        break;
      
      case 1:     /* Load parameters */
        /* Open Controller Pak */
        if(nuContPakOpen(&file, 0))
        {
          /* Error processing */
          osSyncPrintf("nuContPakOpen : ");
          HandlePfsError(file.error);
          break;
        }
        /* Open Game Notes */
        if(nuContPakFileOpenJis(&file, gameNoteName, (u8 *)&noteExtension, NU_CONT_PAK_MODE_NOCREATE, 256))
        {
          /* Error processing */
          osSyncPrintf("nuContPakFileOpenJis : ");
          HandlePfsError(file.error);
          break;
        }
        /* Load Game Notes */
        if(nuContPakFileFread(&file, GAME_DATA_OFFSET, GAME_DATA_SIZE, ioBuffer))
        {
          /* Error processing */
          osSyncPrintf("nuContPakFileFread : ");
          HandlePfsError(file.error);
          break;
        }
        /* When read OK, copy parameters from I/O buffer */
        hitPoint = ioBuffer[0];
        magicPoint = ioBuffer[1];
        break;
      
#if        __ENABLE_DELETION__
      case 2:     /* Delete Game Notes */
        /* Open Controller Pak */
        if(nuContPakOpen(&file, 0))
        {
          /* Error processing */
          osSyncPrintf("nuContPakOpen : ");
          HandlePfsError(file.error);
          break;
        }
        /* Delete Game Notes */
        if(nuContPakFileDeleteJis(&file, gameNoteName, (u8 *)&noteExtension))
        {
          /* Error processing */
          osSyncPrintf("nuContPakFileDeleteJis : ");
          HandlePfsError(file.error);
          break;
        }
        break;
#endif /*  __ENABLE_DELETION__ */
      }
    }
  }
  else          /* Processing when there is no Controller Pak or on open error */
  {
    nuDebConTextColor(NU_DEB_CON_WINDOW0, NU_DEB_CON_TEXT_RED);
    nuDebConTextPos(NU_DEB_CON_WINDOW0, (NU_DEB_CON_ROW_MAX - strlen(errString)) >> 1, NU_DEB_CON_COLUMN_MAX >> 1);
    nuDebConCPuts(NU_DEB_CON_WINDOW0, errString);
  }
  
  /* Return to top menu if START button is pressed */
  if(contData.trigger & START_BUTTON)
  {
    menuID = MENU_ID_TOP;
    opened = 0;             /* Reopen on next call */
  }
}

/* Function for performing special Controller Pak menu processing */
void            UpdateCpakParameters(void)
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

/* Function to perform special Controller Pak menu drawing */
void            ShowCpakParameters(void)
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

/* Error processing functions (Do nothing in this sample) */
void            HandlePfsError(s32 errorCode)
{
  switch(errorCode)
  {
  case PFS_ERR_NOPACK:
    /* Processing if Controller Pak is not found is noted here */
    osSyncPrintf("PFS_ERR_NOPACK\n");
    break;
  case PFS_ERR_NEW_PACK:
    /* Processing if new Controller Pak is found is noted here */
    osSyncPrintf("PFS_ERR_NEW_PACK\n");
    break;
  case PFS_ERR_CONTRFAIL:
    /* Processing if Controller Pak is not properly inserted is noted here */
    osSyncPrintf("PFS_ERR_CONTRFAIL\n");
    break;
  case PFS_ERR_DEVICE:
    /* Processing if device other than Controller Pak is connected is noted here */
    osSyncPrintf("PFS_ERR_DEVICE\n");
    break;
  case PFS_ERR_ID_FATAL:
    /* Processing if there is an abnormality in the controller or the Pak is noted here */
    /* Try to restore or re-initialize*/
    osSyncPrintf("PFS_ERR_ID_FATAL\n");
    break;
  case PFS_ERR_INCONSISTENT:
    /* Processing if file system control area is corrupted is noted here */
    osSyncPrintf("PFS_ERR_INCONSISTENT\n");
    break;
  case PFS_ERR_EXIST:
    /* Processing if file already exists is noted here */
    osSyncPrintf("PFS_ERR_EXIST\n");
    break;
  case PFS_ERR_BAD_DATA:
    /* Processing when attempting to read data from Note that has not been written is noted here */
    osSyncPrintf("PFS_ERR_BAD_DATA\n");
    break;
  case PFS_ERR_INVALID:
    /* Processing when inavlid parameters were transferred or when file does not exist is noted here */
    osSyncPrintf("PFS_ERR_INVALID\n");
    break;
  case PFS_DATA_FULL:
    /* Processing when the Controller Pak does not have enough free space is noted here */
    osSyncPrintf("PFS_DATA_FULL\n");
    break;
  case PFS_DIR_FULL:
    /* Processing when the maximum number of Game Notes already exist is noted here */
    osSyncPrintf("PFS_DIR_FULL\n");
    break;
  }
}
