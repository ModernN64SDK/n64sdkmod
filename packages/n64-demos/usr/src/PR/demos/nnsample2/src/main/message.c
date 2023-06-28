/*============================================================================

		NINTENDO64 TECHNICAL SUPPORT CENTER 
		
		    NINTENDO64 SAMPLE PROGRAM 2

		Copyright (C) 1997, NINTENDO Co,Ltd.

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

