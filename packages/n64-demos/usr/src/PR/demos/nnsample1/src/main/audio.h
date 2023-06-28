/*======================================================================

		NINTENDO64 TECHNICAL SUPPORT CENTER 
		
		    NINTENDO64 SAMPLE PROGRAM 1

		Copyright (C) 1997, NINTENDO Co,Ltd.

======================================================================*/
#ifndef _AUDIO_H_
#define _AUDIO_H_

/*------------------------- debug flag ----------------------------*/

#ifdef _AUDIO_DEBUG_
/*
   _AUDIO_DEBUG_ flag must be enabled to use the following flags.  
*/
#undef _AUDIO_DEBUG_DMA_       /* Display number of DMA starts. */
#undef _AUDIO_DEBUG_PROC_      /* Displays the CPU occupancy rate. */

#endif /* _AUDIO_DEBUG */
/*------------------------- define --------------------------------*/
/*
   Please use this flag when using compact MIDI format data. 
   When using a Type0 MIDI sequence player, please do undef.
*/
#undef  _AUDIO_COMPACTMIDI_

/*
   These parameters need to be properly changed.  
   If not done appropriately, they do not work well.
*/
#define AUDIO_HEAP_SIZE          500000  /* Heap area size. */
#define AUDIO_CLIST_SIZE_MAX     5000    /* Command list buffer size. */
#define AUDIO_BUFFER_MAX         0x2000  /* Buffer size for synthesizer. */
#define AUDIO_OUTPUT_RATE        44100   /* Frequency setting. */
#define AUDIO_VVOICE_MAX         64      /* Maximum number of virtual voices. */
#define AUDIO_PVOICE_MAX         32      /* Max. number of physical voices. */
#define AUDIO_UPDATE_MAX         64      /* Max. number of updates of synthesizer parameters. */

/* Sets parameters of the sequence player. */
#define AUDIO_SEQ_VVOICE_MAX     64      /* Maxium number of virtual voices. */
#define AUDIO_SEQ_EVTCOUNT_MAX   64      /* Number of events. */
#define AUDIO_SEQ_CHANNEL_MAX    16      /* No. of MIDI channels (normally 16). */

/* Sets parameters of the sound player. */
#define AUDIO_SND_VOICE_MAX      32      /* Maximum number of sounds. */  
#define AUDIO_SND_EVTCOUNT_MAX   64      /* Maximum number of events. */

#define AUDIO_MESGS_MAX          8

/*
   Sequence data buffer.

   Sets the maximum value of among sequence data files.
   Statically allocates the data buffer.  If necessary, 
   change so allocated dynamically.
 */
#define AUDIO_SEQDATA_SIZE_MAX   0x10000

#define AUDIO_EXTRA_SAMPLES      80

/*
   The number of DMA buffers used by the sequence player.  Used like cache.
If the number exceeds the number of AUDIO_BUFFER_USE_FRAME, they are deleted. The actual required number and size of DMAbuffer varies depending on the software, so this needs to be checked.
*/
#define AUDIO_DMA_BUFFER_NUM    32        /* The number of DMA buffers. */
#define AUDIO_DMA_BUFFER_SIZE   2048      /* DMA buffer size */
#define AUDIO_DMA_QUEUE_SIZE    32        /* DMA message queue size */
#define AUDIO_BUFFER_USE_FRAME  2   /* Frame interval for clearing DMABuffer. */


#define AUDIO_STACKSIZE        0x2000         /* audio thread stack size */
#define AUDIO_THREAD_PRI       50
#define AUDIO_THREAD_ID        5

#define AUDIO_NUM_FIELDS       1             /* Retrace interval (1 or 2). */

/*------------------------- values --------------------------------*/
/* Sequence player structure. */
typedef struct {
  ALSeqpConfig seqconfig;
#ifdef _AUDIO_COMPACTMIDI_
  ALCSPlayer  seqplayer;     /* Sequence player structure. */
  ALCSeq       sequence;
#else
  ALSeqPlayer seqplayer;      /* Sequence player structure. */
  ALSeq       sequence;
#endif /*_AUDIO_COMPACTMIDI_ */

  u8  *seqdata_ptr;           /* Sequence data. */
  u32 seqno;                  /* Sequence No. */
  u32 state;                  /* Sequence player state. */
} Audio_seqplayer;

extern Audio_seqplayer seqplayer[2];

/*------------------------- functions --------------------------------*/
void auRomRead(u32 , void* , u32);
extern void auAudioInit(void);
extern void auReadMidiData(u32, u32);
extern void auReadSoundData(u32, u32);
extern void auReadSeqFileHeader(u32);
extern void auReadSeqFile(s32);
extern void auCleanDMABuffers(void);
extern void auCreateAudioThread(NNSched* );
extern void auSeqPlayerInit(u8* , u8*, u8*, u8*, u8*);
extern void auSeqPlayerSetFile(u32,s32);
extern int  auSeqPlayerPlay(u32);
extern s32  auSeqPlayerState(u32);
extern void auSeqPlayerStop(u32);
extern void auSndPlayerInit(u8* ,u8* ,u8*);
extern void auSndPlay(u32);
extern void auSndStop(u32);
extern s32  auSndGetState(u32);
#endif /* _AUDIO_H_ */


