/*---------------------------------------------------------------------*
        Copyright (C) 1998 Nintendo.
        
        $RCSfile: video_core.c,v $
        $Revision: 1.1 $
        $Date: 1999/07/05 02:17:01 $
 *---------------------------------------------------------------------*/


/* pre-process */

#include "hglobals.h"
#include "hvideo.h"
#include  "hvideo_str.h"
#include "hsi.h"
#include "hdd.h"

/* variables */

static s32 	draw_buffer = 0;

/* protected functions */

void    	video_initDL(s32 buf);
void    	video_clearDL(s32 cur);

/* implmentations */

void
video_create(void)
{
  /*initialize video heap */
  video_initSprite();
  video_initString();
}

void 
video_clearDL(s32 cur) /* dont extern */
{
  video_clearSprite(cur);
  video_clearString(cur);
}

s32
video_getCurrentBuffer(void)  /* draw_buffer to read_only */
{
  return draw_buffer;
}

void
video_flush(void)
{

/*  from core */
  dd_flush();
  si_ticker();

  /* switching of the buffer */
  draw_buffer ^= 1;

  /* clear the display list*/
  video_clearDL( draw_buffer );
}

#if 0
/*
   tech note

   call video_flush() at the head of frame

 */
#endif
