/*====================================================================
 *
 * Copyright 1993, Silicon Graphics, Inc.
 * All Rights Reserved.
 *
 * This is UNPUBLISHED PROPRIETARY SOURCE CODE of Silicon Graphics,
 * Inc.; the contents of this file may not be disclosed to third
 * parties, copied or duplicated in any form, in whole or in part,
 * without the prior written permission of Silicon Graphics, Inc.
 *
 * RESTRICTED RIGHTS LEGEND:
 * Use, duplication or disclosure by the Government is subject to
 * restrictions as set forth in subdivision (c)(1)(ii) of the Rights
 * in Technical Data and Computer Software clause at DFARS
 * 252.227-7013, and/or in similar or successor clauses in the FAR,
 * DOD or NASA FAR Supplement. Unpublished - rights reserved under the
 * Copyright Laws of the United States.
 *====================================================================*/

#include <os_internal.h>
#include <ultraerror.h>
#include <assert.h>
#include "n_sndp.h"

static  ALMicroTime     _n_sndpVoiceHandler(void *node);
static  void            _n_handleEvent(N_ALSndpEvent *event);
static	void		_removeEvents(ALEventQueue *evtq, N_ALSoundState *state);
static	s32		_DivS32ByF32 (s32 i, f32 f);

void n_alSndpNew(N_ALSndPlayer *sndp, ALSndpConfig *c) 
{    
  u8             *ptr;
  N_ALEvent         evt;
  N_ALSoundState *sState;
  u32             i;
  
  /*
   * Init member variables
   */
  sndp->maxSounds = c->maxSounds;
  sndp->target = -1;
  sndp->frameTime = AL_USEC_PER_FRAME;        /* time between API events */
  sState = (N_ALSoundState*)alHeapAlloc(c->heap, 1,
                                        c->maxSounds * sizeof(N_ALSoundState));
  sndp->sndState = sState;
  
  for(i = 0; i < c->maxSounds; i++)
    sState[i].sound = 0;
  
  /*
   * init the event queue
   */
  ptr = alHeapAlloc(c->heap, 1, c->maxEvents * sizeof(N_ALEventListItem));
  n_alEvtqNew(&sndp->evtq, (N_ALEventListItem *)ptr, c->maxEvents);
  
  /*
   * add ourselves to the driver
   */
  sndp->node.next = NULL;
  sndp->node.handler = _n_sndpVoiceHandler;
  sndp->node.clientData = sndp;

  n_alSynAddSndPlayer( &sndp->node);
  
  /*
   * Start responding to API events
   */
  evt.type = AL_SNDP_API_EVT;
  n_alEvtqPostEvent(&sndp->evtq, (N_ALEvent *)&evt, sndp->frameTime);
  sndp->nextDelta = n_alEvtqNextEvent(&sndp->evtq, &sndp->nextEvent);
  
}

/*************************************************************
 * Sound Player private routines
 *************************************************************/
static ALMicroTime 
  _n_sndpVoiceHandler(void *node)
{
  N_ALSndPlayer *sndp = (N_ALSndPlayer *) node;
  N_ALSndpEvent evt;
  
  do {
    switch (sndp->nextEvent.type) {
    case (AL_SNDP_API_EVT):
      evt.common.type = AL_SNDP_API_EVT;
      evt.common.state = (N_ALSoundState*)-1;
      n_alEvtqPostEvent(&sndp->evtq, (N_ALEvent *)&evt, sndp->frameTime);
      break;
      
    default:
      _n_handleEvent((N_ALSndpEvent *)&sndp->nextEvent);
      break;
    }
    sndp->nextDelta = n_alEvtqNextEvent(&sndp->evtq, &sndp->nextEvent);
    
  } while (sndp->nextDelta == 0);
  sndp->curTime += sndp->nextDelta;
  return sndp->nextDelta;
}

