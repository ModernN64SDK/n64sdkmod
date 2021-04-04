
/*************************************************************

  test_music.c : Nintendo 64 Music Tools Library Sample
  (c) Copyright 1998, Software Creations (Holdings) Ltd.

  Version 3.14

  NNSCHED demo main source file. This demo illustrates how to
  use a scheduler other than the default OS scheduler.

**************************************************************/

/* include system header files */
#ifndef F3DEX_GBI
#define F3DEX_GBI
#endif

#include <ultra64.h>
#include <libmus.h>
#include <libmus_data.h>
#include "nnsched.h"

/* include current header file */
#include "test_rom.h"


/* internal function prototypes */
static void DmaRomToRam(void *rom, void *ram, int len);

/* music library callback function prototypes these prototypes must match the 
   typedefs of the callback functions specified in "libmus.h":

      typedef void    (*LIBMUScb_install)     (void);
      typedef void    (*LIBMUScb_waitframe)   (void);
      typedef void    (*LIBMUScb_dotask)      (musTask *);
*/
static void NnSchedInstall(void);
static void NnSchedWaitFrame(void);
static void NnSchedDoTask(musTask *task);


/* music library scheduler callback structure */
static musSched nn_mus_sched = 
{ 
   NnSchedInstall, NnSchedWaitFrame, NnSchedDoTask 
};


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
#define MUSIC1_START	_tune1SegmentRomStart
#define MUSIC1_END	_tune1SegmentRomEnd

extern char WBANK_START[];
extern char PBANK_START[];
extern char PBANK_END[];
extern char MUSIC1_START[];
extern char MUSIC1_END[];

/* permanent RAM buffers */
#define AUDIO_HEAP_SIZE	300000
#define PBANK_SIZE	   5000
#define TUNE_SIZE	      8000

unsigned char	audio_memory	[AUDIO_HEAP_SIZE]__attribute__((aligned(16)));
unsigned char	pointer_buf	   [PBANK_SIZE]__attribute__((aligned(16)));
/* make sure tune is 32bit aligned */
unsigned int	tune_buf1	   [TUNE_SIZE/sizeof(int)]__attribute__((aligned(16)));


void InitMusicDriver(void)
{
   musConfig init;

   /* donwload sample .ptr and song .bin file to RAM */
   DmaRomToRam(PBANK_START, pointer_buf, PBANK_END-PBANK_START);
   DmaRomToRam(MUSIC1_START, (unsigned char *)tune_buf1, MUSIC1_END-MUSIC1_START);

   /* setup configuration structure */
   init.control_flag	   = 0;
   init.channels		   = 28; /* lots of channels! */
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

   /* set for NN scheduler this function must be called before 'musInitialize' 
      otherwise default scheduler will be initialised and used */
   MusSetScheduler(&nn_mus_sched);

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

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
  [MUSIC LIBRARY CALLBACK FUNCTION]
  NnSchedInstall()

  [Explanation]
  Initialise the NN scheduler callback functions for the music library. This function
  is called once by the audio thread before entering an infinite loop. 
  
  [Return value]
  NONE
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/

/* scheduler callback functions workspace */
#define QUEUE_SIZE   4

static NNScClient	   nnclient;
static OSMesgQueue   nnframe_queue;
static OSMesgQueue	nntask_queue;
static OSMesg        nnframe_messages  [QUEUE_SIZE];
static OSMesg		   nntask_messages   [QUEUE_SIZE];

static void NnSchedInstall(void)
{
   /* create message queues for WaitFrame and DoTask functions */
   osCreateMesgQueue(&nnframe_queue, &nnframe_messages[0], QUEUE_SIZE);
  	osCreateMesgQueue(&nntask_queue, &nntask_messages[0], QUEUE_SIZE);
   /* add waitframe client */
   nnScAddClient(&sc, &nnclient, &nnframe_queue);
}

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
  [MUSIC LIBRARY CALLBACK FUNCTION]
  NnSchedWaitFrame()

  [Explanation]
  Wait for a retrace message for the music library. This function is called by the
  audio threads inifinite loop to wait for a retrace message. The 'syn_retraceCount'
  parameter of the musConfig structure must contain the number of retraces per
  message received.
  
  [Return value]
  NONE
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/

static void NnSchedWaitFrame(void)
{
   NNScMsg *message;

	do
	{
      osRecvMesg(&nnframe_queue, (OSMesg *)&message, OS_MESG_BLOCK);
		osRecvMesg(&nnframe_queue, NULL, OS_MESG_NOBLOCK);	/* bin any missed syncs! */
	} while (*message!=NN_SC_RETRACE_MSG);
}

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
  [MUSIC LIBRARY CALLBACK FUNCTION]
  NnSchedDoTask()

  [Parameters]
  task      address of the music library task descriptor structure

  [Explanation]
  Process a audio task for the music library. This function is called by the audio
  threads inifinite loop to generate an audio task and wait for its completion.

  The structure passed is defined in "libmus.h" and is defined as:

         typedef struct
         {
            u64   *data;
            int   data_size;
            u64   *ucode;
            u64   *ucode_data;
         } musTask;

  The 'data' and 'data_size' components of the structure relate to the command list
  to be processed. 'data' is the address of the command list and 'data_size' is the
  number of commands it contains.

  The 'ucode' and 'ucode_data' components of the structure relate to the microcode
  to be used. 'ucode' is the address of the microcode Text section, 'ucode_data' is
  the address of the microcode Data section.
  
  [Return value]
  NONE
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/

static void NnSchedDoTask(musTask *task)
{
	NNScTask t;
   NNScMsg message;

   /* copy settings from musTask structure */
   t.list.t.data_ptr    = task->data;
   t.list.t.data_size   = task->data_size;
   t.list.t.ucode = (u64 *) task->ucode;
   t.list.t.ucode_data = (u64 *) task->ucode_data;

   /* fill in the rest of task data */
   t.next      = 0;
   t.msgQ      = &nntask_queue;
   t.msg       = &message;    
   t.list.t.type  = M_AUDTASK;
   t.list.t.ucode_boot = (u64 *)rspbootTextStart;
   t.list.t.ucode_boot_size = ((int) rspbootTextEnd - (int) rspbootTextStart);
   t.list.t.flags  = 0;
   t.list.t.ucode_size = 4096;
   t.list.t.ucode_data_size = SP_UCODE_DATA_SIZE;
   t.list.t.dram_stack = (u64 *) NULL;
   t.list.t.dram_stack_size = 0;
   t.list.t.output_buff = (u64 *) NULL;
   t.list.t.output_buff_size = 0;
   t.list.t.yield_data_ptr = NULL;
   t.list.t.yield_data_size = 0;

   /* start task */
   osSendMesg(nnScGetAudioMQ(&sc), (OSMesg) &t, OS_MESG_BLOCK);    
   /* wait for task to finish */
	osRecvMesg(&nntask_queue, NULL, OS_MESG_BLOCK);
}


/* end of file */
