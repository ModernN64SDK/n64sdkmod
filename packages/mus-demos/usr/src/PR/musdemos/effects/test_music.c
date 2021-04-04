
/*************************************************************

  test_music.c : Nintendo 64 Music Tools Library Sample
  (c) Copyright 1998, Software Creations (Holdings) Ltd.

  Version 3.14

  EFFECTS demo main source file. This demo illustrates how to
  use sound effects from different sound effect banks.

**************************************************************/

/* include system header files */
#ifndef F3DEX_GBI
#define F3DEX_GBI
#endif

#include <ultra64.h>
#include <libmus.h>
#include <libmus_data.h>

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
#define WBANK1_START    _wbank1SegmentRomStart
#define PBANK1_START    _pbank1SegmentRomStart
#define PBANK1_END      _pbank1SegmentRomEnd
#define WBANK2_START    _wbank2SegmentRomStart
#define PBANK2_START    _pbank2SegmentRomStart
#define PBANK2_END      _pbank2SegmentRomEnd
#define WBANK3_START    _wbank3SegmentRomStart
#define PBANK3_START    _pbank3SegmentRomStart
#define PBANK3_END      _pbank3SegmentRomEnd
#define MUSIC_START     _tuneSegmentRomStart
#define MUSIC_END       _tuneSegmentRomEnd
#define EFFECT1_START   _effect0SegmentRomStart
#define EFFECT1_END     _effect0SegmentRomEnd  
#define EFFECT2_START   _effect1SegmentRomStart
#define EFFECT2_END     _effect1SegmentRomEnd  
#define EFFECT3_START   _effect2SegmentRomStart
#define EFFECT3_END     _effect2SegmentRomEnd  

extern char WBANK1_START[];
extern char PBANK1_START[];
extern char PBANK1_END[];
extern char WBANK2_START[];
extern char PBANK2_START[];
extern char PBANK2_END[];
extern char WBANK3_START[];
extern char PBANK3_START[];
extern char PBANK3_END[];
extern char MUSIC_START[];
extern char MUSIC_END[];
extern char EFFECT1_START[];
extern char EFFECT1_END[];
extern char EFFECT2_START[];
extern char EFFECT2_END[];
extern char EFFECT3_START[];
extern char EFFECT3_END[];

/* permanent RAM buffers */
#define AUDIO_HEAP_SIZE	300000
#define PBANK_SIZE      10000
#define TUNE_SIZE       8000
#define EFFECT_SIZE     1024

unsigned char	audio_memory	[AUDIO_HEAP_SIZE] __attribute__((aligned (16)));
unsigned char   pointer_buf1    [PBANK_SIZE] __attribute__((aligned (16)));
unsigned char   pointer_buf2    [PBANK_SIZE] __attribute__((aligned (16)));
unsigned char   pointer_buf3    [PBANK_SIZE] __attribute__((aligned (16)));
/* make sure of 32bit alignment */
unsigned int    tune_buf1       [TUNE_SIZE/sizeof(int)] __attribute__((aligned (16)));
unsigned int    effect_buf1     [EFFECT_SIZE/sizeof(int)] __attribute__((aligned (16)));
unsigned int    effect_buf2     [EFFECT_SIZE/sizeof(int)] __attribute__((aligned (16)));
unsigned int    effect_buf3     [EFFECT_SIZE/sizeof(int)] __attribute__((aligned (16)));

void InitMusicDriver(void)
{
   musConfig init;

   /* download sample .ptr, song .bin and effect .bfx files to RAM */
   DmaRomToRam(PBANK1_START, pointer_buf1, PBANK1_END-PBANK1_START);
   DmaRomToRam(PBANK2_START, pointer_buf2, PBANK2_END-PBANK2_START);
   DmaRomToRam(PBANK3_START, pointer_buf3, PBANK3_END-PBANK3_START);
   DmaRomToRam(MUSIC_START, tune_buf1, MUSIC_END-MUSIC_START);
   DmaRomToRam(EFFECT1_START, effect_buf1, EFFECT1_END-EFFECT1_START); 
   DmaRomToRam(EFFECT2_START, effect_buf2, EFFECT2_END-EFFECT2_START); 
   DmaRomToRam(EFFECT3_START, effect_buf3, EFFECT3_END-EFFECT3_START); 

   /* setup configuration structure */
   init.control_flag    = 0;
   init.channels        = 28; /* lots of channels! */
   init.sched		      = &sc;
   init.thread_priority	= 12;
   init.heap		      = audio_memory;
   init.heap_length	   = AUDIO_HEAP_SIZE;
   init.fifo_length	   = 64;
   init.ptr                      = pointer_buf1;
   init.wbk                      = WBANK1_START;
   init.default_fxbank  = effect_buf1;
   init.syn_output_rate	= 44100;
   init.syn_updates	   = 256;
   init.syn_rsp_cmds	   = 4096;
   init.syn_retraceCount= 1;
   init.syn_num_dma_bufs= 36;
   init.syn_dma_buf_size= 0x800;

   MusInitialize(&init);

   /* initialise 2nd and 3rd sample banks */
   MusPtrBankInitialize(pointer_buf2, WBANK2_START);
   MusPtrBankInitialize(pointer_buf3, WBANK3_START);
   /* initialise 2nd and 3rd effect banks */
   MusFxBankInitialize(effect_buf2);
   MusFxBankInitialize(effect_buf3);
   /* set default sample bank for 2nd and 3rd effect banks */
   MusFxBankSetPtrBank(effect_buf2, pointer_buf2);
   MusFxBankSetPtrBank(effect_buf3, pointer_buf3);
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
