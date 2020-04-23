
/*---------------------------------------------------------------------*
        Copyright (C) 1998 Nintendo.
        
        $RCSfile: dd_proc.c,v $
        $Revision: 1.1 $
        $Date: 1999/07/05 02:20:13 $
 *---------------------------------------------------------------------*/



#include "hmenu.h"
#include "herr.h"
#include "htry.h"

OSMesgQueue             myQ;
OSMesg                  myQBuf[1];
u16 _trig;

static s32 _curscrn = SCREEN_TOPGUN;

void dd_proc(void);
void scr_topgun(void);

void
dd_proc(void)
{
  static s32 _first=1;
 
  if(_first)
	{
	  osCreateMesgQueue(&myQ, myQBuf, 1);
	  si_create();
	  video_create();
	  rst_create();
	  try_assert("fail ddcreate",dd_create());
	  osViBlack(0);
	  _first--;
	}

  video_flush();
  _trig = sb_jpad(si_getTrigger(0));

  if(_trig)
	{
	  switch(_trig)
		{
		case A_BUTTON:		  
		  break;
		}

	}

  switch( _curscrn )
	{
	case SCREEN_MENU:
	  scr_menu();
	  break;
	case SCREEN_RTC:
	  scr_rtc();
	  break;
	case SCREEN_SUMMON:
	  scr_summon();
	  break;
	case SCREEN_TOPGUN:
	  scr_topgun();
	  break;
	default:
	  break;
	}
  scr_error();
  
  video_finishDLMode();
}

void
scr_topgun(void)
{
  if( main_getScreen() == SCREEN_TOPGUN )/*  key output */
        {
          switch(_trig)
                {
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

