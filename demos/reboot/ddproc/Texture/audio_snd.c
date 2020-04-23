/*
 * audio_snd.c
 *
 *  Sound Player
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

static ALSndPlayer 	*_sndp = NULL;
ALInstrument *_inst;
static ALSndId    	*_sndid;
   
ALSndpConfig    _sndpcfg =
{
  AUDIO_SND_VOICE_MAX,   	/* maxSoundd      */
  AUDIO_SND_EVTCOUNT_MAX,	/* maxEvents      */
  &audio_heap,				/* heap           */ 
};


/* local functions */

/* public
ALSndPlayer *	ausnd_create(TAudioSegInfo *info);
s32     	ausnd_play(s32 seqNo);
s32     	ausnd_stop(s32 seqNo);
s32     	ausnd_getState(s32 seqNo);
void    	ausnd_init(ALSndPlayer *sndp, ALSndpConfig *cfg , ALBank *bank );
 */

/* implementation */ 

/*-----------------------------------------------------------------------------
  auplyr_create( void ) -- Sound Player creation

  This class library is limited to one Sound Player. 
-----------------------------------------------------------------------------*/

#define ausnd_loadBank auplyr_loadBank

ALSndPlayer *
ausnd_create( TAudioSegInfo *auinfo ) 
{
  ALSndPlayer *sndp;
  ALBank      *sfxbank=0;

  sndp = (ALSndPlayer *)try_heapAlloc( _sndpcfg.heap, sizeof(ALSeqPlayer),16,"sndp");

  sfxbank = ausnd_loadBank( auinfo->sndbank.handle ,
							auinfo->sndbank.size ,
							auinfo->sndtable.handle );

  ausnd_init(sndp ,&_sndpcfg ,sfxbank);

  _sndp = sndp; 

  return sndp;
}

void
ausnd_init(ALSndPlayer *sndp, ALSndpConfig *cfg , ALBank *bank )
{
  s32 i;

  alSndpNew(sndp, cfg);
	
  _inst = bank->instArray[0]; /*  Only installment 0 is used. */
  _sndid = (ALSndId	*)try_heapAlloc(cfg->heap,_inst->soundCount*sizeof(ALSndId) ,16 ,"_sndid");

  FORI(_inst->soundCount)
	{
	  _sndid[i] = alSndpAllocate(sndp,_inst->soundArray[i]);
	  try_assert("audio_snd : fail to allocate ",( _sndid[i] != -1 ));
	}
}

/*-----------------------------------------------------------------------------
  ausnd_play(u8 * seqfile) -- sound output
-----------------------------------------------------------------------------*/
  f32 pitch=0.5;
  u8  fxmix=0;
  ALPan pan=64;
  s16 vol=30000;

s32
ausnd_play(s32 seqNo)
{

  if( !_sndp || seqNo >= _inst->soundCount
	 || seqNo < 0 ) return 0;

  alSndpSetSound(_sndp , _sndid[seqNo] );
  if( alSndpGetState(_sndp) != AL_STOPPED ) return 0;

  alSndpSetPitch(_sndp,pitch);
  alSndpSetPan(_sndp,pan);  
  alSndpSetVol(_sndp,vol);
  alSndpPlay(_sndp);

  return -1;
}

s32
ausnd_stop(s32 seqNo) /* If -1, the current target. */
{

  if( !_sndp || seqNo >= _inst->soundCount ) return 0;
  
  if(seqNo >= 0) alSndpSetSound(_sndp , _sndid[seqNo] );
	 
  alSndpStop(_sndp);
  return -1;
}

s32
ausnd_getState(s32 seqNo)
{
  if( !_sndp || seqNo >= _inst->soundCount ) return -1;

  if(seqNo >= 0) alSndpSetSound(_sndp , _sndid[seqNo] );
  return alSndpGetState(_sndp);
}

/*
   Discussion:

   ausnd_init assumes that only installment 0 of bank 0 in the bank file is used. 
   Using other numbers as well requires changes in the initialization mechanism. 
 
 */
