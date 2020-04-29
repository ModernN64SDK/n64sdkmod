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

/* Parameters changed during sound synthesis */
s32               songChannels, seChannels, totalChannels;

/* Memory related information */
s32               totalHeapSize, usedHeapSize, initHeapSize, dataHeapSize;

/* Variable indicating Options mode in Stage 0 */
u8                optionsMode = 0;

/* Audio data pointers */
void *            pbank1Buffer;     /* Sample banks A, B */
void *            pbank2Buffer;     /* Sample bank C */
void *            pbank3Buffer;     /* Sample banks D, E */
void *            song1Buffer;      /* Songs A, B */
void *            song2Buffer;      /* Song C */
void *            sebank1Buffer;    /* SE banks D, E */

/* Audio handles */
static musHandle  bgmHandle;

/* Function to change volume scale of song */
static void       ChangeSongVolume(void);

/* Audio related initialization function */
void    InitializeAudio(void)
{
  musConfig   init;
  s32         bankSize;
  
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
  init.syn_output_rate  = 44100;  /* Normally, 32kHz or less */
  init.syn_rsp_cmds     = 0x800;
  init.syn_retraceCount = 1;      /* Normally, left as is */
  init.syn_num_dma_bufs = 64;
  init.syn_dma_buf_size = 0x400;
  
  /* Initialize Audio Manager */
  initHeapSize = nuAuStlMgrInit(&init);
  
  /* Reserve data areas from audio heap */
  bankSize = (PBANK_A_END - PBANK_A_START > PBANK_B_END - PBANK_B_START ?
              PBANK_A_END - PBANK_A_START :
              PBANK_B_END - PBANK_B_START);
  pbank1Buffer = nuAuStlHeapAlloc(bankSize);
  pbank2Buffer = nuAuStlHeapAlloc(PBANK_C_END - PBANK_C_START);
  bankSize = (PBANK_D_END - PBANK_D_START > PBANK_E_END - PBANK_E_START ?
              PBANK_D_END - PBANK_D_START :
              PBANK_E_END - PBANK_E_START);
  pbank3Buffer = nuAuStlHeapAlloc(bankSize);
  bankSize = (SONG_A_END - SONG_A_START > SONG_B_END - SONG_B_START ?
              SONG_A_END - SONG_A_START :
              SONG_B_END - SONG_B_START);
  song1Buffer  = nuAuStlHeapAlloc(bankSize);
  song2Buffer  = nuAuStlHeapAlloc(SONG_C_END   - SONG_C_START);
  bankSize = (SEBANK_D_END - SEBANK_D_START > SEBANK_E_END - SEBANK_E_START ?
              SEBANK_D_END - SEBANK_D_START :
              SEBANK_E_END - SEBANK_E_START);
  sebank1Buffer = nuAuStlHeapAlloc(bankSize);
  
  /* Default heap variable settings */
  totalHeapSize = AUDIO_HEAP_SIZE;
  usedHeapSize  = nuAuStlHeapGetUsed();
  dataHeapSize  = usedHeapSize - initHeapSize;
  
  /* Set BGM master volume */
  MusSetMasterVolume(MUSFLAG_SONGS, BGM_VOLUME);
}

/* Audio initialization function for Stage 0 */
void  InitializeAudio0(void)
{
  /* Load and initialize sample banks */
  DmaRomToRam(PBANK_A_START, pbank1Buffer, PBANK_A_END - PBANK_A_START);
  MusPtrBankInitialize(pbank1Buffer, WBANK_A_START);
  DmaRomToRam(PBANK_C_START, pbank2Buffer, PBANK_C_END - PBANK_C_START);
  MusPtrBankInitialize(pbank2Buffer, WBANK_C_START);
  DmaRomToRam(PBANK_D_START, pbank3Buffer, PBANK_D_END - PBANK_D_START);
  MusPtrBankInitialize(pbank3Buffer, WBANK_D_START);
  
  /* Load and initialize sound effect banks */
  DmaRomToRam(SEBANK_D_START, sebank1Buffer, SEBANK_D_END - SEBANK_D_START);
  MusFxBankInitialize(sebank1Buffer);
  
  /* Default bank settings */
  MusPtrBankSetCurrent(pbank1Buffer);
  MusFxBankSetCurrent(sebank1Buffer);
  
  /* Load songs */
  DmaRomToRam(SONG_A_START, song1Buffer, SONG_A_END - SONG_A_START);
  DmaRomToRam(SONG_C_START, song2Buffer, SONG_C_END - SONG_C_START);
  
  /* Start BGM */
  bgmHandle = MusStartSong(song1Buffer);
}

