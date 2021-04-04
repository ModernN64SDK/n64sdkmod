
/*************************************************************

  test_main.c : Nintendo 64 Music Tools Library Sample
  (c) Copyright 1998, Software Creations (Holdings) Ltd.

  Version 3.14

  SIMPLE demo music related file.

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

/* ROM addresses */
#define WBANK_START	_wbankSegmentRomStart
#define PBANK_START 	_pbankSegmentRomStart
#define PBANK_END	   _pbankSegmentRomEnd
#define MUSIC_START	_tuneSegmentRomStart
#define MUSIC_END	   _tuneSegmentRomEnd

extern char WBANK_START[];
extern char PBANK_START[];
extern char PBANK_END[];
extern char MUSIC_START[];
extern char MUSIC_END[];

/* permanent RAM buffers */
#define AUDIO_HEAP_SIZE    300000
#define PBANK_SIZE         50000
#define TUNE_SIZE          10000

unsigned char	audio_memory	[AUDIO_HEAP_SIZE] __attribute__((aligned (16)));
unsigned char	pointer_buf	   [PBANK_SIZE] __attribute__((aligned (16)));
/* make sure tune is 32bit aligned */
unsigned int	tune_buf	      [TUNE_SIZE/sizeof(int)] __attribute__((aligned (16)));

void InitMusicDriver(void)
{
   musConfig init;

   /* download sample .ptr and song .bin file to RAM */
   DmaRomToRam(PBANK_START, pointer_buf, PBANK_END-PBANK_START);
   DmaRomToRam(MUSIC_START, tune_buf, MUSIC_END-MUSIC_START);

   /* setup configuration structure */
   init.control_flag	   = 0;
   init.channels		   = 16;
   init.sched		      = &sc;
   init.thread_priority	= 12;
   init.heap		      = audio_memory;
   init.heap_length	   = AUDIO_HEAP_SIZE;
   init.fifo_length	   = 64;
   init.ptr		         = pointer_buf;
   init.wbk		         = WBANK_START;
   init.default_fxbank  = NULL;
   init.syn_output_rate	= 44100;
   init.syn_updates	   = 256;
   init.syn_rsp_cmds	   = 4096;
   init.syn_retraceCount= 1;
   init.syn_num_dma_bufs= 36;
   init.syn_dma_buf_size= 0x800;

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

/* avoid large DMA clashes */
#define DMA_LEN 16384

static void DmaRomToRam(void *rom, void *ram, int len)
{
   char *src, *dest;
   OSIoMesg dmaIoMesgBuf;
   OSMesg dummyMesg;
   int length;

   src = rom;
   dest = ram;
    
   while (len)
   {
      if (len>DMA_LEN)
         length = DMA_LEN;
      else
         length = len;

      osInvalDCache(dest, length);
      osPiStartDma(&dmaIoMesgBuf, OS_MESG_PRI_NORMAL, OS_READ, (u32)src, dest, length, &dma_queue);
      osRecvMesg(&dma_queue, &dummyMesg, OS_MESG_BLOCK);
      src+=length;
      dest+=length;
      len-=length;
   }
}


/* end of file */
