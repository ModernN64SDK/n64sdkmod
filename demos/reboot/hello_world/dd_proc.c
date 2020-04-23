
/*---------------------------------------------------------------------*
        Copyright (C) 1998 Nintendo.
        
        $RCSfile: dd_proc.c,v $
        $Revision: 1.1 $
        $Date: 1999/07/05 02:19:36 $
 *---------------------------------------------------------------------*/

/*
   this file written in sjis code.
 */

#include "hddproc.h"

void dd_proc(void);

#define RGBA_WHITE 0xFFFFFFFF

void
dd_proc(void)
{
  static s32 _first=1;
 
  if(_first)
	{
	  si_create();
	  video_create();
	  dd_create();
	  osViBlack(0);
	  _first--;
	}

  video_flush();

/*  main start */

  video_setJStr("To play this sample",30,50,RGBA_WHITE);
  video_setJStr("you need the accompanying game pak.",30,64,RGBA_WHITE);

/*  main end */

  video_finishDLMode();
}


extern Gfx * glistp;

Gfx *
video_getDL()
{
  return glistp;
}

s32
video_setDL( Gfx * pgfx )
{
  glistp = pgfx;
  return -1;
}

