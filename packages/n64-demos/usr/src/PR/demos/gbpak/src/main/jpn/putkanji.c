/*---------------------------------------------------------------------
        Copyright (C) 1998 Nintendo.
        
        File            printkanji.c
        Coded    by     Koji Mitsunari. Apr 25, 1998.
        Modified by     
        Comments        Function to display kanji
   
        $Id: 
   ---------------------------------------------------------------------*/
#include	<ultra64.h>
#include	"nu64sys.h"
#include	"src/main/graph.h"

void
putkanji(u16 *p, int x, int y, u16 color, s16 data)
{
  u16	tmp;
  int	i, j;
  u8	*kp = kanji_font + data;

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
