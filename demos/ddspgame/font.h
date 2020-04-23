/*---------------------------------------------------------------------*
        Copyright (C) 1998 Nintendo.
        
        $RCSfile: font.h,v $
        $Revision: 1.2 $
        $Date: 1998/11/13 12:23:45 $
 *---------------------------------------------------------------------*/
/*
	Fonts: Definitions and externals.
*/

#ifndef __FONT_H__
#define __FONT_H__

#include <PR/sp.h>
#include "main.h"

#define ERROR_MES_SIZE  26

typedef struct sp2d_font_struct {
    char *index_string;
    Bitmap *bitmaps;
    char *img;
} Font;

extern OSPiHandle		*handler;

extern OSMesgQueue		dmaMessageQ;
extern OSMesg			dmaMessageBuf;
extern OSIoMesg			dmaIOMessageBuf;

extern Font lcase_font;
extern unsigned char    kbuffer[];

extern void text_sprite( Sprite *, char *, Font *, int , int );
extern s32  setupkanji(void);

extern void font_init( Gfx ** );
extern void font_finish( Gfx ** );
extern void font_show_num( Gfx **, int );
extern void font_show_string( Gfx **, char * );

extern void jfont_show_string( Gfx **, u16 );
extern void font_set_scale( double, double );
extern void font_set_color( unsigned char, unsigned char, unsigned char, unsigned char );
extern void font_set_win( int, int );
extern void font_set_pos( int, int );

extern void font_set_transparent( int );

#endif
