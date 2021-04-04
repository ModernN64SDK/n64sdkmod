
/*************************************************************

  test_music.c : Nintendo 64 Music Tools Library Sample
  (c) Copyright 1998, Software Creations (Holdings) Ltd.

  Version 3.14

  CROSSFADE demo music related file. This demo illustrates how 
  to use the library with samples based in RAM as opposed to 
  ROM.

**************************************************************/

/* include system header files */
#ifndef F3DEX_GBI
#define F3DEX_GBI
#endif

#include <ultra64.h>
#include <libmus.h>

/* include current header file */
#include "test_rom.h"


#define FADE_VALUE 4	/* volume change value per frame */

/* internal workspace */
unsigned long  current_handle=0;
unsigned long  last_handle=0;
int            current_volume;
int            last_volume;

/* internal function prototypes */
static void DmaRomToRam(void *rom, void *ram, int len);


/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
  [GLOBAL FUNCTION]
  InitMusicDriver()

  [Explanation]
  Download ROM files and initialise the music player.

  [Return value]
  NONE
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/

/* ROM addresses */
#define WBANK_START     _wbankSegmentRomStart
#define PBANK_START 	   _pbankSegmentRomStart
#define PBANK_END	      _pbankSegmentRomEnd
#define MUSIC1_START	   _tune1SegmentRomStart
#define MUSIC1_END	   _tune1SegmentRomEnd
#define MUSIC2_START	   _tune2SegmentRomStart
#define MUSIC2_END	   _tune2SegmentRomEnd

extern char WBANK_START[];
extern char PBANK_START[];
extern char PBANK_END[];
extern char MUSIC1_START[];
extern char MUSIC1_END[];
extern char MUSIC2_START[];
extern char MUSIC2_END[];

/* permanent RAM buffers */
#define AUDIO_HEAP_SIZE 300000
#define PBANK_SIZE      50000
#define TUNE_SIZE       10000

unsigned char	audio_memory	[AUDIO_HEAP_SIZE]__attribute__((aligned(16)));
unsigned char	pointer_buf	   [PBANK_SIZE]__attribute__((aligned(16)));
/* make sure tune is 32bit aligned */
unsigned int	tune_buf1	   [TUNE_SIZE/sizeof(int)]__attribute__((aligned(16)));
unsigned int	tune_buf2	   [TUNE_SIZE/sizeof(int)]__attribute__((aligned(16)));

void InitMusicDriver(void)
{
   musConfig init;

   /* download .ptr file and song files to RAM */
   DmaRomToRam(PBANK_START, pointer_buf, PBANK_END-PBANK_START);
   DmaRomToRam(MUSIC1_START, (unsigned char *)tune_buf1, MUSIC1_END-MUSIC1_START);
   DmaRomToRam(MUSIC2_START, (unsigned char *)tune_buf2, MUSIC2_END-MUSIC2_START);

   /* setup configuration structure */
   init.control_flag    = 0;
   init.channels		   = 28; /* lots of channels! */
   init.sched           = &sc;
   init.thread_priority	= 12;
   init.heap		      = audio_memory;
   init.heap_length	   = AUDIO_HEAP_SIZE;
   init.fifo_length	   = 64;
   init.ptr		         = pointer_buf;
   init.wbk		         = WBANK_START;
   init.default_fxbank  = NULL;
   init.syn_output_rate	= 44100;
   init.syn_updates     = 256;
   init.syn_rsp_cmds	   = 4096;
   init.syn_retraceCount= 1;
   init.syn_num_dma_bufs= 36;
   init.syn_dma_buf_size= 0x800;

   MusInitialize(&init);
}

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
  [GLOBAL FUNCTION]
  FadeSongStart(song)

  [Explanation]
  Start a song, fading the old one down if one exists.

  [Parameters]
  song		address of song to start

  [Return value]
  NONE
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/

void FadeSongStart(void *song)
{
   if (current_handle)
   {
      if (last_handle)
      {
         MusHandleStop(last_handle, 0);
         while (MusHandleAsk(last_handle));
      }
      last_handle = current_handle;
      last_volume = current_volume;
   }
   current_handle = MusStartSong(song);
   current_volume = 0;
   MusHandleSetVolume(current_handle, current_volume);
}

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
  [GLOBAL FUNCTION]
  FadeSongProcess()

  [Explanation]
  Process the fading of songs volume levels.

  [Return value]
  NONE
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/

void FadeSongProcess(void)
{
   if (last_handle)
   {
      last_volume-=FADE_VALUE;
      if (last_volume<=0)
      {
         last_volume = 0;
         MusHandleStop(last_handle, 0);
      }
      else
      {
         MusHandleSetVolume(last_handle, last_volume);
      }
   }
   if (current_handle)
   {
      if (current_volume<0x80)
      {
         current_volume+=FADE_VALUE;
         if (current_volume>0x80)
            current_volume=0x80;
         MusHandleSetVolume(current_handle, current_volume);
      }
   }
}

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
  [INTERNAL FUNCTION]
  DmaRomToRam(rom, ram, len)

  [Parameters]
  rom		   address of ROM source
  ram		   address of RAM destination 
  len		   number of bytes to transfer

  [Explanation]
  Download an area of ROM to RAM. Note this function limits the size of any DMA
  generated to 16k so as to cause clashes with audio DMAs.

  [Return value]
  NONE
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/

/* avoid DMA clashes */
#define DMA_LEN 16384
                   
static void DmaRomToRam(void *rom, void *ram, int len)
{
   OSIoMesg io_mesg;
   OSMesg dummyMesg;
   int length;
   u32 src;
   unsigned char *dest;

   src = (u32)rom;
   dest = ram;
    
   while (len)
   {
      if (len>DMA_LEN)
         length = DMA_LEN;
      else
         length = len;
      osInvalDCache(dest, length);
      osPiStartDma(&io_mesg, OS_MESG_PRI_NORMAL, OS_READ, src, dest, length, &dma_queue);
      osRecvMesg(&dma_queue, &dummyMesg, OS_MESG_BLOCK);
      src+=length;
      dest+=length;
      len-=length;
   }
}

/* end of file */
