/*
  cfb.c
    Copyright (C) 1999, NINTENDO Co,Ltd., MONEGI CORPORATION.
    Jan. 28, 1999.
*/

/*
 * RSP view of the frame buffer.  It exists to create an RSP address for
 * the frame buffer, which is remapped on each frame to either of two
 * regions of physical memory that store that actual bits.
 */

#include <ultra64.h>
#include "FrameBuffer.h"

#ifdef	USE_CFB32
u32 cfb[ 2 ][ SCREEN_WD*SCREEN_HT ];
#else	/* USE_CFB32 */
u16 cfb[ 2 ][ SCREEN_WD*SCREEN_HT ];
#endif	/* USE_CFB32 */