static void 
  _n_handleEvent(N_ALSndpEvent *event) 
{
  N_ALSndPlayer *sndp = (N_ALSndPlayer *)n_syn->n_sndp;
  ALVoiceConfig       vc;
  ALSound             *snd;
  N_ALVoice             *voice;
  ALPan               pan;
  f32                 pitch;
  N_ALSndpEvent         evt;
  ALMicroTime         delta;
  
  s16                 vol;
  s16                 tmp;
  s32                 vtmp;
  N_ALSoundState        *state;
  
  state = event->common.state;
  snd   = state->sound;
  
  
  switch (event->msg.type) {
  case (AL_SNDP_PLAY_EVT):
    if (state->state != AL_STOPPED || !snd)
      return;
    
    vc.fxBus      = 0;            /* effect buss 0 */
    vc.priority   = state->priority;
    vc.unityPitch = 0;
    
    voice = &state->voice;
    n_alSynAllocVoice(voice, &vc);

    vol   = (s16) ((s32) snd->envelope->attackVolume*state->vol/AL_VOL_FULL);
    tmp   = state->pan - AL_PAN_CENTER + snd->samplePan;
    tmp   = MAX(tmp, AL_PAN_LEFT);
    pan   = (ALPan) MIN(tmp, AL_PAN_RIGHT);
    pitch = state->pitch;
#if 0
    delta   = snd->envelope->attackTime;
#else
    delta = (ALMicroTime) _DivS32ByF32 (snd->envelope->attackTime, state->pitch);
#endif
    
    n_alSynStartVoice(voice, snd->wavetable);
    state->state = AL_PLAYING;
    
    n_alSynSetPan(voice, pan);
    n_alSynSetVol(voice, vol, delta);
    n_alSynSetPitch(voice, pitch);
    n_alSynSetFXMix(voice, state->fxMix);
    
    evt.common.type     = AL_SNDP_DECAY_EVT;
    evt.common.state    = state;
    delta = (ALMicroTime) _DivS32ByF32 (snd->envelope->attackTime, state->pitch);
    n_alEvtqPostEvent(&sndp->evtq, (N_ALEvent *)&evt, delta);
    break;
    
  case (AL_SNDP_STOP_EVT):
    if (state->state != AL_PLAYING || !snd)
      return;

    delta = (ALMicroTime) _DivS32ByF32 (snd->envelope->releaseTime, state->pitch);
    n_alSynSetVol(&state->voice, 0, delta);
    
    if (delta) {
      evt.common.type  = AL_SNDP_END_EVT;
      evt.common.state = state;
      n_alEvtqPostEvent(&sndp->evtq, (N_ALEvent *)&evt, delta);
      state->state = AL_STOPPING;
    } else {
      /* note: this code is repeated in AL_SNDP_END_EVT */
      n_alSynStopVoice(&state->voice);
      n_alSynFreeVoice(&state->voice);
      _removeEvents(&sndp->evtq, state);
      state->state = AL_STOPPED;
    }            
    break;
    
  case (AL_SNDP_PAN_EVT):
    state->pan = event->pan.pan;
    if (state->state == AL_PLAYING && snd){
      tmp   = state->pan - AL_PAN_CENTER + snd->samplePan;
      tmp   = MAX(tmp, AL_PAN_LEFT);
      pan   = (ALPan) MIN(tmp, AL_PAN_RIGHT);
      n_alSynSetPan(&state->voice, pan);
    }
    break;
    
  case (AL_SNDP_PITCH_EVT):
    /* Limit the pitch to a practical value even though we only need */
    /* to limit it to a non-zero number to avoid divide by zero. */
    if ((state->pitch = event->pitch.pitch) < MIN_RATIO)
      state->pitch = MIN_RATIO;
    
    if (state->state == AL_PLAYING){
      n_alSynSetPitch(&state->voice, state->pitch);
    }
    break;
    
  case (AL_SNDP_FX_EVT):
    state->fxMix = event->fx.mix;
    if (state->state == AL_PLAYING)
      n_alSynSetFXMix(&state->voice, state->fxMix);
    break;
    
  case (AL_SNDP_VOL_EVT):
    state->vol = event->vol.vol;
    if (state->state == AL_PLAYING && snd){
      vtmp  = snd->envelope->decayVolume * state->vol/AL_VOL_FULL;            
      n_alSynSetVol(&state->voice, (s16) vtmp, 1000);
    }
    break;
    
  case (AL_SNDP_DECAY_EVT):
    /*
     * The voice has theoretically reached its attack velocity,
     * set up callback for release envelope - except for a looped sound
     */
    if (snd->envelope->decayTime != -1){
      vtmp   = snd->envelope->decayVolume * state->vol/AL_VOL_FULL;            
      delta = (ALMicroTime) _DivS32ByF32 (snd->envelope->decayTime, state->pitch);
      n_alSynSetVol(&state->voice, (s16) vtmp, delta);
      evt.common.type        = AL_SNDP_STOP_EVT;
      evt.common.state       = state;
      n_alEvtqPostEvent(&sndp->evtq, (N_ALEvent *)&evt, delta);
    }
    break;
    
  case (AL_SNDP_END_EVT):
    /* note: this code is repeated in AL_SNDP_STOP_EVT */
    n_alSynStopVoice(&state->voice);
    n_alSynFreeVoice(&state->voice);
    _removeEvents(&sndp->evtq, state);
    state->state = AL_STOPPED;
    break;
    
  default:
    break;
  }
}

static
  void _removeEvents(ALEventQueue *evtq, N_ALSoundState *state)
{
  ALLink              *thisNode;
  ALLink              *nextNode;
  N_ALEventListItem     *thisItem;
  N_ALEventListItem     *nextItem;
  N_ALSndpEvent         *thisEvent;
  OSIntMask           mask;
  
  mask = osSetIntMask(OS_IM_NONE);
  
  thisNode = evtq->allocList.next;
  while( thisNode != 0 ) {
    nextNode = thisNode->next;
    thisItem = (N_ALEventListItem *)thisNode;
    nextItem = (N_ALEventListItem *)nextNode;
    thisEvent = (N_ALSndpEvent *) &thisItem->evt;
    if (thisEvent->common.state == state){
      if( nextItem )
	nextItem->delta += thisItem->delta;
      alUnlink(thisNode);
      alLink(thisNode, &evtq->freeList);
    }
    thisNode = nextNode;
  }
  osSetIntMask(mask);
}


static s32 _DivS32ByF32 (s32 i, f32 f)
{
#define INT_MAX         2147483647      /* Should be in a limits.h file. */
  
  f64	rd;
  int	ri;
  
#ifdef _DEBUG
  assert(f!=0);	/* Caller must make sure we do not divide by zero! */
#endif
  
  rd = i/f;		/* Store result as a double to avoid overflow. */
  
  if (rd > INT_MAX)	/* Limit the value if necessary. */
    ri = INT_MAX;
  else
    ri = rd;
  
  return ri;
}
