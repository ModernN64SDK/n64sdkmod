
/*---------------------------------------------------------------------*
        Copyright (C) 1998 Nintendo.
        
        $RCSfile: dd_rtc.c,v $
        $Revision: 1.1 $
        $Date: 1999/07/05 02:20:13 $
 *---------------------------------------------------------------------*/

/*
   This is the time set screen used when an RTC error occurs during the DD 
   initialization sequence.
   For games that do not use the RTC, this module can be omitted by  
   using the -DNO_USE_RTC option when compiling dd.c.
   Although this sample does not use the RTC, support for this error has 
   been prepared since the sample is meant to be a sample of DD error handling
   sequences.
 */

#include "hmenu.h"
#include "herr.h"

/* declarerations */

#define ITEM_H        16 
#define TIME_NUM      6
#define LABEL_NUM     15
#define BTILE_NUM     7

static s32 _selected  = 0;
static TStringBar  _time[TIME_NUM];
static TStringBar  _label[LABEL_NUM];
static TBackTile   _btile[BTILE_NUM];

static LEODiskTime _rtc;
static s32 _noreadmode=1, _editmode = 0;

static s32 BTILE_WIDTH_1=100;


static TMessage4dd _ddmsg = {0};

/* functions */

static void _print(void);
static s32  _init(void); 
static s32	_keyProc(void);
static s32  _get( s32 *stepp );
static s32  _set( s32 *stepp );

/* implementation */

static void
_print(void)
{
  s32 i;

  if( _editmode )
	{
	  FORI(TIME_NUM) _label[i+1].color = RGBA_GREY;
	  FORI(TIME_NUM) _time[i].color	  = RGBA_GREY;
	  _label[_selected+1].color = RGBA_WHITE;
	  _time[_selected].color = RGBA_WHITE;
	}
  else
	{
	  FORI(TIME_NUM) _label[i+1].color = RGBA_WHITE;
	  FORI(TIME_NUM) _time[i].color	  = RGBA_WHITE; 
	}

  sprintf( _time[0].str ,"%02x%02x" ,_rtc.yearhi , _rtc.yearlo );
  FORI(5) sprintf( _time[i+1].str ,"%02x" ,*(((u8 *)&_rtc) + i + 3) );

  _btile[2].x = _time[ _selected ].x + BD_X;
  _btile[2].y = _time[ _selected ].y + BD_Y;
  _btile[2].w = sbar_getWidth(&_time[ _selected ]) + 10;
  
  FORI(BTILE_NUM)   btile_show(&_btile[i]);
  FORI(TIME_NUM) 	sbar_show(&_time[i]);
  FORI(LABEL_NUM)   sbar_show(&_label[i]);
}

static s32 _cmd=0;
#define MODE_IDLE  	 0
#define MODE_GETRTC	 1
#define MODE_SETRTC  2

static s32
_keyProc(void)
{
  s32 ret,dire;
  static s32 _first = 1,i,j;
  static u8  _keep;
  s32 min[]={00,1,1,0,0,0};
  s32 max[]={99,12,31,23,59,59};
  s32 daymax[]={31,28,31,30,31,30,31,31,30,31,30,31};

  if(_first)
	{
	  --_first;
	  _init();
	  _cmd = MODE_GETRTC;
	}

  ret = 0;

#define TARGET_RTCVAL *(((u8 *)&_rtc) + _selected + 2)
#define BCD2DEC(val)  (( (val>>4) & 0xf ) * 10 + ( val & 0xf ))
#define DEC2BCD(val)  ( u8 )(( val / 10 ) * 16 + ( val % 10 ))

  if( _editmode )
	{
	  switch(_trig)
		{
		case D_JPAD:
		case L_JPAD:  dire = -1;
					  goto edit;
		case U_JPAD:
		case R_JPAD:  dire = 1;
					  goto edit;
		case A_BUTTON:
					  _editmode = 0;
					  break;
		case B_BUTTON:
					  TARGET_RTCVAL = _keep;
					  _editmode = 0;
					  break;
		default:
					  break;			  
		edit:	
					  ret = (s32)(TARGET_RTCVAL); 
					  ret = ( _selected == 2 ) ? ( BCD2DEC(ret) + dire ) :
						sb_clip( BCD2DEC(ret) + dire ,min[_selected] ,max[_selected]);
					  if( _selected == 0 )
						{
						  _rtc.yearhi = ( ret > 95 ) ? DEC2BCD(19) : DEC2BCD(20); 
						}
					  TARGET_RTCVAL = DEC2BCD(ret);
						  
					  /* day */
					  if( _selected < 3 )
						{
						  ret = BCD2DEC(_rtc.day);
						  i = BCD2DEC(_rtc.month);
						  j = ( i==2 && !( BCD2DEC(_rtc.yearlo) % 4 )) ?  /* Leap year. */
							29 : daymax[i - 1];
						  if (_selected == 2 )
							ret = sb_clip(ret,1,j);							
						  else
							ret = (ret > j) ? j : ret;
  
						  _rtc.day = DEC2BCD( ret );	  
						}
					  break;
		}
	}
  else
	{
	  switch(_trig)
		{
		case U_JPAD: --_selected;  break;
		case D_JPAD: ++_selected;  break;
		case START_BUTTON:
				 _cmd = MODE_SETRTC;
				 break;
		case B_BUTTON:
		  if(_noreadmode)break;
				 _first =1;
				 return -1; 			 
		case A_BUTTON:
			_keep = TARGET_RTCVAL;
			_editmode = true;		 
			break;		 
		}
	  
	  _selected  = sb_clip(_selected ,0 , TIME_NUM-1);
	}
  return 0;
}

