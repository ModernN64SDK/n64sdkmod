
/*---------------------------------------------------------------------*
        Copyright (C) 1998 Nintendo.
        
        $RCSfile: dd_summon.c,v $
        $Revision: 1.1 $
        $Date: 1999/07/05 02:20:13 $
 *---------------------------------------------------------------------*/

#include "hmenu.h"
#include "herr.h"

/* declarerations */


#define ITEM_H        16 
#define BTILE_NUM     3
#define LABEL_NUM     12
#define MES_NUM       7

static TStringBar  _label[LABEL_NUM];
static TStringBar  _mes[MES_NUM];
static TBackTile   _btile[BTILE_NUM];

static s32 _step = 0;

static TMessage4dd _ddmsg = {0};

/* functions */

static void _print(void);
static s32  _init(void); 

/* implementation */

static void
_print(void)
{
  s32 i,j,mes_num=0,max_width=0;

  FORI(MES_NUM) _mes[i].visible = false;
  _btile[0].visible = false;

  switch(_step)
	{
	case 2:
	case 3:
	case 4:
	  sprintf( _mes[mes_num++].str ,"Loading the") ;
	  sprintf( _mes[mes_num++].str ,"activated game......") ;
	  break;
	}

  if(mes_num) _btile[0].visible = true;

  /*  Adjust the display position */
  FORI(mes_num)
	{
	  j = sbar_getWidth(&_mes[i]);
	  max_width = (j>max_width) ? j : max_width;
	  _mes[i].visible = true;
	}
  _btile[0].w = max_width + 10;

  j = ( SCREEN_WD - max_width ) / 2 ;
  FORI(mes_num) _mes[i].x = j;
  _btile[0].x = j - 5;
  _btile[0].h = mes_num * 16;
  
  /* Display */
  FORI(BTILE_NUM) btile_show(&_btile[i]);
  FORI(MES_NUM) sbar_show(&_mes[i]);
  FORI(LABEL_NUM) sbar_show(&_label[i]);

}

void
scr_summon(void)
{
//  s32 cur;
  static s32 cnt=0,_first=1; 
  static LEODiskID	_id;

  if(_first)
	{
	  --_first;
	  _init();
	  _step = 0;
	}

  _print();

  if( dd_checkGate(&_ddmsg) )
	{
	  switch(_step)
		{
		case 0: /* Checks the disk */
		  _ddmsg.command  = LEOCMD_GETID;
		  _ddmsg.param.hi = (s32)&_id;
		  _ddmsg.param.lo = 0; 
		  dd_exec( &_ddmsg );
		  ++_step;
		  break;

		case 1: /*  Get the specified area size */
		  if( _ddmsg.retVal )
			{
			  if(!bcmp(_id.gameName,"REBT",4))
				{
				  _ddmsg.command  = LEOCMD_LBAREAD;
				  _ddmsg.param.hi = 49;
				  _ddmsg.param.lo = 1; 
				  dd_exec( &_ddmsg );
				  ++_step;
				}
			  else
				err_set(1);
			  break;
			}
		  _step = 0;
		  break;

		case 2: /*  Load the activated game */
		  if( _ddmsg.retVal )
			{
//			  cur  = dd_getStatus()->current_buffer ^ 1;
			  _ddmsg.command  = LEOCMD_LBAREAD;
			  _ddmsg.param.hi = *((s32 *)_ddmsg.param.rw);
			  _ddmsg.param.lo = *((s32 *)_ddmsg.param.rw + 1);
			  _ddmsg.param.rw = (u32)0x80400000;
//			  _rwbuf[0] = (u8 *)0x80400000;
//			  dd_getStatus()->current_buffer = 0;

			  dd_exec( &_ddmsg );
			  ++_step;
			}
		  else
			_step = 0;
		  break;

		case 3: 
		  if( _ddmsg.retVal )
			  ++_step;
		  else
			_step = 0;
		  break;
		  
		case 4: 
		  if ( ++cnt < 0 * 180 )  return;
		  osViBlack(1);
		  cnt = 0;		  
		  ++_step;
		  break;

		case 5:
#ifdef DEBUG
		  osSyncPrintf("load end\n");
		  osAppNMIBuffer[5] = 0x12345678;
		  osSyncPrintf("buffer is 0x%08x\n", osAppNMIBuffer[5]);
#endif		  
		  LeoBootGame((void *)0x80400000);
		  break;

		default:
		  break;
		}
	}
}  

static s32
_init(void)
{
   s32 xpos,ypos,i;
  
  xpos = 20;
  ypos = 20;

  FORI(MES_NUM) sbar_create(&_mes[i],"*" ,45 ,50 + i * 16 );
  btile_create(&_btile[0] ,70 ,50 ,240 ,100 ,RGBA_BLUEALPHA );

  _ddmsg.retQ = &myQ;

  return -1;
}
