/*====================================================================
 * audio.h
 *
 * Synopsis:
 *
 * Copyright 1995, Silicon Graphics, Inc.
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

/*---------------------------------------------------------------------*
        Copyright (C) 1998 Nintendo. (Originated by SGI)
        
        $RCSfile: audio.h,v $
        $Revision: 1.1.1.1 $
        $Date: 2002/05/02 03:27:21 $
 *---------------------------------------------------------------------*/

#ifndef __audio__
#define __audio__

#include <ultra64.h>
#include <libaudio.h>
#include <sched.h>

#define MAX_UPDATES             32
#define MAX_EVENTS              32
#define AUDIO_HEAP_SIZE         300000

#define MAX_VOICES              22
#define EXTRA_SAMPLES           80
#define NUM_OUTPUT_BUFFERS      3      /* Need three of these */
#define OUTPUT_RATE             44100
#define MAX_MESGS               8
#define QUIT_MSG                10

#define DMA_BUFFER_LENGTH       0x800  /* Larger buffers result in fewer DMA' but more  */
                                       /* memory being used.  */


#define NUM_ACMD_LISTS          2      /* two lists used by this example                */
#define MAX_RSP_CMDS            4096   /* max number of commands in any command list.   */
                                       /* Mainly dependent on sequences used            */

#define NUM_DMA_BUFFERS         24     /* max number of dma buffers needed.             */
                                       /* Mainly dependent on sequences and sfx's       */

#define NUM_DMA_MESSAGES        32     /* The maximum number of DMAs any one frame can  */
                                       /* have.                                         */

#define FRAME_LAG               1      /* The number of frames to keep a dma buffer.    */
                                       /* Increasing this number causes buffers to not  */
                                       /* be deleted as quickly. This results in fewer  */
                                       /* DMA's but you need more buffers.              */


#define AUDIO_STACKSIZE         0x2000

#define MAX_SEQ_LENGTH  20000


typedef struct {
    u32       outputRate;
    u32       framesPerField;
    u32       maxACMDSize;
} amConfig;


void    amCreateAudioMgr(ALSynConfig *c, OSPri priority, amConfig *amc);

extern u64        audYieldBuf[];

#endif

