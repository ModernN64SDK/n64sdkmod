/*---------------------------------------------------------------------*
        Copyright (C) 1998 Nintendo.
        
        $RCSfile: test_64dd.c,v $
        $Revision: 1.5 $
        $Date: 1998/12/18 01:10:21 $
 *---------------------------------------------------------------------*/

#include "hscr.h"

static s32 _screen = SCREEN_INIT;
static s32 _cursor = 1;

static u8 temp[32];
LEODiskTime     RTCdata;

static u8       *func_str[] = {
  "LeoCreateLeoManager()",
  "LeoReadWrite()(WRITE)",
  "LeoReadWrite()(READ)",
  "LeoSeek()",
  "LeoRezero()",
  "LeoReadDiskID()",
  "LeoReadRTC()",
  "LeoSetRTC()",
  "LeoModeSelectAsync()",
  "LeoSpdlMotor()(SLEEP)",
  "LeoSpdlMotor()(BRAKE)",
  "LeoSpdlMotor()(ACTIVE)",
  "LeoTestUnitReady()",
};

OSMesgQueue             myQ;
OSMesg                  myQBuf[1];

TMessage4dd _ddmsg = {0};
static s32 rtc_check(u16 year ,u8 month ,u8 day);

s32
sb_clip(s32 val ,s32 from ,s32 to)
{
  s32 ret;
  ret = val;
  if( val < from ) ret = to;
  else if( val > to ) ret = from;
  return ret;
}

u8
fromBCD(u8 data)
{
  return ((data >> 4) & 15) * 10 + (data & 15);
}

u8
toBCD(u8 data)
{
  return ((u8)(data / 10)) * 16 + (data % 10);
}

static s32 
rtc_check(u16 year ,u8 month ,u8 day)
{
  s32 daymax[]={31,28,31,30,31,30,31,31,30,31,30,31};

  if((year % 4) == 0) daymax[2-1]=29;
    
  return ( day <= daymax[month-1]);
}

void
printDate(Gfx **gpp)
{
  Gfx       *gp = *gpp;

  sprintf(temp, "%02d%02d/%02d/%02d %02d:%02d:%02d",
		  fromBCD(RTCdata.yearhi),
		  fromBCD(RTCdata.yearlo),
		  fromBCD(RTCdata.month),
		  fromBCD(RTCdata.day),
		  fromBCD(RTCdata.hour),
		  fromBCD(RTCdata.minute),
		  fromBCD(RTCdata.second) );
  _setString(&gp, temp, MES_DATE_X + 0, MES_DATE_Y,RGBA_WHITE);

  *gpp = gp;
}

void
_setString(Gfx **gpp,u8 * str,s32 x,s32 y,u32 color)
{
 Gfx *gp = *gpp;

 font_set_color(  (u8)((color >> 24 ) & 0xFF) 
				, (u8)((color >> 16 ) & 0xFF)
				, (u8)((color >> 8  ) & 0xFF)
				, (u8)((color >> 0  ) & 0xFF) );
 font_set_pos(x, y);
 font_show_string( &gp, str );

  *gpp = gp;
}

void
_setJStr(Gfx **gpp,s32 str_no,s32 x,s32 y,u32 color)
{
 Gfx *gp = *gpp;

 font_set_color(  (u8)((color >> 24 ) & 0xFF) 
				, (u8)((color >> 16 ) & 0xFF)
				, (u8)((color >> 8  ) & 0xFF)
				, (u8)((color >> 0  ) & 0xFF) );
 font_set_pos(x, y);
 jfont_show_string( &gp, str_no );

  *gpp = gp;
}

void
_setScreen(s32 i)
{
  _screen = i;
}

s32
_getCurrentScreen(void)
{
  return _screen;
}

void
test_64dd( Gfx **pglistp )
{
  Gfx       *gp;

  gp = *pglistp;
  
  font_init( &gp );

  switch( _screen )
	{
	case SCREEN_INIT:
	  scr_init(&gp);
	  _printErrorNo(&gp);
	  break;

	case SCREEN_MENU:
	  scr_menu(&gp);
	  break;

	case SCREEN_RESULT:
	  scr_result(&gp);
	  _printErrorNo(&gp);
	  break;

	case SCREEN_SETRTC:
	  scr_setrtc(&gp);
	  break;

	case SCREEN_SETMOTOR:
	  scr_setmotor(&gp);
	  break;

	default:
	  break;
	}

  /* Display Error No */

  
  scr_error(&gp);

  font_finish( &gp );
  *pglistp = gp;
}

