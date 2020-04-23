
/*---------------------------------------------------------------------*
        Copyright (C) 1998 Nintendo.
        
        $RCSfile: dd_proc.c,v $
        $Revision: 1.1 $
        $Date: 1999/07/05 02:20:04 $
 *---------------------------------------------------------------------*/

#include "hmenu.h"
#include "herr.h"

u16 _trig;

static s32 _curscrn = SCREEN_ONETRI;

void dd_proc(void);
void scr_onetri(void);

void
dd_proc(void)
{
  static s32 _first=1;
 
  if(_first)
	{
	  si_create();
	  video_create();
	  rst_create();
	  dd_create();
	  osViBlack(0);
	  _first--;
	}

  video_flush();
  _trig = sb_jpad(si_getTrigger(0));

  switch( _curscrn )
	{
	case SCREEN_MENU:
	  scr_menu();
	  break;
	case SCREEN_ONETRI:
	  scr_onetri();
	  break;
	default:
	  break;
	}
  scr_error();
  
  video_finishDLMode();
}

extern s32 do_texture; 

void
scr_onetri(void)
{
  if( main_getScreen() == SCREEN_ONETRI )/*  key output */
        {
          switch(_trig)
                {
                case L_TRIG:
                case R_TRIG:
				  do_texture ^= 1;
				  break;

                case A_BUTTON:
                  main_setScreen(SCREEN_MENU);
                  return;                        
                default:
                  break;
                }
        }
}  

void 
main_setScreen(s32 i)
{
  _trig = 0;
  _curscrn = i;
}

s32 
main_getScreen()
{
  return _curscrn;
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

