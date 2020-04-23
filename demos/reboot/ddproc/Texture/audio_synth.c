/*
 * audio_synth.c
 *
 *  Synthesizer --- Sequencer and Sound Player server
 *
 */

/* pre-process */

#include "hglobals.h"
#include "haudio.h"

#ifdef __NAUDIO__
#include <PR/n_libaudio.h>
#include <PR/n_libaudio_s_to_n.h>
#endif

/* variables */

ALGlobals    audio_global;

ALSynConfig  _syncfg =
{
  AUDIO_VVOICE_MAX, /*  maxVVoices  /* maximum number of virtual voices */ */
  AUDIO_PVOICE_MAX, /*  maxPVoices  /* maximum number of physical voices */ */
  AUDIO_UPDATE_MAX, /*  maxUpdates  /* maximum number of parameter update buffers */ */
  1,                /*  maxFXbusses /* auxiliary effects bus ( ignored in current version )*/ */
  (void *)(&audma_new),      /*  dmaproc     /* DMA callback function */ */
  &audio_heap,      /*  heap        /* audio heap */ */
  NULL,             /*  outputRate  /* fill in later */ */
  AL_FX_NONE, 	/*  fxType      /* effect type */ */
  0                 /*  params      /* custom effect */ */
};

/* local functions  */

/* public functions
s32     ausyn_create(void);
s32     ausyn_getFrameBufferSize(void);
void    ausyn_setCustomFxParam(ALSynConfig *sc);
 */

/* implmentations */

/*
   Synthesizer creation.  Returns the size of the frame buffer required. 
 */

s32
ausyn_create(void)
{
  /* synthethizer configuration */
  _syncfg.outputRate = osAiSetFrequency( AUDIO_OUTPUT_RATE );

/*  _setCustomFxParam(&_syncfg); */
  alInit(&audio_global, &_syncfg);

  return ausyn_getFrameBufferSize();
}

void
ausyn_setCustomFxParam(ALSynConfig *sc)
{
	/*
	 * if you wanted to build an effect, you would specify c.fxType
	 * to be AL_FX_CUSTOM, and then allocate and fill in the effect
	 * parameters. Some examples follows:
	 */
#define ms *(((s32) ((f32) AUDIO_OUTPUT_RATE/1000))&~0x7)
#define SECTION_COUNT 8
#define SECTION_SIZE  8

	s32	params[SECTION_COUNT*SECTION_SIZE+2] = {

	    /* sections	   total length */
	    SECTION_COUNT,    325 ms,
                                      /*       chorus  chorus   filter
       input  output  fbcoef  ffcoef   gain     rate   depth     coef      */
	        0,   8 ms,  9830,  -9830,      0,      0,      0,      0,
	     8 ms,  12 ms,  9830,  -9830, 0x2b84,      0,      0, 0x5000,
	    41 ms, 128 ms, 16384, -16384, 0x11eb,      0,      0,      0,
	    45 ms, 103 ms,  8192,  -8192,      0,      0,      0,      0,
	   162 ms, 282 ms, 16384, -16384, 0x11eb,      0,      0, 0x6000,
	   166 ms, 238 ms,  8192,  -8192,      0,      0,      0,      0,
	   238 ms, 268 ms,  8192,  -8192,      0,      0,      0,      0,
	        0, 299 ms, 18000,      0,      0,    380,   2000, 0x7000
	};

	sc->fxType = AL_FX_CUSTOM;
	sc->params = params;
}

s32
ausyn_getFrameBufferSize(void)
{
  return CEIL( (f32) AUDIO_NUM_FIELDS * _syncfg.outputRate / (f32) 60 );
}

