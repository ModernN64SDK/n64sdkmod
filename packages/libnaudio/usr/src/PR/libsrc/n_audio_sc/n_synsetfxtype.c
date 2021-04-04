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
#include "n_synthInternals.h"


ALFxRef n_alReverbSetType(s16 bus, ALFxId fxid, s32 rate)
{
  n_syn->mainBus->filter.handler = (N_ALCmdHandler)n_alFxPull;
  return( n_syn->auxBus->fx_array[bus] );
}

void n_alSynSetFXtype(s16 bus, ALFxId fxid, s32 rate)
{
    n_syn->auxBus->fx = n_alReverbSetType(bus, fxid, rate);
}

