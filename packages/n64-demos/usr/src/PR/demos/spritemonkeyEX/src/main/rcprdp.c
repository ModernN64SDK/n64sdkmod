/*
  rcprdp.h
    Copyright (C) 1999, NINTENDO Co,Ltd., MONEGI CORPORATION.
    Jan. 28, 1999.
*/

#include <ultra64.h>
#include "RcpRdp.h"
#include "FrameBuffer.h"

extern Gfx rspinit_dl[];
extern Gfx rdpinit_dl[];

Gfx *init_rcprdp( Gfx *glistp, char *staticSegment, int num_of_cfb )
{
    /*
     * Tell RCP where each segment is
     */
    gSPSegment( glistp++, 0, 0x0 );	/* Physical address segment */
    gSPSegment( glistp++, STATIC_SEGMENT, OS_K0_TO_PHYSICAL(staticSegment) );
    gSPSegment( glistp++, CFB_SEGMENT,
                OS_K0_TO_PHYSICAL( cfb[ num_of_cfb ] ) );

    gSPDisplayList( glistp++, rdpinit_dl );
    gSPDisplayList( glistp++, rspinit_dl );

    return glistp;
}


