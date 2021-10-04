/*---------------------------------------------------------------------
        Copyright (C) 1998 Nintendo.
        
        File            printkanji.c
        Coded    by     Koji Mitsunari. Apr 25, 1998.
        Modified by     
        Comments        Function to display kanji character strings
   
        $Id: 
   ---------------------------------------------------------------------*/
#include	<ultra64.h>
#include	"nu64sys.h"
#include	"src/main/graph.h"

void
printkanji(u16 *p, int x, int y, u16 color, s16 *s)
{
  int i;
  for(i = 0 ; s[i]!=-1 ; i++, x+=16) putkanji(p, x, y, color, s[i]);
}