void
_printErrorNo(Gfx **gpp)
{
  Gfx *gp=*gpp;
  s32 i;

  sprintf(temp, "%02d", dd_errbuf[0] );
  _setString(&gp, temp, 32 + 0*28, 200, RGBA_WHITE );

  for (i = 1 ; i < 5 ; i ++)
	{
	  if( !dd_errbuf[i] ) break; 
	  sprintf(temp, "%02d", dd_errbuf[i] );
	  _setString(&gp, temp, 32 + i*28, 200, RGBA_WHITE );
/*
	  if ( dd_errbuf[i] == LEO_ERROR_POWERONRESET_DEVICERESET_OCCURED )
		{
		  sprintf(temp, "%02d", dd_getStatus()->counter );
		  _setString(&gp, temp, 32 + i*28, 180, RGBA_WHITE );
		}
*/
	}

  *gpp = gp;
}

void
scr_result(Gfx **gpp)
{
  Gfx *gp = *gpp;
  s32 cur;

  cur = _getCursor();
  _setString( &gp, func_str[cur], MES_CMD_X, MES_CMD_Y, RGBA_WHITE );

  if( dd_checkGate(&_ddmsg) )
    {
	  if( A_BUTTON_PUSH )
		{
		  dd_errbuf[1] = 0;		  
		  _setScreen(SCREEN_MENU);
		}

	  if( _ddmsg.command == LEOCMD_GETRTC )
		{
		  bcopy( &_ddmsg.param ,&RTCdata , sizeof(LEODiskTime));
		  printDate(&gp);
		}
	  _setJStr(&gp, ES_SEIJO_SHURYO, 120,120 ,RGBA_WHITE);
	}

  *gpp = gp;
}

void
scr_init(Gfx **gpp)
{
  Gfx *gp = *gpp;
  static s32 _first = 1;
  static s32 _step=0;

  if(_first)
	{
	  osCreateMesgQueue(&myQ, myQBuf, 1);
      _ddmsg.retQ = &myQ;

/*  dd_errHook(9,5,1);*/

	  dd_create();
      _first--;

	}
	
  _setString(&gp,func_str[0],MES_CMD_X,MES_CMD_Y,RGBA_WHITE);

  if( dd_checkGate(&_ddmsg) )
    {
	  switch(_step)
		{
		case 0:
		  if( (dd_getStatus()->errmes & 0xFF) == ERRMES__SET_CLOCK )
			{
			  if( A_BUTTON_PUSH )
				{
				  _setScreen( SCREEN_SETRTC );
				  return;
				}
			  break;
			}
		  if( dd_getStatus()->mode == DDSTAT_GOOD ) ++_step;
		  break;

		case 1: /* read rtc */
		  _ddmsg.command  = LEOCMD_GETRTC;
		  dd_exec( &_ddmsg );
		  ++_step;
		  break;
  
		case 2: /* first write */
		  if( _ddmsg.retVal )
			{
			  bcopy( &_ddmsg.param ,&RTCdata , sizeof(LEODiskTime));
			  _ddmsg.command  = LEOCMD_LBAWRITE;
			  _ddmsg.param.hi = TEST_START_LBA;
			  _ddmsg.param.lo = 1;
			  _ddmsg.param.rw = NULL;
			  dd_exec( &_ddmsg );
			  ++_step;
			}
		  break;

		case 3:
		  if ( A_BUTTON_PUSH )
			{
			  _setScreen( SCREEN_MENU );
			  return;
			}
		  printDate(&gp);
		  _setJStr(&gp, ES_SEIJO_SHURYO, 120,120 ,RGBA_WHITE);
		  break;

		default:
		  break;
		}
	}

  *gpp = gp;
}


