/*
  framebuffer.c
    Copyright (C) 1999, NINTENDO Co,Ltd., MONEGI CORPORATION.
    Jan. 28, 1999.
*/

#include <ultra64.h>
#include "FrameBuffer.h"

extern Gfx clearcfb_dl[];

static OSMesgQueue retraceMessageQ;
static OSMesg      retraceMessageBuf;

void init_framebuffer( void )
{
    osCreateMesgQueue( &retraceMessageQ, &retraceMessageBuf, 1 );
    osViSetEvent( &retraceMessageQ, NULL, 1 );
}

Gfx *clear_cfb( Gfx *glistp )
{
    gSPDisplayList(glistp++, clearcfb_dl);

    return glistp;
}

void swap_framebuffer( int draw_buffer )
{
    osViSwapBuffer( cfb[ draw_buffer ] );

    if ( MQ_IS_FULL(&retraceMessageQ) )
        (void)osRecvMesg( &retraceMessageQ, NULL, OS_MESG_BLOCK );

    (void)osRecvMesg( &retraceMessageQ, NULL, OS_MESG_BLOCK );
}

