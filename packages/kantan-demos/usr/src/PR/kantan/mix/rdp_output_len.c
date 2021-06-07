/*============================================================================
  rdp_output_len.c
============================================================================*/

#include <ultra64.h>
#include <PR/ramrom.h>
#include <assert.h>
#include "message.h"
#include "def.h"

/* RSP writes back size of RDP data */
u64 rdp_output_len;
