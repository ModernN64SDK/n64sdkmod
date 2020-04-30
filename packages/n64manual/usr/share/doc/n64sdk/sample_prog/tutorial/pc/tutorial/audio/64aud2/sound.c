/*******************************************************************
 *
 *  sound.c
 *
 *******************************************************************/

#include  <nusys.h>

#ifdef     N_AUDIO
#include  <nualstl_n.h>
#else  /* !defined(N_AUDIO) */
#include  <nualstl.h>
#endif /*  N_AUDIO */
#include  <libmus.h>

#include  "sound.h"
#include  "menu.h"
#include  "rom.h"
#include  "segment.h"

/* Parameters changed by user input */
s32               effectType = AL_FX_BIGROOM,
                  markerNo;
s32               engine = 5000;
s32               effectX, effectZ;
s32               markerDone;

/* Parameters changed during sound synthesis */
s32               songChannels, seChannels, totalChannels;

/* Memory related information */
s32               totalHeapSize, usedHeapSize, initHeapSize, dataHeapSize;

static musHandle  song1Handle = 0,
                  song2Handle = 0,
                  se1Handle = 0,
                  se2Handle = 0,
                  se3Handle = 0;

/* Audio data pointers */
void *            pbank1Buffer;
void *            song1Buffer;
void *            song2Buffer;
void *            sebank1Buffer;

/* Synchronization marker callback functions */
static void   markerCallback(u32 handle, s32 number)
{
  if(handle == song2Handle)
    markerDone = number;
}

/* Audio related initialization functions */
void    InitializeAudio(void)
{
  musConfig init;
  
  /* Default parameter settings */
  init.control_flag     = 0;      /* If wbk is also placed in RAM, MUSCONTROL_RAM */
  init.channels         = 24;     /* Maximum total number of channels */
  init.sched            = NULL;   /* Leave as is */
  init.thread_priority  = NU_AU_MGR_THREAD_PRI; /* Normally, left as is */
  init.heap             = (u8 *)(NU_GFX_FRAMEBUFFER_ADDR - AUDIO_HEAP_SIZE);
  init.heap_length      = AUDIO_HEAP_SIZE;
  init.ptr              = NULL;   /* Initial ptr file */
  init.wbk              = NULL;   /* Initial wbk file */
  init.default_fxbank   = NULL;   /* Initial bfx file */
  init.fifo_length      = 64;     /* Normally, left as is */
  init.syn_updates      = 128;
  init.syn_output_rate  = 32000;  /* Normally, 32kHz or less */
  init.syn_rsp_cmds     = 0x800;
  init.syn_retraceCount = 1;      /* Normally, left as is */
  init.syn_num_dma_bufs = 64;
  init.syn_dma_buf_size = 0x300;
  
  /* Initialize Audio Manager */
  initHeapSize = nuAuStlMgrInit(&init);
  
  /* Reserve data areas from audio heap */
  pbank1Buffer  = nuAuStlHeapAlloc(PBANK1_END  - PBANK1_START);
  sebank1Buffer = nuAuStlHeapAlloc(SEBANK1_END - SEBANK1_START);
  song1Buffer   = nuAuStlHeapAlloc(SONG1_END   - SONG1_START);
  song2Buffer   = nuAuStlHeapAlloc(SONG2_END   - SONG2_START);
  
  /* Load and initialize default sample bank */
  DmaRomToRam(PBANK1_START, pbank1Buffer, PBANK1_END - PBANK1_START);
  MusPtrBankInitialize(pbank1Buffer, WBANK1_START);
  
  /* Load and initialize default sound effect bank */
  DmaRomToRam(SEBANK1_START, sebank1Buffer, SEBANK1_END - SEBANK1_START);
  MusFxBankInitialize(sebank1Buffer);
  
  /* Load songs	*/
  DmaRomToRam(SONG1_START, song1Buffer, SONG1_END - SONG1_START);
  DmaRomToRam(SONG2_START, song2Buffer, SONG2_END - SONG2_START);
  
  /* Default heap variable settings */
  totalHeapSize = AUDIO_HEAP_SIZE;
  usedHeapSize  = nuAuStlHeapGetUsed();
  dataHeapSize  = usedHeapSize - initHeapSize;
  
  /* Set BGM master volume */
  MusSetMasterVolume(MUSFLAG_SONGS, BGM_VOLUME);
}

extern NUContData   contData;

