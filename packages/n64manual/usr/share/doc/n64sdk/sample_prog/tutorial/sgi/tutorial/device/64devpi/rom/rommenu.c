/*******************************************************************
 *
 *  rommenu.c
 *
 *******************************************************************/

#include  <nusys.h>

#include  "common.h"
#include  "rommenu.h"

/* Address to which to load PCM waveform data */
#define   PCM_BUFFER_ADDRESS    0x80200000

/********************* Static Variables & Prototypes **********************/

/* Menu title and subtitle */
static s8 *   menuTitle    = "PI Device Test";
static s8 *   menuSubTitle = "Rom Menu";

/* RumblePak menu text strings */
static s8 *   menuStrings[] = {"Play DAIGORO.PCM",
                               "Play DAISONG.PCM"};

/* Pointer to PCM waveform data buffer */
static u8 *   pcmBuffer = (u8 *)PCM_BUFFER_ADDRESS;

/* ROM addresses used by DMA */
extern u8     _pcmSegmentRomStart[],
              _pcmSegmentRomEnd[];
extern u8     _songSegmentRomStart[],
              _songSegmentRomEnd[];

/* Display PCM waveform playback status */
void    ShowPCMStatus(void);

/************************** Source Code ***************************/

/*
 *  ROM DMA submenu functions 
 */
void    RomMenu(void)
{
  static u8       index = 0;
  
  /* Display title */
  DrawTitle(menuTitle, menuSubTitle);
  
  /* Display menu and processing to move between items */
  index = DrawAndProcessMenu(index, 2, menuStrings);
  
  if(contData.trigger & A_BUTTON)
  {
    if(index == 0)              /* Play DAIGORO.PCM */
    {
      if(osAiGetLength() == 0)    /* If not already being played by AI... */
      {
        /* Transfer PCM data to RDRAM */
        nuPiReadRom((u32)_pcmSegmentRomStart,
                    pcmBuffer,
                    (u32)(_pcmSegmentRomEnd - _pcmSegmentRomStart));
        
        /* Set playback frequency for PCM data */
        osAiSetFrequency(22050);
        
        /* Play PCM data */
        osAiSetNextBuffer(pcmBuffer, (u32)(_pcmSegmentRomEnd - _pcmSegmentRomStart));
      }
    }
    else /* if(index == 1) */   /* Play DAISONG.PCM */
    {
      if(osAiGetLength() == 0)    /* If not already being played by AI... */
      {
        /* Transfer PCM data to RDRAM */
        nuPiReadRom((u32)_songSegmentRomStart,
                    pcmBuffer,
                    (u32)(_songSegmentRomEnd - _songSegmentRomStart));
        
        /* Set playback frequency for PCM data */
        osAiSetFrequency(8000);
        
        /* Play PCM data */
        osAiSetNextBuffer(pcmBuffer, (u32)(_songSegmentRomEnd - _songSegmentRomStart));
      }
    }
  }
  
  /* Display PCM waveform playback status */
  ShowPCMStatus();
  
  /* Return to top menu with START button */
  if(contData.trigger & START_BUTTON)
    menuID = MENU_ID_TOP;
}

/* Display PCM waveform playback status */
void    ShowPCMStatus(void)
{
  static s8   message1[] = "Playing PCM waveform...";
  static s8   message2[] = "Idle";
  
  if(osAiGetLength())
  {
    /* Display that playback is in progress */
    nuDebConTextColor(NU_DEB_CON_WINDOW0, NU_DEB_CON_TEXT_LIGHTRED);
    nuDebConTextPos(NU_DEB_CON_WINDOW0, (NU_DEB_CON_ROW_MAX - strlen(message1)) >> 1, 20);
    nuDebConCPuts(NU_DEB_CON_WINDOW0, message1);
  }
  else
  {
    /* Display idle status */
    nuDebConTextColor(NU_DEB_CON_WINDOW0, NU_DEB_CON_TEXT_WHITE);
    nuDebConTextPos(NU_DEB_CON_WINDOW0, (NU_DEB_CON_ROW_MAX - strlen(message2)) >> 1, 20);
    nuDebConCPuts(NU_DEB_CON_WINDOW0, message2);
  }
}
