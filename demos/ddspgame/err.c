
/*---------------------------------------------------------------------*
        Copyright (C) 1998 Nintendo.
        
        $RCSfile: err.c,v $
        $Revision: 1.5 $
        $Date: 1998/12/02 02:27:23 $
 *---------------------------------------------------------------------*/

#include "main.h"
#include "herr.h"
#include "hdd.h"
#include "font.h"
#include "hscr.h"

u8 err_str[60];
static s32 _err_show=0,_err_no=0;

static void _print(Gfx ** gpp);
static void _printNum(Gfx **gpp);
/* static s32 _init(void); */
static s32 _mes ;
static u8 temp[32];

void
_printNum(Gfx **gpp)
{
  Gfx *gp=*gpp;

  font_set_pos( 96, 64 );
  jfont_show_string( &gp, 3 );
  font_set_pos( 178, 64 );
  sprintf(temp, "%02d", dd_getStatus()->errno);
  font_show_string( &gp, temp );

  *gpp = gp;
}

void
_print(Gfx **gpp)
{
  u8		temp[32];
  Gfx *gp=*gpp;

  _mes = dd_getStatus()->errmes;
 
  font_set_transparent( 0 );
  font_set_color(255, 255, 255, 255 );
 
  if( dd_getStatus()->mode & ( DDSTAT_DEAD | DDSTAT_DIALOGUE) )
	{
	  switch( _mes & 0xFF) 
		{
/*  Required messages (areas 1 and 2)   */

		case ERRMES__SET_CLOCK:
		  if( _getCurrentScreen() != SCREEN_SETRTC)
			{
			  _printNum(&gp);
			  font_set_pos( 40, 88 );
			  jfont_show_string( &gp, ES_JIKOKU_WO_SETTEI );
			  font_set_pos( 40, 108 );
			  jfont_show_string( &gp, ES_KUWASIKU_HA_SETUMEISHO );
			  font_set_pos( 40, 128 );
			  jfont_show_string( &gp, ES_OYOMIKUDASAI );
			  font_set_pos( 40, 172 );
			  jfont_show_string( &gp, ES_A_BUTTON_WO_OSHITE );
			}
		  break;

		case ERRMES__RAMPACK_NOT_CONNECTED:
		  _printNum(&gp);
		  font_set_pos( 40, 88 );
		  jfont_show_string( &gp, ES_KAKCHO_RAM_PAK );
		  font_set_pos( 40, 108 );
		  jfont_show_string( &gp, ES_SASHIKONNDE_ARIMASUKA );
		  font_set_pos( 40, 128 );
		  jfont_show_string( &gp, ES_KUWASIKU_HA_SETUMEISHO );
		  font_set_pos( 40, 148 );
		  jfont_show_string( &gp, ES_OYOMIKUDASAI );
		  break;

		case ERRMES__TORISETSU_YOME:
		  _printNum(&gp);
		  font_set_pos( 40, 88 );
		  jfont_show_string( &gp, ES_TORIATUKAI_SETUMEISHO );
		  break;

		case ERRMES__NOW_IN_RESTORE:  
		  font_set_pos( 40, 108 );
		  jfont_show_string( &gp, ES_ZENKAI_DATA_GA);
		  font_set_pos( 40, 128 );
		  jfont_show_string( &gp, ES_SAVE_DEKIMASEN);
		  font_set_pos( 40, 148 );
		  jfont_show_string( &gp, ES_DATA_WO_SHOKIKA);
		  font_set_pos( 40, 172 );
		  jfont_show_string( &gp, ES_A_BUTTON_WO_OSHITE );
		  break;

		case ERRMES__ACCESSLAMP:
		  _printNum(&gp);
		  font_set_pos( 40, 88 );
		  jfont_show_string( &gp, ES_ACCESS_LAMP );
		  font_set_pos( 40, 108 );
		  jfont_show_string( &gp, ES_DISK_WO_NUKANAIDE );
		  font_set_pos( 40, 128 );
		  jfont_show_string( &gp, ES_KUWASIKU_HA_SETUMEISHO );
		  font_set_pos( 40, 148 );
		  jfont_show_string( &gp, ES_OYOMIKUDASAI );
		  break;

		case ERRMES__WRONG_DISK:
		  font_set_pos( 40, 88 );
		  jfont_show_string( &gp, ES_MACHIGATTA_DISK );
		  font_set_pos( 40, 108 );
		  jfont_show_string( &gp, ES_KANOSEI_GA_ARIMASU );
		  font_set_pos( 40, 128 );
		  jfont_show_string( &gp, ES_TADASHII_DISK_NI );
		  font_set_pos( 40, 148 );
		  jfont_show_string( &gp, ES_KUDASAI );
		  break;
 
		default:
		  break;
		}

/*  Required messages (area 3) */
	  switch( _mes & 0xFF00 ) 
		{
		case ERRMES__DISK_WO_NUKE:
		  font_set_pos( 40, 172 );
		  jfont_show_string( &gp, ES_DISK_WO_TORIDASHITE );
		  break;

		case ERRMES__DISK_WO_IRERO:
		  font_set_pos( 40, 172 );
		  jfont_show_string( &gp, ES_DISK_WO_SASHIKONDE );
		  break;

		case ERRMES__INSERT_AGAIN:
		  font_set_pos( 24, 172 );
		  jfont_show_string( &gp, ES_DISK_WO_SASHIKOMINAOSHITE );
		  break;

		}


	}
/* Application-specific messages (if any) */
  else if( dd_getStatus()->mode & DDSTAT_KILLED )
	{
	  switch( _mes & 0xFF) 
		{
		case ERRMES__DRIVE_NOT_FOUND:
		  font_set_pos( 80, 108 );
		  sprintf(temp, "%s", "64DD DRIVE NOT FOUND!");
		  font_show_string( &gp, temp );
		  break;
		}
	}
  else
	{
	  if( _err_show )
		{
		  switch( _err_no ) 
			{
			case 1:
			  break;
			}
		}
	}

  *gpp = gp;
}

void
err_set(s32 no)
{
  _err_show = 1;
  _err_no   = no;
}

void
scr_error(Gfx ** gpp)
{
  static s32 _first = 1;

  if(_first)
	{
	  --_first;
/*	  _init();
*/
	}

  _print(gpp);

  if( conts[0].nowtrg & A_BUTTON )
	{
	  switch ( _mes & 0xFF )
		{
		case ERRMES__NOW_IN_RESTORE:
		  dd_thaw(ERRMES__NOW_IN_RESTORE);
		  break;
		}
	}
	
}
