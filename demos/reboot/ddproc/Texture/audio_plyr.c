/*
 * audio_seq.c
 *
 *  Sequencer
 *
 */

/* pre-process */

#include "hglobals.h"
#include "haudio.h"
#include "haudio_osc.h"

#ifdef __NAUDIO__
#include <PR/n_libaudio.h>
#include <PR/n_libaudio_s_to_n.h>
#endif

/* variables */

ALSeq   sequence ,*_seqinfo = &sequence;

static ALSeqPlayer *_plyr = NULL;
static ALSeqFile  *_seq_file = NULL;             /* sequence file header */
static u8 * _seqbuf = NULL;      

ALSeqpConfig    _plyrcfg =
{
    AUDIO_SEQ_VVOICE_MAX ,	      /* maxVoices  */
	AUDIO_SEQ_EVTCOUNT_MAX,       /* maxEvents  */                                           
	AUDIO_SEQ_CHANNEL_MAX,        /* maxChannels 16 */ 
	0,
/* 	NO_VOICE_ERR_MASK | */
/* 	  NOTE_OFF_ERR_MASK | */
/* 		NO_SOUND_ERR_MASK,    /* debugFlags     */  */
	&audio_heap,          /* heap           */ 
	&initOsc,             /* initOsc        */ 
	&updateOsc,           /* updateOsc      */ 
	&stopOsc,             /* stopOsc        */ 
};


/* local functions */


/* implementation */ 

/*-----------------------------------------------------------------------------
  auplyr_create( void ) -- Sequence Player creation

  Only one Sequence Player can be created. 
-----------------------------------------------------------------------------*/

ALSeqPlayer *
auplyr_create( TAudioSegInfo *auinfo )
{
  ALSeqPlayer *seqp;
  ALBank      *_midibank=0;

  seqp = (ALSeqPlayer *)try_heapAlloc( _plyrcfg.heap, sizeof(ALSeqPlayer) ,16 ,"seqp");

  _midibank = auplyr_loadBank( auinfo->midibank.handle ,
							   auinfo->midibank.size ,
							   auinfo->miditable.handle );

  auosc_create(); /*  Put the oscillator into effect */

  auplyr_init(seqp ,&_plyrcfg ,_midibank);

  _seqbuf = (u8 *)try_heapAlloc(&audio_heap, AUDIO_SEQ_DATASIZE_MAX ,16 ,"_seqbuf"); /*  Sequence buffer  */

  _plyr = seqp; 

  return seqp;
}

void
auplyr_init(ALSeqPlayer *seqp, ALSeqpConfig *cfg , ALBank *bank )
{
    alSeqpNew(seqp, cfg);
    alSeqpSetBank(seqp, bank);
}

ALBank *
auplyr_loadBank(ALBankFile * bankfile ,s32 banklength , u8 * tablefile )
{
  ALBankFile * p;

  p = (ALBankFile *)try_heapAlloc(&audio_heap, banklength ,16 ,"_midibank");
  g_romRead( (void *)bankfile , (void *)p ,banklength);
  alBnkfNew( p , tablefile );

  return (ALBank *)( p->bankArray[0] );
}

/*-----------------------------------------------------------------------------
  auplyr_loadSeqFile(u8 * seqfile) -- sequence information code
-----------------------------------------------------------------------------*/

s32
auplyr_loadSeqFile(u8 * seqfile)
{
  ALSeqFile * p;
  s32 seqfile_size;

  /*  Obtain the file length */
  p = (ALSeqFile *)g_heapAlloc(&audio_heap, 4 ,16);  
  g_romRead( (void *)seqfile ,(void *)p , 4);
  seqfile_size = 4 + p->seqCount*sizeof(ALSeqData);

  p = (ALSeqFile *)try_heapAlloc(&audio_heap, seqfile_size ,16 ,"loadseqfile");
  g_romRead( (void *)seqfile ,(void *)p , seqfile_size);

  alSeqFileNew( p, seqfile );
  
  _seq_file = p;
  return _seq_file->seqCount;
}

/*-----------------------------------------------------------------------------
  auplyr_play(u8 * seqfile) -- start of sequence performance
-----------------------------------------------------------------------------*/

s32
auplyr_play(s32 seqNo)
{
  u8 *p;
  s32 len;

  if( !_seq_file || !_plyr ||
      ( _plyr->state != AL_STOPPED ) ||
      ( seqNo >= _seq_file->seqCount ) ) return 0;

  /*  Load the sequence. */
  p = _seq_file->seqArray[seqNo].offset;
  len = _seq_file->seqArray[seqNo].len;

  len = g_romRead((void *)p, (void *)_seqbuf, (u32)len);

  alSeqNew(_seqinfo, _seqbuf, len);
  alSeqpSetSeq(_plyr, _seqinfo);
  alSeqpPlay(_plyr);

  return -1;
}

s32
auplyr_stop(void)
{
  if( !_seq_file || !_plyr ||
      ( _plyr->state != AL_PLAYING ) ) return 0;
	 
  alSeqpStop(_plyr);
  return -1;
}

s32
auplyr_getState(void)
{
  if(!_plyr) return -1;
  return _plyr->state;
}

