
/*---------------------------------------------------------------------*
        Copyright (C) 1998 Nintendo.
        
        $RCSfile: menu.c,v $
        $Revision: 1.1 $
        $Date: 1999/07/05 02:20:13 $
 *---------------------------------------------------------------------*/

/*
   this file written in sjis code.
 */

/* pre-process */

#include "hmenu.h"
#include "hsb.h"

/* declarations */

#define MENU_NUM  2
#define ITEM_H    20  
#define BTILE_NUM 2

static s32 _selected = 0;
static TStringBar  _menu[MENU_NUM];
static TStringBar  _label[1];
static TBackTile   _btile[BTILE_NUM];

static TMessage4dd _ddmsg = {0};

/* functions */
s32 _menuInit(void);
extern void _writeonly_automode(void);

/* implementation */
void dd_ready_for_reinit(void);

void
scr_menu(void)
{
  static s32 _first = 1;
  s32 i;

  if (_first)
	{
	  _menuInit();
	  _first = 0;
	}

/*  printing */
  
  _btile[1].y = _menu[_selected].y + BD_Y; 

  for(i=0;i< BTILE_NUM ;i++)  btile_show(&_btile[i]);
  sbar_show(&_label[0]);
  for(i=0;i< MENU_NUM ;i++)  sbar_show(&_menu[i]);

/*  key proc */

  switch(_trig)
	{
	case B_BUTTON:
	  main_setScreen( SCREEN_TOPGUN );
	  return;
	case U_JPAD: _selected = sb_clip(--_selected,0,MENU_NUM-1);break;
	case D_JPAD: _selected = sb_clip(++_selected,0,MENU_NUM-1);break;
	}

  if( _trig == A_BUTTON )
  switch( _selected )
	{
	case 0: 
	  main_setScreen( SCREEN_SUMMON );
	  break;
	case 1: 
	  main_setScreen( SCREEN_TOPGUN );
	  break;
	default:
	  break;
	}

}

s32
_menuInit(void)
{
  s32 xpos,ypos;

  xpos = 50;
  ypos = 50;

  sbar_create(&_label[0],"Reboots onetri" ,xpos,ypos );
  sbar_moveCenter(&_label[0],0,SCREEN_WD);

  xpos = _label[0].x + 10; 
  sbar_create(&_menu[0],"O.K." 	             ,xpos,(ypos += ITEM_H) );
  sbar_create(&_menu[1],"Cancel"             ,xpos,(ypos += ITEM_H) );

  btile_create(&_btile[0] , _label[0].x + BD_X ,_label[0].y + BD_Y
			   ,sbar_getWidth(&_label[0]) + 15 , ITEM_H * 3 ,RGBA_BLUEALPHA );

  btile_create(&_btile[1] , _label[0].x + BD_X ,_label[0].y + BD_Y 
			   ,_btile[0].w , ITEM_H , RGBA_SKYBLUEALPHA );

  _ddmsg.retQ = &myQ;

  return -1;
}

