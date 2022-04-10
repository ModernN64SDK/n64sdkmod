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
#include "n_sndp.h"

void n_alSndpSetPitch(f32 pitch) 
{
    N_ALSndpEvent evt;
    N_ALSndPlayer *sndp = (N_ALSndPlayer *)n_syn->n_sndp;

    N_ALSoundState  *sState = sndp->sndState;

    /*
     * If this is set during playback there will be
     * a the envelope lengths won't be accurate - but you can still
     * do it. 
     */

#ifdef _DEBUG
    if ((sndp->target >= sndp->maxSounds) || (sndp->target < 0)){
        __osError(ERR_ALSNDPSETPAR, 2, sndp->target, sndp->maxSounds-1);
	return;
    }
#endif

    evt.pitch.type = AL_SNDP_PITCH_EVT;
    evt.pitch.state = &sState[sndp->target];
    evt.pitch.pitch = pitch;
    n_alEvtqPostEvent(&sndp->evtq, (N_ALEvent *)&evt, 0);
}