void
scr_menu(Gfx **gpp)
{
  Gfx *gp = *gpp;
  s32 i;	

  font_set_transparent( 1 );
  font_set_win( 32, 1 );
  
  for ( i = 1 ; i < FUNC_NUM ; i++)
	_setString(&gp, func_str[i],MES_CMD_X, MES_CMD_Y + MES_CMD_DY*(i-1),
			   ((i == _cursor)  ? RGBA_SELECTED : RGBA_UNSELECTED));


  if( dd_checkGate(&_ddmsg) )
    {
	  if( A_BUTTON_PUSH )
		{
		  switch ( _cursor ) 
			{
			case 1:
			  _ddmsg.command  = LEOCMD_LBAWRITE;
			  _ddmsg.param.hi = TEST_START_LBA;
			  _ddmsg.param.lo = 1;  
			  _ddmsg.param.rw = NULL;  
			  dd_exec(&_ddmsg);
			  _setScreen( SCREEN_RESULT );		
			  break;

			case 2:
			  _ddmsg.command  = LEOCMD_LBAREAD;
			  _ddmsg.param.hi = TEST_START_LBA;
			  _ddmsg.param.lo = 1;  
			  _ddmsg.param.rw = NULL;  
			  dd_exec(&_ddmsg);
			  _setScreen( SCREEN_RESULT );		
			  break;
			  
			case 3:
			  _ddmsg.command  = LEOCMD_SEEK;
			  _ddmsg.param.hi = TEST_START_LBA;
			  dd_exec(&_ddmsg);
			  _setScreen( SCREEN_RESULT );		
			  break;

			case 4:
			  _ddmsg.command  = LEOCMD_REZERO;
			  dd_exec(&_ddmsg);
			  _setScreen( SCREEN_RESULT );		
			  break;

			case 5:
			  _ddmsg.command  = LEOCMD_GETID;
			  _ddmsg.param.hi = 0; /* ID is no required  */
			  _ddmsg.param.lo = 0; /* boot ID required   */
			  dd_exec(&_ddmsg);
			  _setScreen( SCREEN_RESULT );		
			  break;

			case 6:
			  _ddmsg.command  = LEOCMD_GETRTC;
			  dd_exec(&_ddmsg);
			  _setScreen( SCREEN_RESULT );		
			  break;

			case 7:
			  _setScreen( SCREEN_SETRTC );
			  break;

			case 8:
			  _setScreen( SCREEN_SETMOTOR );
			  break;

			case 9:
			  _ddmsg.command  = LEOCMD_SLEEP;
			  dd_exec(&_ddmsg);
			  _setScreen( SCREEN_RESULT );		
			  break;

			case 10:
			  _ddmsg.command  = LEOCMD_BRAKE;
			  dd_exec(&_ddmsg);
			  _setScreen( SCREEN_RESULT );		
			  break;

			case 11:
			  _ddmsg.command  = LEOCMD_ACTIVE;
			  dd_exec(&_ddmsg);
			  _setScreen( SCREEN_RESULT );		
			  break;
			case 12:
			  _ddmsg.command  = LEOCMD_TESTUNIT;
			  dd_exec(&_ddmsg);
			  _setScreen( SCREEN_RESULT );		
			  break;


			default:
			  break;

			}
		  return;
		}
		
	  switch ( conts[0].nowtrg ) 
		{
		case D_JPAD:
		  if (++_cursor >= FUNC_NUM) _cursor = 1;
		  break;
		case U_JPAD:
		  if (--_cursor < 1) _cursor =  sizeof(func_str)/sizeof(u8 *) - 1;
		  break;
		}
	}

  *gpp = gp;
}

s32
_getCursor(void)
{
  return _cursor;
}

#define U_JPAD_PUSH ((conts[0].nowtrg | conts[0].repeat) & U_JPAD)
#define D_JPAD_PUSH ((conts[0].nowtrg | conts[0].repeat) & D_JPAD)

