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
#include "n_synthInternals.h"

void n_alSynRemovePlayer( ALPlayer *client)
{
  ALPlayer *thing;
  ALPlayer *prev = 0;
  OSIntMask n_mask;
  
#if 1
  if (n_syn->head != 0)
    {
      OSIntMask mask = osSetIntMask(OS_IM_NONE);
      
      for (thing = n_syn->head; thing != 0; thing = thing->next) {
	if (thing == client) {
	  if(prev)
	    prev->next = thing->next;
	  else
	    n_syn->head = thing->next;
	  
	  client->next = 0;
	  break;
	}
	prev = thing;
      }
      
      osSetIntMask(mask);
    }
#endif    

  n_mask = osSetIntMask(OS_IM_NONE);
  if (n_syn->n_sndp == client)
    n_syn->n_sndp = NULL;
  else if (n_syn->n_seqp1 == client)
    n_syn->n_seqp1 = NULL;
  else if (n_syn->n_seqp2 == client)
    n_syn->n_seqp2 = NULL;

  osSetIntMask(n_mask);
}

