/*============================================================================

		NINTENDO64 TECHNICAL SUPPORT CENTER 
		
		    NINTENDO64 SAMPLE PROGRAM 1

		Copyright (C) 1997, NINTENDO Co,Ltd.

============================================================================*/
#ifndef _AUDIO_H_
#define _AUDIO_H_

/*------------------------- debug flag ----------------------------*/

/*
  Flag definition for debugging
Make here define if necessary, and undef if not necessary  
*/
#undef _AUDIO_DEBUG_           /* debug flag */

#ifdef _AUDIO_DEBUG_
/*
    It is necessary to validate _AUDIO_DEBUG_  flag when the following flag is used.
    
*/
#define _AUDIO_DEBUG_DMA_       /* Display DMA activating number */
#define _AUDIO_DEBUG_PROC_      /* Display CPU occupancy rate*/

#endif /* _AUDIO_DEBUG */
/*------------------------- define --------------------------------*/
/*
   When data of compact MIDI form is used,  use this flag.
   When Type0 MIDI sequence player is used, do unddef
*/
#undef  _AUDIO_COMPACTMIDI_

/*
   Parameters here need to be changed properly.
  If changed improperly,  it won't operate right.
*/
#define AUDIO_HEAP_SIZE          700000  /* Size of heap area */
#define AUDIO_CLIST_SIZE_MAX     5000    /* Buffer size of command list */
#define AUDIO_BUFFER_MAX         0x2000  /*Buffer size for synthesizer */
#define AUDIO_OUTPUT_RATE        44100   /* Set frequency */
#define AUDIO_VVOICE_MAX         64      /* The number of maximum virtual voices */
#define AUDIO_PVOICE_MAX         32      /* The number of maximum physical voices */
#define AUDIO_UPDATE_MAX         128      /* Maximum number of buffer to be used in synthesize driver*/

/*Set parameter of sequence player */
#define AUDIO_SEQ_VVOICE_MAX     64      /* The number of maximum virtual voices*/
#define AUDIO_SEQ_EVTCOUNT_MAX   64      /* The number of events */
#define AUDIO_SEQ_CHANNEL_MAX    16      /* The number of MIDI channels (normally 16) */

/*Set parameter of sound player */
#define AUDIO_SND_VOICE_MAX      32      /*The number of maximum sound */  
#define AUDIO_SND_EVTCOUNT_MAX   32      /* The number of maximum event */

#define AUDIO_MESGS_MAX          8

/*
   Sequence data buffer

   Set the maximum value of sequence data file.
  Allocate data buffer fixed.
  Change it to allocate it dynamically if necessary.
 */
#define AUDIO_SEQDATA_SIZE_MAX   0x10000
#ifdef NAUDIO
#define AUDIO_SAMPLES		184
#define AUDIO_EXTRA_SAMPLES	0
extern u8 n_aspMainTextStart[], n_aspMainTextEnd[];
extern u8 n_aspMainDataStart[], n_aspMainDataEnd[];
#else
#define AUDIO_EXTRA_SAMPLES      80
#endif /* NAUDIO */

/*
   The number of buffer for DMA to be used for sequence player  
   As used like a cache and if it passed AUDIO_BUFFER_USE_FRAME number, it will be deleted. 
   It is necessary to check how many DMAbuffers and what size are required as they vary from software to  
   software.
*/
#define AUDIO_DMA_BUFFER_NUM    64        /* The number of DMA buffer  */
#define AUDIO_DMA_BUFFER_SIZE   512      /* DMA buffer size */
#define AUDIO_DMA_QUEUE_SIZE    128        /* DMA message queue size */
#define AUDIO_BUFFER_USE_FRAME  1         /* Frame intervals to clear DMABuffer  */


#define AUDIO_STACKSIZE        0x2000         /* audio thread stack size */
#define AUDIO_THREAD_PRI       50
#define AUDIO_THREAD_ID        5

#define AUDIO_NUM_FIELDS       1             /* Retrace intervals*/

/*------------------------- values --------------------------------*/
/* Sequence player structure */
typedef struct {
  ALSeqpConfig seqconfig;
#ifdef _AUDIO_COMPACTMIDI_
  ALCSPlayer  seqplayer;     /* Sequence player structure  */
  ALCSeq       sequence;
#else
  ALSeqPlayer seqplayer;      /* Sequence player structure*/
  ALSeq       sequence;
#endif /*_AUDIO_COMPACTMIDI_ */

  u8  *seqdata_ptr;           /* Sequence data */
  u32 seqno;                  /*Sequence No. */
  u32 state;                  /* State of sequence player */
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
extern u32  snd_alloc_num;
#endif /* _AUDIO_H_ */
