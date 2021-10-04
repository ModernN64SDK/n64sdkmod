/*---------------------------------------------------------------------
        Copyright (C) 1998 Nintendo.
        
        File            cont.h
        Coded    by     Koji Mitsunari. Oct  2, 1997.
        Modified by     Koji Mitsunari. Oct 27, 1998.
        Comments        functions for graphics
   
        $Id: 
   ---------------------------------------------------------------------*/
#include	<ultra64.h>
#include	"nu64sys.h"
#include	"graph.h"

/*---------------------------------------------------------------------
                  Display character on the screen
  ---------------------------------------------------------------------*/
void
putc(u16 *p, int x, int y, u16 color, u8 c)
{
  int	i, j;
  u8	*pc = pat[c-0x20];

  p = p + x+y*SCREEN_WD;

  for (j = 0; j < CHAR_HT; j ++, pc++) {
    for (i = CHAR_WD - 1; i >= 0; i --) {
      if (*pc & (1 << i)) {
	*p = color;
      } else {
	*p = 0;
      }
      p ++;
    }
    p += SCREEN_WD - CHAR_WD;
  }
}


/*---------------------------------------------------------------------
                  Display string on the screen
  ---------------------------------------------------------------------*/
void
prints(u16 *p, int x, int y, u16 color, u8 *s)
{
    int i;

    for (i = 0 ; *s != NULL; i ++, x+=CHAR_WD) {
      putc(p, x , y, color, *s++);
    }
}

/*---------------------------------------------------------------------
                  Clear the screen
  ---------------------------------------------------------------------*/
void
gcls(u16 *p){
  int 	i;
  u16	*ptr = p;

  for (i = 0; i < SCREEN_WD*SCREEN_HT; i ++) {
    *ptr++ = BGCOLOR;
  }
}
