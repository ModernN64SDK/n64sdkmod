
/*---------------------------------------------------------------------*
        Copyright (C) 1998 Nintendo.
        
        $RCSfile: btile.c,v $
        $Revision: 1.1 $
        $Date: 1999/07/05 02:16:59 $
 *---------------------------------------------------------------------*/
/*
     btile.c

         To draw rectangle tile, refer to sb.h 

*/

#include "hglobals.h"
#include "hsb.h"
#include "hdd.h" 
#include "hvideo.h"
#include "hvideo_str.h"

s32
btile_create(TBackTile *bt ,s32 xpos ,s32 ypos ,s32 width ,s32 height ,u32 color)
{
  try_assert("btile create : invalid width"  ,width>0 );
  try_assert("btile create : invalid height" , height>0 );

  bt->x = xpos;
  bt->y = ypos;
  bt->w = width;
  bt->h = height;
  bt->color = color;
  bt->visible = true;
  bt->clipping = false;
  bt->base.x=0;
  bt->base.y=0;
  return -1;
}

s32
btile_setBase(TBackTile * bt ,s32 x ,s32 y)
{
  bt->base.x = x;
  bt->base.y = y;
  return -1;
}

s32
btile_moveCenter(TBackTile * bt ,s32 left ,s32 right )
{
  try_assert("btile movecenter : right < left " , ((right - left) > 0 ) );
  bt->x = left + ( (right - left - bt->w) / 2 );
  return -1;
}

s32
btile_setClip(TBackTile * bt,s32 left ,s32 top ,s32 right ,s32 bottom )
{
  bt->clip.left    = left;
  bt->clip.top     = top;
  bt->clip.right   = right;
  bt->clip.bottom  = bottom;
  bt->clipping = true;

  return -1;
}

s32
btile_show(TBackTile * bt)
{
  s32 x,y;

  if ( !bt->visible ) return 0;
  x = bt->x + bt->base.x;
  y = bt->y + bt->base.y;

  if( !bt->clipping || EXIST_IN(x,y,bt->clip) ) 
	{
	  video_drawRect(bt->x,bt->y,bt->w,bt->h ,bt->color);
	}
  return -1;
}

