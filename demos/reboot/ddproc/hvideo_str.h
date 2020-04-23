
/*---------------------------------------------------------------------*
        Copyright (C) 1998 Nintendo.
        
        $RCSfile: hvideo_str.h,v $
        $Revision: 1.1 $
        $Date: 1999/07/05 02:17:01 $
 *---------------------------------------------------------------------*/

#ifndef __VIDEO_STR_H__
#define __VIDEO_STR_H__

#include "hglobals.h"

#define STR_MAXLENGTH   64		/*  maximum length of the single
character string */
#define STR_MAXSUM   	512	    /*  total number of characters */

#define STR_DL_BUFSIZE   NUM_DL( STR_MAXSUM )    

#define JSTR_DMA_NUM     400    
#define JSTR_KEEP_COUNT  1
#define JSTR_DMA_SIZE    128

/*  structure for the fixed size font */

typedef struct {
    u8 		*ascii;
	s16 	char_width;
	s16		char_height;
    Bitmap  *bmp;
	s32		bmsize;
    u8 		*img;
} TFont;

extern s32 	video_setString(u8 *str, s32 xpos, s32 ypos, u32 color);
extern void	video_clearString(s32 cur);
extern void video_initString(void);

extern void    	jstr_clean(void);
extern void    	jstr_init(void);
extern void *  	jstr_getBmp(s32 offs);
extern s32     	video_setJStr(u8 *str, s32 xpos, s32 ypos, u32 color);
extern void str_setColor(Sprite *sp, u32 color);
extern s32      str_getWidth(u8 *str);
extern s32      str_getFontWidth(void);
extern s32      str_getFontHeight(void);


#endif /* __VIDEO_STR_H__ */