void
scr_setrtc(Gfx **gpp)
{
  Gfx *gp = *gpp;
  static s32 i;
  static s16 xpos;
  static s32 _first = 1;
  s32 dire=0,next_screen;
  static u16 year = 1999;
  static u8 month = 7;
  static u8 day = 7;
  static u8 hour = 23;
  static u8 minute = 59;
  static u8 second = 59;

  if(_first)
	{
	  xpos = 0;
	  i = MES_DATE_X + 2*8;
	  if(dd_getStatus()->errmes != ERRMES__SET_CLOCK)
		{
		  year   = fromBCD(RTCdata.yearhi)*100 + fromBCD(RTCdata.yearlo);
		  month  = fromBCD(RTCdata.month);
		  day    = fromBCD(RTCdata.day);
		  hour   = fromBCD(RTCdata.hour);
		  minute = fromBCD(RTCdata.minute);
		  second = fromBCD(RTCdata.second);
		}
	  --_first;
	}

  _setString(&gp,func_str[7],MES_CMD_X,MES_CMD_Y,RGBA_WHITE);

  sprintf(temp, "%04d/%02d/%02d %02d:%02d:%02d",
		  year, month, day,
		  hour, minute, second);

  _setString(&gp,temp,MES_DATE_X + 0, MES_DATE_Y,RGBA_WHITE);
  _setString(&gp,"--",i, MES_DATE_Y+8 ,RGBA_YELLOW);

  if( dd_checkGate(&_ddmsg) )
    {
	  switch ( conts[0].nowtrg )
		{
		case A_BUTTON:
		  if (rtc_check(year,month,day))
			{
			  RTCdata.yearhi = toBCD( (u8)(year / 100) );
			  RTCdata.yearlo = toBCD( (u8)(year % 100) );
			  RTCdata.month  = toBCD(month);
			  RTCdata.day    = toBCD(day);
			  RTCdata.hour   = toBCD(hour);
			  RTCdata.minute = toBCD(minute);
			  RTCdata.second = toBCD(second);
			  _ddmsg.command = LEOCMD_SETRTC;
			  bcopy( &RTCdata ,&_ddmsg.param , sizeof(LEODiskTime));

			  next_screen = ( (dd_getStatus()->errmes & 0xff) == ERRMES__SET_CLOCK ) ?
				SCREEN_INIT : SCREEN_RESULT;
			  dd_exec( &_ddmsg );

			  _setScreen(next_screen);
			}
		  break;

		case B_BUTTON:
		  if(  (dd_getStatus()->errmes & 0xff) != ERRMES__SET_CLOCK )
			{
			  _setScreen(SCREEN_MENU);
			  ++_first;
			}
		  break;

		case R_JPAD:
		  if ( xpos < 5 ) xpos ++;
		  break;

		case L_JPAD:
		  if ( xpos > 0 ) xpos --;
		  break;

		}

	  if(U_JPAD_PUSH) dire = 1;
	  if(D_JPAD_PUSH) dire = -1;
		
	  switch (xpos)
		{
		case 0:
		  i = MES_DATE_X + 2*8;
		  year = sb_clip(year+dire,1996,2095); 
		  break;
		  
		case 1:
		  i = MES_DATE_X + 5*8;
		  month = sb_clip(month+dire,1,12); 
		  break;
		  
		case 2:
		  i = MES_DATE_X + 8*8;
		  day = sb_clip(day + dire,1,31); 
		  break;    
		  
		case 3:
		  i = MES_DATE_X + 11*8;
		  hour = sb_clip( hour + dire, 0, 23 ); 
		  break;    
		  
		case 4:
		  i = MES_DATE_X + 14*8;
		  minute = sb_clip( minute + dire, 0, 59 ); 
		  break;
		  
		case 5:
		  i = MES_DATE_X + 17*8;
		  second = sb_clip( second + dire, 0, 59 ); 
		  break;    
		}
	}

  *gpp = gp;
}

#define U_JPAD_PUSH ((conts[0].nowtrg | conts[0].repeat) & U_JPAD)
#define D_JPAD_PUSH ((conts[0].nowtrg | conts[0].repeat) & D_JPAD)

void
scr_setmotor(Gfx **gpp)
{
  Gfx *gp = *gpp;
  static s16 _first=1,standby, sleep ,xpos;
  s32 dire;
  u32 color[2]={RGBA_WHITE};

  if(_first)
	{
	  standby = 3;
	  sleep = 1;
	  xpos = 0;
	  --_first;
	}

  _setString(&gp,func_str[8],MES_CMD_X,MES_CMD_Y,RGBA_WHITE);

  color[xpos] = RGBA_YELLOW;
  color[xpos^1] = RGBA_WHITE;
 
  sprintf(temp, "%2d", standby);
  _setString(&gp, "To STANDBY", MES_CMD_X + 16, MES_CMD_Y + 52, color[0]);
  _setString(&gp, temp, MES_CMD_X + 36, MES_CMD_Y + 72, color[0]);

  sprintf(temp, "%2d", sleep);
  _setString(&gp, "To SLEEP",  MES_CMD_X + 128, MES_CMD_Y + 52, color[1]);
  _setString(&gp, temp, MES_CMD_X + 144, MES_CMD_Y + 72, color[1]);

  if( dd_checkGate(&_ddmsg) )
    {
	  switch ( conts[0].nowtrg )
		{
		case A_BUTTON:
		  _ddmsg.command = LEOCMD_SETMOTORTIME;
		  _ddmsg.param.hi = standby;
		  _ddmsg.param.lo = sleep;

		  dd_exec(&_ddmsg);
		  _setScreen( SCREEN_RESULT );
		  break;
		  
		case B_BUTTON:
		  _setScreen( SCREEN_MENU );
		  break;
		  
		case U_JPAD:
		  dire = 1;
		  goto UPDOWN;
		  
		case D_JPAD:
		  dire = -1;
		  goto UPDOWN;
		  
		case R_JPAD:
		case L_JPAD:
		  xpos ^= 1;
		  break;
		  
			default:
			  break;

			UPDOWN:
              if (xpos == 0) 
                  standby = sb_clip(standby+dire,0,10);
              else
                  sleep = sb_clip(sleep + dire,0,10);
              break;
            }
	}

  *gpp = gp;
}
