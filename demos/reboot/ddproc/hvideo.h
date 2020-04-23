
/*---------------------------------------------------------------------*
        Copyright (C) 1998 Nintendo.
        
        $RCSfile: hvideo.h,v $
        $Revision: 1.1 $
        $Date: 1999/07/05 02:17:01 $
 *---------------------------------------------------------------------*/
#include "hglobals.h"

#ifndef __VIDEO_H__
#define __VIDEO_H__

#define VIDEO_MSGQ_SIZE 		8

#define VIDEO_DLMODE_IDLE 		NULL
#define VIDEO_DLMODE_SPRITE 	1
#define VIDEO_DLMODE_DRAWRECT   2
#define VIDEO_DLMODE_CUSTOM     99

#define  GLIST_LEN       		512     /*  buffer size for the main DL */
#define  DL_BUFSIZE  NUM_DL( 0x200 )    /*  buffer size for the sprite and
the character string DL */

extern OSMesgQueue      videoMsgQ, retraceMsgQ;

extern void    	video_create(void);
extern Gfx *   	video_getDL(void);
extern s32     	video_setDL( Gfx * pgfx );
extern s32     	video_getCurrentBuffer(void);
extern void    	video_flush(void);

extern s32  	video_setSprite(Sprite *sp ,s32 xpos,s32 ypos);
extern void 	video_startDLMode(s32 mode);
extern void 	video_finishDLMode(void);
extern s32     	video_getCurrentDLMode(void);
extern void    	video_clearSprite(s32 cur);
extern void    	video_initSprite(void);
extern s32      video_drawRect(s32 x,s32 y,s32 w,s32 h ,u32 rgba);

#endif



