/*******************************************************************
 *
 *  ddmenu.c
 *
 *******************************************************************/

#include  <nusys.h>

#include  <mfs.h>

#include  "define.h"
#include  "ddmenu.h"

/* Number of lines of texture to be simultaneously loaded, drawn */
#define     ROWS      3

/* Macro for displaying macro constants corresponding to error code */
#define     SHOW_ERR(err)   \
  case err:\
    osSyncPrintf(#err "\n");\
    break;

NUContData      contData;
MfsFileHandle   fileHandle[MFS_HANDLES];
u8              failed = 0;

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
  gsDPSetScissor(G_SC_NON_INTERLACE, 0, 0, SCREEN_WD, SCREEN_HT),
  gsDPSetCombineKey(G_CK_NONE),
  gsDPSetAlphaCompare(G_AC_NONE),
  gsDPSetRenderMode(G_RM_NOOP, G_RM_NOOP2),
  gsDPSetColorDither(G_CD_DISABLE),
  gsSPEndDisplayList(),
};

/* Number of pictures */
static s32        pictureCount = 11;

/* Names of pictures */
static s8   *     pictureName[] = {
  "A:/FOREST1.RGB",
  "A:/FOREST2.RGB",
  "A:/FOREST3.RGB",
  "A:/SNOW1.RGB",
  "A:/SNOW2.RGB",
  "A:/SNOW3.RGB",
  "A:/UNIV1.RGB",
  "A:/UNIV2.RGB",
  "A:/UNIV3.RGB",
  "A:/UNIV4.RGB",
  "A:/UNIV5.RGB",
};

/* Display list */
static Gfx    glist[GLIST_LENGTH];
static Gfx *  glistp;

/* Picture storage array */
static u16    picture[SCREEN_WD * SCREEN_HT];

/* Function prototypes */
static void   ProcessController(u8 * showMenu, s32 * index);
static void   ClearBackground(u8 r, u8 g, u8 b);
static void   DrawBackground(void);

/************************** Source Code ***************************/

/*
 * Functions responsible for drawing menus
 */
void  DdMenu(void)
{
  static s32    menuIndex = -1;
  static u8     showMenu = 1;
  s32           i;
  
  glistp = glist;
  /* Initialize RCP */
  gSPSegment(glistp++, 0, 0);
  gSPDisplayList(glistp++, rspinit_dl);
  gSPDisplayList(glistp++, rdpinit_dl);
  /* Clear background */
  ClearBackground(0, 0, 0);
  /* Draw background */
  DrawBackground();
  gDPFullSync(glistp++);
  gSPEndDisplayList(glistp++);
  /* Start task */
  nuGfxTaskStart(glist,
                 (s32)(glistp - glist) * sizeof(Gfx),
                 NU_GFX_UCODE_F3DEX, NU_SC_NOSWAPBUFFER);
  
  /* Clear debugging console */
  nuDebConClear(NU_DEB_CON_WINDOW0);
  
  if(showMenu)
  {
    /* Display title */
    nuDebConTextColor(NU_DEB_CON_WINDOW0, NU_DEB_CON_TEXT_LIGHTYELLOW);
    nuDebConTextPos(NU_DEB_CON_WINDOW0, 11, 2);
    nuDebConCPuts(NU_DEB_CON_WINDOW0, "PI Device Test (DD)");
    
    /* Menu processing */
    nuDebConTextColor(NU_DEB_CON_WINDOW0, NU_DEB_CON_TEXT_LIGHTBLUE);
    nuDebConTextPos(NU_DEB_CON_WINDOW0, 15, 4);
    nuDebConCPuts(NU_DEB_CON_WINDOW0, "Main Menu");
    for(i = 0; i < pictureCount; i++)
    {
      nuDebConTextColor(NU_DEB_CON_WINDOW0,
                        menuIndex != i ? NU_DEB_CON_TEXT_LIGHTWHITE :
                                         NU_DEB_CON_TEXT_LIGHTRED);
      nuDebConTextPos(NU_DEB_CON_WINDOW0, 12, 7 + (i << 1));
      nuDebConCPuts(NU_DEB_CON_WINDOW0, pictureName[i]);
    }
  }
  ProcessController(&showMenu, &menuIndex);
  
  /* Display debugging console */
  nuDebConDisp(NU_SC_SWAPBUFFER);
}

