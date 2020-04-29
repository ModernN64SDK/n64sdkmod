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

/* Parameters changed with user input */
s32             songNo, seNo;
u32             fadeFrames;

/* Parameters changed during sound synthesis */
s32             songChannels, seChannels, totalChannels;

/* Memory related information */
s32             totalHeapSize, usedHeapSize, initHeapSize, dataHeapSize;

/* Video data pointers */
void *          pbank1Buffer;
void *          song1Buffer;
void *          song2Buffer;
void *          sebank1Buffer;

/* Audio related initialization functions */
void    InitializeAudio(void)
{
  musConfig init;
  
  /* Set initialization parameters */
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
  init.syn_retraceCount = 1;      /* Normally left as is */
  init.syn_num_dma_bufs = 64;
  init.syn_dma_buf_size = 0x300;
  
  /* Initialize Audio Manager */
  initHeapSize = nuAuStlMgrInit(&init);
  
  /* Reserve data areas from audio heap */
  pbank1Buffer  = nuAuStlHeapAlloc(PBANK1_END  - PBANK1_START);
  sebank1Buffer = nuAuStlHeapAlloc(SEBANK1_END - SEBANK1_START);
  song1Buffer   = nuAuStlHeapAlloc(SONG1_END   - SONG1_START);
  song2Buffer   = nuAuStlHeapAlloc(SONG2_END   - SONG2_START);
  
  /* Load and initialize default sample banks */
  DmaRomToRam(PBANK1_START, pbank1Buffer, PBANK1_END - PBANK1_START);
  MusPtrBankInitialize(pbank1Buffer, WBANK1_START);
  
  /* Load and initialize default sound effect banks */
  DmaRomToRam(SEBANK1_START, sebank1Buffer, SEBANK1_END - SEBANK1_START);
  MusFxBankInitialize(sebank1Buffer);
  
  /* Load songs */
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
  static musHandle      lastHandle = 0;
  
  /* Use Z trigger to stop all sound */
  if(contData.trigger & Z_TRIG)
    MusStop(MUSFLAG_SONGS | MUSFLAG_EFFECTS, fadeFrames);
  
  /* Use B button to stop the sound last started */
  if(contData.trigger & B_BUTTON && lastHandle != 0)
    MusHandleStop(lastHandle, fadeFrames);
  
  /* Processing according to current menu item */
  switch(index)
  {
  case MENU_SONG:
    if(contData.trigger & A_BUTTON) /* Start song */
      lastHandle = MusStartSong(songNo == 0 ? song1Buffer : song2Buffer);
    break;
  case MENU_SE:
    if(contData.trigger & A_BUTTON) /* Start SE playback */
      lastHandle = MusStartEffect(seNo);
    break;
  case MENU_FO:
    break;
  }
  
  /* Update real time information */
  songChannels = MusAsk(MUSFLAG_SONGS);
  seChannels = MusAsk(MUSFLAG_EFFECTS);
  totalChannels = songChannels + seChannels;
}

s32   GetNumberOfEffects(void)
{
  return MusFxBankNumberOfEffects(sebank1Buffer);
}
