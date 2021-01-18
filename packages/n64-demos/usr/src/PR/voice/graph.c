/*---------------------------------------------------------------------
        Copyright (C) 1998 Nintendo.
        
        File            graph.c
        Coded    by     Tetsuyuki Ootsuka.      July, 1998.
        Comments        functions for graphics
   
        $Id: 
   ---------------------------------------------------------------------*/
#include	<ultra64.h>
#include	"nu64sys.h"
#include	"graph.h"


/*---------------------------------------------------------------------
                   Ascii character on the screen
  ---------------------------------------------------------------------*/
void
putc(int x, int y, u16 color, char c)
{
  s32	i, j;
  u16	*p = (u16 *)cfb[draw_buffer] + x + SCREEN_WD*y;
  u8    *pc = pat[c-0x20];

  for (j = 0; j < CHAR_HT; j ++, pc++) {
    for (i = CHAR_WD - 1; i >= 0; i --) {
      if (*pc & (1 << i)) {
	*p = color;
      } else {
	*p = BGCOLOR;
      }
      p ++;
    }
    p += SCREEN_WD - CHAR_WD;
  }
}


/*---------------------------------------------------------------------
                  Ascii string on the screen
  ---------------------------------------------------------------------*/
void
prints(int x, int y, u16 color, char *s)
{
  s32	i;

  for (i = 0 ; *s != NULL; i ++, x+=CHAR_WD) {
    putc(x , y, color, *s++);
  }
}


/*---------------------------------------------------------------------
                  Sjis string on the screen
  ---------------------------------------------------------------------*/
void
printkanji(u16 *p, int x, int y, u16 color, s16 *s)
{
  s32 	i;

  for(i = 0 ; s[i]!=-1 ; i++, x+=16) putkanji(p, x, y, color, s[i]);
}


/*---------------------------------------------------------------------
                  Sjis character on the screen
  ---------------------------------------------------------------------*/
void
putkanji(u16 *p, int x, int y, u16 color, s16 data)
{
  s32   i, j;
  u16   tmp;
  u8    *kp = kanji_font + data;

  p += x + SCREEN_WD*y;

  for (j = 0; j < 16 ; j ++) {
    for (i = 0 ; i < 8 ; i ++) {
      tmp = (((u16)*kp >> 2) & 0x3c);
      tmp = (tmp << 10) + (tmp << 5) + tmp+1;
      *p++ = (tmp & color);
      tmp = ((u16)(*kp & 0x0f)<<2);
      tmp = (tmp << 10) + (tmp<<5) + tmp+1;
      *p++ = (tmp & color);
      kp ++;
    }
    p += SCREEN_WD - 16;
  }
}


/*---------------------------------------------------------------------
                  Clear the screen
  ---------------------------------------------------------------------*/
void
gcls(u16 *p)
{
  s32 	i;
  
  for (i = 0; i < SCREEN_WD*SCREEN_HT; i ++) {
    *p++ = BGCOLOR;
  }
}