/* MFS error processing functions (doesn't actually do anything) */
void    HandleMfsError(s32 errorCode)
{
  switch(errorCode)
  {
  SHOW_ERR(MFS_ERR_ARG);
  SHOW_ERR(MFS_ERR_DEVICE);
  SHOW_ERR(MFS_ERR_NOINIT);
  SHOW_ERR(MFS_ERR_NAME);
  SHOW_ERR(MFS_ERR_NOTFOUND);
  SHOW_ERR(MFS_ERR_FULL);
  SHOW_ERR(MFS_ERR_FILEEXIST);
  SHOW_ERR(MFS_ERR_PROTECT);
  SHOW_ERR(MFS_ERR_REFUSE);
  SHOW_ERR(MFS_ERR_HEADER);
  SHOW_ERR(MFS_ERR_SEARCHEND);
  SHOW_ERR(MFS_ERR_FATAL);
  SHOW_ERR(MFS_ERR_NOTMFSDISK);
  SHOW_ERR(MFS_ERR_DISKCHANGE);
  SHOW_ERR(MFS_ERR_VERSION);
  default:
    osSyncPrintf("UNKNOWN ERROR\n");
  }
}

/*
 *  Main menu controller processing 
 */
void    ProcessController(u8 * showMenu, s32 * index)
{
  MfsFile         file;
  s32             errorCode;
  
  /* Get controller input status */
  nuContDataGetEx(&contData, 0);
  
  /* Toggle menu display with START button */
  if(contData.trigger & START_BUTTON)
    *showMenu ^= 1;
  
  if(!*showMenu)
    return;
  
  if(contData.trigger & D_JPAD)
  {
    if((*index)++ == pictureCount - 1)
      *index = 0;
  }
  else if(contData.trigger & U_JPAD)
  {
    if((*index)-- <= 0)
      *index = pictureCount - 1;
  }
  
  /* Change background if A button is pressed */
  if((contData.trigger & A_BUTTON) && !failed && *index != -1)
  {
    /* Open file */
    if(errorCode = mfsHFopen(&file, pictureName[*index], MFS_OPEN_READ))
    {
      /* Error processing */
      osSyncPrintf("mfsHFopen : ");
      HandleMfsError(errorCode);
      return;
    }
    /* Load file */
    /* mfsHFread refers to the error code when the returned value is a negative number */
    if((errorCode = mfsHFread(file, picture, SCREEN_WD * SCREEN_HT * sizeof(u16))) < 0)
    {
      /* Error processing */
      osSyncPrintf("mfsHFread : ");
      HandleMfsError(errorCode);
      return;
    }
    /* Close file */
    if(errorCode = mfsHFclose(file))
    {
      /* Error processing */
      osSyncPrintf("mfsHFclose : ");
      HandleMfsError(errorCode);
      return;
    }
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
  /* Specify fill color (background color) 
  gDPSetFillColor(glistp++,
                  (GPACK_RGBA5551(r, g, b, 1) << 16 |
                   GPACK_RGBA5551(r, g, b, 1)));
  /* Size of rectangle to be filled (entire screen) 
  gDPFillRectangle(glistp++, 0, 0, SCREEN_WD - 1, SCREEN_HT - 1);
}

/* Function to draw background (add display list command) */
void    DrawBackground(void)
{
  s32         i;
  
  gDPSetCycleType(glistp++, G_CYC_COPY);
  gDPSetCombineLERP(glistp++, 0, 0, 0, 0,
                              0, 0, 0, 0,
                              0, 0, 0, 0,
                              0, 0, 0, 0);
  gDPSetRenderMode(glistp++, G_RM_NOOP, G_RM_NOOP2);
  gDPSetTexturePersp(glistp++, G_TP_NONE);
  for(i = 0; i < (SCREEN_HT / ROWS); i++)
  {
    gDPLoadTextureTile(glistp++,
                       picture,
                       G_IM_FMT_RGBA,
                       G_IM_SIZ_16b,
                       SCREEN_WD,
                       SCREEN_HT,
                       0,
                       i * ROWS,
                       SCREEN_WD - 1,
                       i * ROWS + (ROWS - 1),
                       0,
                       G_TX_WRAP, G_TX_WRAP,
                       0, 0,
                       G_TX_NOLOD, G_TX_NOLOD);
    gSPTextureRectangle(glistp++,
                        0 << 2,
                        (i * ROWS) << 2,
                        (SCREEN_WD - 1) << 2,
                        (i * ROWS + (ROWS - 1)) << 2,
                        G_TX_RENDERTILE,
                        0 << 5, (i * ROWS) << 5,
                        4 << 10, 1 << 10);
  }
}