/* Functions responsible for processing tying menu processing to MUS, etc. */
void    SoundMenu(s32 index)
{
  static s32            se1Pan, se1Volume = 0;
  static u8             seUnset = 0,
                        markUnset1 = 0,
                        markUnset2 = 0;
  
  /* Use Z trigger to stop all sound */
  if(contData.trigger & Z_TRIG)
    MusStop(MUSFLAG_SONGS | MUSFLAG_EFFECTS, 6);
  
  /* Processing according to current menu item */
  switch(index)
  {
  case MENU_SONG1:
    /* Stop with B button (Better to place stop in front of start) */
    if(contData.trigger & B_BUTTON)
    {
        MusHandleStop(song1Handle, 6);/* Fadeout is constant at 6 frames */
        song1Handle = 0;
    }
    if(contData.trigger & L_JPAD)
      if(effectType > AL_FX_NONE)
      {
        effectType--;
        seUnset = 1;    /* FX type is unset */
      }
    if(contData.trigger & R_JPAD)
      if(effectType < AL_FX_ECHO)
      {
        effectType++;
        seUnset = 1;    /* FX type is unset */
      }
    if(seUnset)   /* If FX type is unset, set */
    {
      if(MusSetFxType(effectType))
        seUnset = 0;  /* If setting was ignored, leave seUnset == 1 as is */
    }
    if(contData.trigger & A_BUTTON) /* Start song */
    {
      MusHandleStop(song1Handle, 0);
      song1Handle = MusStartSong(song1Buffer);
    }
    break;
  
  case MENU_SONG2:
    /* Stop with B button (Better to place stop in front of start) */
    if(contData.trigger & B_BUTTON)
    {
      MusHandleStop(song2Handle, 6);
      markUnset1 = !MusSetSongFxChange(MUSBOOL_OFF);  /* Return programmer-specified effect */
      song2Handle = 0;
    }
    else if(markUnset1) /* Re-attempt if failed in previous frame */
      markUnset1 = !MusSetSongFxChange(MUSBOOL_OFF);
    else if(contData.trigger & A_BUTTON)
    {
      MusHandleStop(song2Handle, 0);    /* Stop if already playing */
      
      /* Register synchronization marker callback function (overwrite) */
      MusSetMarkerCallback(markerCallback);
      
      markUnset2 = !MusSetSongFxChange(MUSBOOL_ON); /* Use effect embedded in song */
      markerDone = markerNo;  /* When starting from synchronization marker, callback won't discard that marker */
      song2Handle = MusStartSongFromMarker(song2Buffer, markerNo);
    }
    else if(markUnset2) /* Re-attempt if failed in previous frame */
    {
      markUnset2 = !MusSetSongFxChange(MUSBOOL_ON);
    }
    break;
  
  case MENU_SE1:
    /* Reflect 3D stick status in variable */
    effectX = contData.stick_x * 1.15;
    effectZ = contData.stick_y * 1.15;
    
    se1Pan = effectX + 0x80;
    se1Volume = sqrtf(effectX * effectX + (0x80 - effectZ) * (0x80 - effectZ));
    
    /* Stop with B button (Better to place stop in front of start) */
    if(contData.trigger & B_BUTTON)
    {
      MusHandleStop(se1Handle, 6); /* Fadeout is constant at 6 frames */
      se1Handle = 0;
    }
    /* Use A button to restart */
    if(contData.trigger & A_BUTTON)
      se1Handle = MusStartEffect2(SE1_ID, se1Volume, se1Pan, 1, -1);
    break;
  
  case MENU_SE2:
    /* Stop with B button (Better to place stop in front of start) */
    if(contData.trigger & B_BUTTON)
    {
      MusHandleStop(se2Handle, 6); /* Fadeout is constant at 6 frames */
      se2Handle = 0;
    }
    /* Use A button to restart */
    if(contData.trigger & A_BUTTON)
      se2Handle = MusStartEffect(SE2_ID);
    break;
  
  case MENU_SE3:
    /* Reflect 3D stick status in variable */
    if(contData.stick_y > -60)
      engine = (contData.stick_y + 100) * 50;
    else
      engine = 0;
    
    /* Stop with B button (Better to place stop in front of start) */
    if(contData.trigger & B_BUTTON)
    {
      MusHandleStop(se3Handle, 6);
      se3Handle = 0;
    }
    /* Use A button to restart */
    if(contData.trigger & A_BUTTON)
      se3Handle = MusStartEffect(SE3_1_ID);
    break;
  }
  
  /* Update real time information */
  songChannels = MusAsk(MUSFLAG_SONGS);
  seChannels = MusAsk(MUSFLAG_EFFECTS);
  totalChannels = songChannels + seChannels;
  
  /* Perform periodic processing during playback of specific voice */
  if(se1Handle && index == MENU_SE1)  /* Change pan and volume of SE1 */
  {
    MusHandleSetPan(se1Handle, se1Pan);
    MusHandleSetVolume(se1Handle, se1Volume);
  }
  if(se3Handle && index == MENU_SE3)  /* Change sound effect and height of SE3 */
  {
    if(contData.stick_y > -60)
      MusHandleSetFreqOffset(se3Handle, contData.stick_y * 0.07F);
    else
    {
      MusHandleStop(se3Handle, 0);
      MusStartEffect(SE3_2_ID);
      se3Handle = 0;
    }
  }
}

s32   GetNumberOfEffects(void)
{
  return MusFxBankNumberOfEffects(sebank1Buffer);
}
