
/*************************************************************

  test_music.c : Nintendo 64 Music Tools Library Sample
  (c) Copyright 1998, Software Creations (Holdings) Ltd.

  Version 3.14

  FROMRAM demo music related file. This demo illustrates how to
  use the library with samples based in RAM as opposed to ROM.

**************************************************************/

/* include system header files */
#ifndef F3DEX_GBI
#define F3DEX_GBI
#endif

#include <ultra64.h>
#include <libmus.h>

/* include current header file */
#include "test_rom.h"

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

/* 'makerom' generated labels */

/* ROM addresses */
#define WBANK_START  _wbankSegmentRomStart
#define WBANK_END    _wbankSegmentRomEnd
#define PBANK_START 	_pbankSegmentRomStart
#define PBANK_END	   _pbankSegmentRomEnd
#define MUSIC_START	_tuneSegmentRomStart
#define MUSIC_END	   _tuneSegmentRomEnd

extern char WBANK_START[];
extern char WBANK_END[];
extern char PBANK_START[];
extern char PBANK_END[];
extern char MUSIC_START[];
extern char MUSIC_END[];

/* permanent RAM buffers */
#define AUDIO_HEAP_SIZE	300000
#define PBANK_SIZE      10000
#define TUNE_SIZE       10000

unsigned char	audio_memory	[AUDIO_HEAP_SIZE]__attribute__((aligned(16)));
unsigned char	pointer_buf	   [PBANK_SIZE]__attribute__((aligned(16)));
/* make sure tune is 32bit aligned */
unsigned int	tune_buf	      [TUNE_SIZE/sizeof(int)]__attribute__((aligned(16)));

extern unsigned char samples[];

void InitMusicDriver(void)
{
   musConfig init;

   /* indicate samples are to come from RAM (no audio DMAs) */
   init.control_flag = MUSCONTROL_RAM;

   /* download .wbk, .ptr and .bin files to RAM */
   DmaRomToRam(WBANK_START, samples, WBANK_END-WBANK_START);
   DmaRomToRam(PBANK_START, pointer_buf, PBANK_END-PBANK_START);
   DmaRomToRam(MUSIC_START, (char *)tune_buf, MUSIC_END-MUSIC_START);

   /* setup configuration structure */
   init.channels        = 16;
   init.sched		      = &sc;
   init.thread_priority	= 12;
   init.heap            = audio_memory;
   init.heap_length	   = AUDIO_HEAP_SIZE;
   init.fifo_length	   = 64;
   init.ptr		         = pointer_buf;
   init.wbk		         = &samples[0];
   init.default_fxbank  = NULL;
   init.syn_output_rate = 32000;
   init.syn_updates	   = 256;
   init.syn_rsp_cmds	   = 4096;
   init.syn_retraceCount = 1;
   init.syn_num_dma_bufs = 36;
   init.syn_dma_buf_size = 0x800;

   /* initialise music library */
   MusInitialize(&init);
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
