
/*---------------------------------------------------------------------*
        Copyright (C) 1998 Nintendo.
        
        $RCSfile: video_str.c,v $
        $Revision: 1.1 $
        $Date: 1999/07/05 02:17:01 $
 *---------------------------------------------------------------------*/

/* pre-process */

#include "hglobals.h"
#include "hvideo.h"
#include "hvideo_str.h"

#ifndef __NO_STRING__

/*  #define DYN_SPRITE_HACK */

/*  If this font data is too big (or not necessary), modify it to load it when
    executing. */

#include "Texture/font_sys.h"

/* variables */

/*  buffer */
static Bitmap str_bmpbuf[STR_MAXLENGTH];
static Gfx   *str_dlb[2]; 					/* STR_DL_BUFSIZE *8 bytes */

/*  Font currently selected */
static TFont _font;

/*  Total number of characters */
s32 str_charsum;

/*  Sprite structure to create the display list. */
/*  Used repeatedly for all character string sprites on the screen. */

Sprite str_sprite = {
        0,0,                    /* Position: x,y */
        0,0,                    /* width,height : fill in later */
        1.0,1.0,                /* Sprite Scale: x,y */
        0,0,                    /* Explosion (x,y) */
        SP_CUTOUT | SP_TRANSPARENT, /* Sprite Attributes */
        0x1234,                 /* Sprite Depth: Z */
        255,255,255,255,        /* Sprite Coloration: RGBA */
        0,0,NULL,               /* Color LookUp Table: start_index, length, address */
        0,1,                    /* Sprite Bitmap index: start index, step increment */
        NULL,                   /* Number of bitmaps */
		STR_DL_BUFSIZE,			/* Number of display list locations
									 allocated ( for assert only )*/
        15, 128,               /* Sprite Bitmap Height: Used_height, physical height */
        G_IM_FMT_I,            /* Sprite Bitmap Format */
        G_IM_SIZ_DD,           /* Sprite Bitmap Texel Size */
        str_bmpbuf,            /* bitmap : Pointer to bitmaps */
        (Gfx *)NULL,           /* rsp_dl : Display list memory ( Gfx * )str_dlbuf */
        (Gfx *)NULL,           /* rsp_dl_next : HACK: dynamic_dl pointer */
};

/* protected functions */

s32     	str_makeSprite( Sprite *sp, char *str, TFont *font );
s32     	str_getCharIndex( u8 *str, char target );
/*  void    	str_setColor(Sprite *sp, u32 color); */

/* implementation */

s32
str_makeSprite( Sprite *sp, char *str, TFont *font )
{
    s32  i , idx;
    Bitmap * bm;

    bm = sp->bitmap;
	sp->width = 0;

	for(i=0; i < STR_MAXLENGTH; i++) {
	    if( str[i] == 0 ) {
		  bcopy( font->bmp ,bm , sizeof(Bitmap) );
		  bm->width = -1;
		  break;
	    };
	    if( str[i] == ' ' || str[i] == '\n')
		  {
			bcopy( font->bmp ,bm , sizeof(Bitmap) );
			sp->width  += font->bmp->width;
			bm->buf = NULL;
		  }
		else
		  {
			idx = str_getCharIndex( font->ascii ,str[i] );
			if( idx != -1 ) {
			  bcopy( &(font->bmp[idx]) ,bm , sizeof(Bitmap) );
			  sp->width  += font->bmp[idx].width;
			} else {
			  bcopy( &(font->bmp[0]) ,bm , sizeof(Bitmap) );
			  bm->buf = NULL;
			  sp->width  += font->bmp[0].width;
			};
		  };
		bm++;
	};
	  
    sp->nbitmaps = i;
    sp->height = font->char_height;

    return i;
}

s32
str_getCharIndex( u8 *str, char target )
{
  	u8 * p;
	s32 ret;
	
	p = str;
    while( *p && (*p != target) )
	p++;
    
    if( *p ) ret = p - str;
	  else ret = -1;

	return ret;
}

void
video_initString(void)
{
  extern Gfx heap_str_dlb[][STR_DL_BUFSIZE];
  s32 i;

  FORI(2)
	{
	  str_dlb[i] = heap_str_dlb[i];
	  bzero( (void *)str_dlb[i],STR_DL_BUFSIZE * sizeof(Gfx) );
	}

 video_clearString(0);
 jstr_init();
 jstr_clean();
}

void
video_clearString( s32 cur )
{
  str_charsum = 0;

  str_sprite.rsp_dl      = (Gfx *)(&str_dlb[cur][0]);
  str_sprite.rsp_dl_next = str_sprite.rsp_dl;
  bcopy( &font_default, &_font ,sizeof(TFont));

  jstr_clean();
}

s32
video_setString(u8 *str, s32 xpos, s32 ypos, u32 color)
{
  Gfx     *dl,*dl2;
  Sprite  *sp;

  sp = &str_sprite;
  sp->bmsiz = G_IM_SIZ_4b;

  if( VIDEO_DLMODE_SPRITE != video_getCurrentDLMode() )
	  video_startDLMode(VIDEO_DLMODE_SPRITE);

  dl = (Gfx *)video_getDL();
  if ( dl )
	{
	  str_charsum += str_makeSprite( sp, str, &_font);
	  str_setColor( sp, color );
	  spMove( sp, xpos , ypos );
	  dl2 = spDraw( sp );
	  
	  if( dl2 )
		{
		  gSPDisplayList(dl++,dl2);
		  video_setDL(dl);
		}
	}  

  return -1;
}


void
str_setColor(Sprite *sp, u32 color)
{
    sp->red   = (color>>24) & 0xFF;
    sp->green = (color>>16) & 0xFF;
    sp->blue  = (color>>8) & 0xFF;
    sp->alpha = color & 0xFF;
}

s32
str_getWidth(u8 *str) /*  a length of character string using current font */
{
  Sprite  *sp=&str_sprite;

  sp = &str_sprite;
  str_makeSprite( sp, str, &_font);

  return sp->width;
}

s32
str_getFontWidth(void) /*  */
{
  return _font.char_width;
}

s32
str_getFontHeight(void) /*  */
{
  return _font.char_height;
}

#endif