void
scr_rtc(void)
{
  s32 ret;
  static s32 _step = 0;

  _print();

  /*  Key input */
  if( ret = _keyProc() )
	{
	  switch(ret)
		{
		case -1:  
		  main_setScreen(SCREEN_TOPGUN);
		  _step = 0;
		case -2:  
			  return;
		default:  break;
		}
	}

  if( dd_checkGate(&_ddmsg) )
	{
	  switch(_cmd)
		{
		case MODE_GETRTC:
		  ret = _get(&_step);
		  break;
		case MODE_SETRTC:
		  ret = _set(&_step);
		  break;
		default:
		  ret = 0;
		}

	  if(ret)
		{
		  _cmd = MODE_IDLE; 
		  _step = 0;
		}

	}
}  

static s32
_get( s32 *stepp )
{
  s32 step = *stepp,ret = 0;
 
  if(_noreadmode) return -1;

  switch (step)
	{
	case 0:
	  _ddmsg.command  = LEOCMD_GETRTC;
	  dd_exec( &_ddmsg );
	  ++step;
	  break;

	case 1:
	  if( _ddmsg.retVal )
		bcopy( &_ddmsg.param ,&_rtc , sizeof(LEODiskTime));
 	  ret = -1;
	  ++step;
	  break;

	default:
	  break;

	}
  *stepp = step;

  return ret;
}

static s32
_set( s32 *stepp )
{
  s32 step = *stepp,ret = 0;
 
  switch (step)
	{
	case 0:
	  _ddmsg.command  = LEOCMD_SETRTC;
	  bcopy( &_rtc ,&_ddmsg.param , sizeof(LEODiskTime));
	  dd_exec( &_ddmsg );
	  ++step;
	  break;

	case 1:
	  if( _ddmsg.retVal ) ret = -1;
	  if(_noreadmode)
		{
		  _noreadmode = 0;
		  main_setScreen(SCREEN_MENU);
		}
	  ++step;
	  break;

	default:
	  break;

	}
  *stepp = step;

  return ret;
}

void 
rtc_noReadMode(void)
{
  _noreadmode = 1;
}

static s32
_init(void)
{
  s32 xpos,ypos;
  
  xpos = 20;
  ypos = 20;

  sbar_create(&_label[0],"Time setting" ,30,20);
  sbar_moveCenter(&_label[0],0,SCREEN_WD);
  btile_create(&_btile[0] , _label[0].x + BD_X ,_label[0].y + BD_Y
			   ,sbar_getWidth(&_label[0]) + 15 , 16 ,RGBA_BLUEALPHA );
  
  xpos  = 100; ypos = 60;
  sbar_create(&_time[0],"199X",(xpos + 8)   ,ypos);
  sbar_create(&_time[1],"12"  ,(xpos + 20)	,(ypos+= ITEM_H));
  sbar_create(&_time[2],"31"  ,(xpos + 20)	,(ypos+= ITEM_H));
  sbar_create(&_time[3],"23"  ,(xpos + 20)	,(ypos+= ITEM_H));
  sbar_create(&_time[4],"56"  ,(xpos + 20)	,(ypos+= ITEM_H));
  sbar_create(&_time[5],"47"  ,(xpos + 20)	,(ypos+= ITEM_H));

  
  sbar_create(&_label[7],"Set: START BUTTON" ,50 ,(ypos+= ITEM_H * 2) );

  xpos = 100 + 16 * 4;
  ypos = 60;
  sbar_create(&_label[1],"Year" ,xpos,ypos );
  sbar_create(&_label[2],"Month" ,xpos,(ypos+= ITEM_H) );
  sbar_create(&_label[3],"Day" ,xpos,(ypos+= ITEM_H) );
  sbar_create(&_label[4],"Hour" ,xpos,(ypos+= ITEM_H) );
  sbar_create(&_label[5],"Minutes" ,xpos,(ypos+= ITEM_H) );
  sbar_create(&_label[6],"Seconds" ,xpos,(ypos+= ITEM_H) );

  btile_create(&_btile[1] ,_time[0].x + BD_X  ,_time[0].y + BD_Y
			   ,BTILE_WIDTH_1 , ITEM_H * TIME_NUM ,RGBA_BLUEALPHA );

  btile_create(&_btile[2] ,_label[2].x + BD_X  ,_label[1].y + BD_Y
			   ,BTILE_WIDTH_1 , ITEM_H ,RGBA_SKYBLUEALPHA );

  btile_create(&_btile[3] ,_label[7].x + BD_X  ,_label[7].y + BD_Y
			   ,sbar_getWidth(&_label[7]) + 48 , ITEM_H ,RGBA_BLUEALPHA );

  if(_noreadmode)
	{
	  _rtc.pad    = 0;
	  _rtc.yearhi = 0x19;
	  _rtc.yearlo = 0x99;
	  _rtc.month  = 0x12;	  
	  _rtc.day    = 0x28;	  
	  _rtc.hour   = 0x23;
	  _rtc.minute = 0x59;
	  _rtc.second = 0x59;	  
	}

  _ddmsg.retQ = &myQ;
  return -1;
}