/* Audio initialization function for Stage 1 */
void  InitializeAudio1(void)
{
  /* Load and initialize sample banks */
  DmaRomToRam(PBANK_B_START, pbank1Buffer, PBANK_B_END - PBANK_B_START);
  MusPtrBankInitialize(pbank1Buffer, WBANK_B_START);
  DmaRomToRam(PBANK_C_START, pbank2Buffer, PBANK_C_END - PBANK_C_START);
  MusPtrBankInitialize(pbank2Buffer, WBANK_C_START);
  DmaRomToRam(PBANK_E_START, pbank3Buffer, PBANK_E_END - PBANK_E_START);
  MusPtrBankInitialize(pbank3Buffer, WBANK_E_START);
  
  /* Load and initialize sound effect banks */
  DmaRomToRam(SEBANK_E_START, sebank1Buffer, SEBANK_E_END - SEBANK_E_START);
  MusFxBankInitialize(sebank1Buffer);
  
  /* Default bank settings */
  MusPtrBankSetCurrent(pbank1Buffer);
  MusFxBankSetCurrent(sebank1Buffer);
  
  /* Load songs */
  DmaRomToRam(SONG_B_START, song1Buffer, SONG_B_END - SONG_B_START);
  DmaRomToRam(SONG_C_START, song2Buffer, SONG_C_END - SONG_C_START);
  
  /* Start BGM */
  bgmHandle = MusStartSong(song1Buffer);
}

extern NUContData   contData;

/* Functions responsible for processing tying menu processing to MUS, etc. (Stage 0) */
void    SoundMenu0(s32 index)
{
  /* Processing according to current menu item */
  switch(index)
  {
  case MENU0_START:
    if(optionsMode)
    {
      /* Stop playback of song for OPTIONS screen */
      MusHandleStop(bgmHandle, 0);
      optionsMode = 0;
      /* Start playback of song for START screen */
      bgmHandle = MusStartSong(song1Buffer);
    }
    if(contData.trigger & A_BUTTON)   /* Stop song playback (then switch stages) */
      /* Switch stages */
      MusHandleStop(bgmHandle, 120);
    break;
  
  case MENU0_OPTIONS:
    if(contData.trigger & A_BUTTON && !optionsMode) /* Start playback of song for OPTIONS screen */
    {
      /* Switch stages */
      MusHandleStop(bgmHandle, 0);
      /* Start playback of song for OPTIONS screen */
      MusPtrBankSetSingle(pbank2Buffer);
      bgmHandle = MusStartSong(song2Buffer);
      optionsMode = 1;
    }
    /* Controller processing in OPTIONS mode */
    if(optionsMode)
    {
      if(contData.trigger & L_TRIG)
      {
        MusPtrBankSetSingle(pbank3Buffer);
        MusStartEffect(0);
      }
      else if(contData.trigger & R_TRIG)
      {
        MusPtrBankSetSingle(pbank3Buffer);
        MusStartEffect(1);
      }
    }
    break;
  }
  
  /* Update real time information */
  songChannels = MusAsk(MUSFLAG_SONGS);
  seChannels = MusAsk(MUSFLAG_EFFECTS);
  totalChannels = songChannels + seChannels;
  
  /* If all audio playback is done, switch stages */
  if(!MusAsk(MUSFLAG_SONGS | MUSFLAG_EFFECTS))
    stage = 1;
}

/* Functions responsible for processing tying menu processing to MUS, etc. (Stage 1) */
void    SoundMenu1(s32 index)
{
  static u8     gameover = 0;
  
  /* Processing according to current menu item */
  if(!gameover)
  {
    switch(index)
    {
    case MENU1_RESTART:
      if(contData.trigger & A_BUTTON)
        /* Switch stages */
        MusHandleStop(bgmHandle, 120);
      break;
    
    case MENU1_EFFECTS:
      /* Play special effects */
      if(contData.trigger & A_BUTTON)
      {
        MusPtrBankSetSingle(pbank3Buffer);
        MusStartEffect(effectNo);
      }
      break;
    }
  }
  
  /* Update real time information */
  songChannels = MusAsk(MUSFLAG_SONGS);
  seChannels = MusAsk(MUSFLAG_EFFECTS);
  totalChannels = songChannels + seChannels;
  
  /* If all audio playback is done, switch stages */
  if(!MusAsk(MUSFLAG_SONGS | MUSFLAG_EFFECTS))
  {
    if(!gameover)
    {
      /* Play game over sound effect a*/
      MusPtrBankSetSingle(pbank3Buffer);
      MusStartEffect(7);
      gameover = 1;
    }
    else
    {
      stage = 0;
      gameover = 0;
    }
  }
}

s32   GetNumberOfEffects(void * SEBankPtr)
{
  return MusFxBankNumberOfEffects(SEBankPtr);
}
