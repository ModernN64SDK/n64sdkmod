
/*---------------------------------------------------------------------*
        Copyright (C) 1998 Nintendo.
        
        $RCSfile: hsb.h,v $
        $Revision: 1.1 $
        $Date: 1999/07/05 02:17:00 $
 *---------------------------------------------------------------------*/

/*
   sb.h  general function, StringBar, and BackTile
 */

#ifndef __SB_H__
#define __SB_H__

#include "hglobals.h"
#include "hdd.h"

#define  FORI(j)    for(i=0;i<j;i++) 
#define  FORJ(size) for(j=0;j<size;j++)

#define  XOR(a,b)  ((a && !b) || (!a && b))

#define RGBA_WHITE			0xFFFFFFFF
#define RGBA_GREY			0x888888FF
#define RGBA_PINK			0xFF0088FF
#define RGBA_SKYBLUE		0x8888FFFF
#define RGBA_SKYBLUEALPHA	0x8888FF80
#define RGBA_BLUEALPHA	    0x000080C0
#define RGBA_VIOLETALPHA    0x600060C0
#define RGBA_VIOLET         0x600060FF

#define AS_HEX  1
#define AS_DEC  2
#define AS_ASC  4
#define AS_SUBST  8

#define EXIST_IN(x,y,sq) ( \
		  (x > (sq).left) && (x < (sq).right) && \
		  (y > (sq).top) && (y < (sq).bottom) \
          )

#define SBAR_MAXLENGTH      256  

#define BD_X (-5)
#define BD_Y 0   

typedef struct {
  s32 x;
  s32 y;
} TPoint ;

typedef struct {
  s32 left;
  s32 top;
  s32 right;
  s32 bottom;
}TSquare;


typedef struct {
  u8  str[SBAR_MAXLENGTH];
  s32 x;
  s32 y;
  TPoint base;
  s32 clipping;
  TSquare clip;
  u32 color;
  s32 visible;
  s32 autoprintf; 
  s32 length;
  void * src;
  s32 src_as;
  s32 src_length;
  u8  **src_sub;    /*  enable selection by substitute macro */
} TStringBar;

typedef struct {
  s32 x;
  s32 y;
  s32 w;
  s32 h;
  TPoint base;
  s32 clipping;
  TSquare clip;
  u32 color;
  s32 visible;
} TBackTile;

extern s32     	sb_clip(s32 val ,s32 from ,s32 to);
extern u16     	sb_jpad( u16 trig );
extern s32     	sb_length(u8 *s);

extern s32     	sbar_create(TStringBar * bar,u8 * str, s32 xpos ,s32 ypos );
extern s32    	sbar_show(TStringBar * bar);
extern s32     	sbar_setBase(TStringBar * bar,s32 x ,s32 y);
extern s32     	sbar_setClip(TStringBar * bar,s32 left ,s32 top ,s32 right ,s32 bottom );

extern s32 		sbar_setSubst(TStringBar * bar,void * src, u8 **src_sub ,s32 length );
extern s32 		sbar_setSrc(TStringBar * bar,void * src, s32 type ,s32 length );
extern s32     	sbar_moveCenter(TStringBar * bar ,s32 left ,s32 right );
extern s32     	sbar_getWidth(TStringBar * bar);

extern s32     	btile_create(TBackTile *bt ,s32 xpos ,s32 ypos ,s32 width ,s32 height ,u32 color);
extern s32     	btile_setBase(TBackTile * bt ,s32 x ,s32 y);
extern s32     	btile_moveCenter(TBackTile * bt ,s32 left ,s32 right );
extern s32     	btile_setClip(TBackTile * bt,s32 left ,s32 top ,s32 right ,s32 bottom );
extern s32     	btile_show(TBackTile * bt);

#endif /* __SB_H__ */
