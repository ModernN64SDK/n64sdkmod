
/*---------------------------------------------------------------------*
        Copyright (C) 1998 Nintendo.
        
        $RCSfile: menu.c,v $
        $Revision: 1.1 $
        $Date: 1999/07/05 02:20:04 $
 *---------------------------------------------------------------------*/

/*
   this file written in sjis code.
 */

/* pre-process */

#include "hmenu.h"

/* declarerations */

#define ITEM_H    20  
#define BTILE_NUM 1

static TStringBar  _label[1];
static TBackTile   _btile[BTILE_NUM];

/* functions */
s32 _menuInit(void);

/* implementation */

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

  for(i=0;i< BTILE_NUM ;i++)  btile_show(&_btile[i]);
  sbar_show(&_label[0]);

/*  key proc */

  switch(_trig)
	{
	case B_BUTTON:
	  main_setScreen( SCREEN_ONETRI );
	  return;
	}
}

s32
_menuInit(void)
{
  s32 xpos,ypos;

  xpos = 50;
  ypos = 50;

  sbar_create(&_label[0],"Restarted" ,xpos,ypos );
  sbar_moveCenter(&_label[0],0,SCREEN_WD);

  btile_create(&_btile[0] , _label[0].x + BD_X ,_label[0].y - 2
			   ,sbar_getWidth(&_label[0]) + 15 , ITEM_H ,RGBA_BLUEALPHA );

  return -1;
}

