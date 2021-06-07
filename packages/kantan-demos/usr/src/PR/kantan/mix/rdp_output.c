/*============================================================================
  rdp_output.c
============================================================================*/

#include <ultra64.h>
#include <PR/ramrom.h>
#include <assert.h>
#include "message.h"
#include "def.h"

/* RSP writes back RDP data */
u64 rdp_output[GFX_RDP_OUTPUT_SIZE];
