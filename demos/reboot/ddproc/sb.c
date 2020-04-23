
/*---------------------------------------------------------------------*
        Copyright (C) 1998 Nintendo.
        
        $RCSfile: sb.c,v $
        $Revision: 1.1 $
        $Date: 1999/07/05 02:17:01 $
 *---------------------------------------------------------------------*/

#include "hglobals.h"
#include "htry.h"
#include "hsb.h"
#include "hdd.h" 
#include "hvideo_str.h"

s32
sbar_create(TStringBar * bar,u8 * str, s32 xpos ,s32 ypos )
{
  try_assert("sbar create : str is null" ,(s32)str);
  try_assert("sbar create : str is too long" ,( sb_length(str) < SBAR_MAXLENGTH ));

  sprintf( bar->str,str);
  bar->x = xpos;
  bar->y = ypos;
  bar->color = RGBA_WHITE ;
  bar->visible = true;
  bar->clipping = false;
  bar->autoprintf = false; 
  bar->base.x=0;
  bar->base.y=0;
  return -1;
}

s32
sbar_moveCenter(TStringBar * bar ,s32 left ,s32 right )
{
  try_assert("sbar movecenter : right < left " , ((right - left) > 0 ) );
  bar->x = left + ( (right - left - sbar_getWidth(bar) ) / 2 );
  return -1;
}

s32
sbar_getWidth(TStringBar * bar)
{
  return ( bar->autoprintf ) ?
	str_getFontWidth() * bar->src_length : str_getWidth(bar->str);
}

s32
sbar_setBase(TStringBar * bar,s32 x ,s32 y)
{
  bar->base.x = x;
  bar->base.y = y;
  return -1;
}

s32
sbar_setClip(TStringBar * bar,s32 left ,s32 top ,s32 right ,s32 bottom )
{
  bar->clip.left    = left;
  bar->clip.top     = top;
  bar->clip.right   = right;
  bar->clip.bottom  = bottom;
  bar->clipping = true;

  return -1;
}

s32
sbar_setSrc(TStringBar * bar,void * src, s32 type ,s32 length )
{
  if( (type != AS_ASC ) && (length > 16) ) length=16;

  bar->src = src;
  bar->src_as = type;
  bar->src_length   = length;
  bar->autoprintf = true; 
  return -1;
}

s32
sbar_setSubst(TStringBar * bar,void * src, u8 ** src_sub ,s32 length )
{
  bar->src = src;
  bar->src_as = AS_SUBST;
  bar->src_length   = length;
  bar->src_sub  = src_sub;
  bar->autoprintf = true; 
  return -1;
}

s32
sbar_show(TStringBar * bar)
{
  s32 i,x,y;
  s32 ret;

  if ( !bar->visible ) return 0;
  if( bar->autoprintf )
	{
	  switch( bar->src_as )
		{
		case AS_HEX:
		  sprintf( bar->str ,"0x");
		  for(i=0;i< bar->src_length ;i++)
			sprintf( bar->str + i*2 + 2 ,"%02x" ,*((u8 *)(bar->src) + i) );
		  break;
		  
		case AS_DEC:
		  if( try_warning("illegal size for dec" , (bar->src_length <= 4) ) )
			return 0;

		  ret = 0;
		  for(i=0;i<bar->src_length;i++)
			{
			  ret <<= 8;
			  ret += *((u8 *)(bar->src) + i);
			}
		  sprintf( bar->str ,"%d" , ret );
		  break;

		case AS_ASC:
		  for(i=0;i< bar->src_length ;i++)
			{
			  ret = *((u8 *)(bar->src) + i);
			  sprintf( bar->str + i ,"%c" ,ret);
			}
		  break;

		case AS_SUBST:
		  ret = *(s32 *)(bar->src);
		  if ( ret >= bar->src_length ) return 0;
		  sprintf( bar->str ,"%s" ,*(bar->src_sub + ret) );
		  break;

		default: 
		  break;
		}
	}
  x = bar->x + bar->base.x;
  y = bar->y + bar->base.y;

  if( !bar->clipping || EXIST_IN(x,y,bar->clip) )
	{
	  if( *(bar->str) & 0x80 )
		video_setJStr(bar->str , x , y , bar->color );
	  else if( *(bar->str) == '#' )
		video_setString( bar->str + 1 , x , y , bar->color );  
	  else
		video_setJStr(bar->str , x , y , bar->color );
	}
  return -1;
}

s32
sb_clip(s32 val ,s32 from ,s32 to)
{
  s32 ret;
  ret = val;
  if( val < from ) ret = to;
  else if( val > to ) ret = from;
  return ret;
}

u16
sb_jpad( u16 trig )
{
  s32 i;
  u16 mask;
  mask=1;
  for(i=0;i<16;i++)
	{
	  mask = 1<<i; 
	  if( trig & mask ) return mask;
	}
  return 0;
}

s32
sb_length(u8 *s)
{
  s32 i=0;
  
  if(s == 0) return -1;
  while( *(s+i) != 0 ) 	++i;
  return i;
}
