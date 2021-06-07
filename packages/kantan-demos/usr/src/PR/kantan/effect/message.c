/*============================================================================
  message.c
============================================================================*/

#include <ultra64.h>
#include <PR/ramrom.h>
#include <assert.h>
#include "message.h"

OSMesg PiMessages[NUM_PI_MSGS];
OSMesg dmaMessageBuf;
OSMesgQueue PiMessageQ;
OSMesgQueue dmaMessageQ;
OSIoMesg dmaIOMessageBuf;
