 /*---------------------------------------------------------------------
  File : textlib.c

  Coded     by 
  Modified  by Koji Mitsunari.      Feb 27, 1997.
  Copyright by Nintendo, Co., Ltd.          1997.
  ---------------------------------------------------------------------*/
#include "textlib.h"
#include "ncode.h"

extern u16      *bitmap_buf;
extern u16      bitmap_buf1[];
extern u16      bitmap_buf2[];
extern u8	patterns[NUM_OF_CHAR][PATTERN_SIZE_Y];

u32	x_pos = 0;
u32	y_pos = 0;
u16	color = 0;
u16	bg_color = 0;
u8	str[256];
u8	*err_mes_ptr;

#ifdef  __MWERKS__
u8      CR25[] = "\f\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
u8      CR21[] = "\f\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\t";
#endif  /* __MWERKS__ */

void v_sync_double_buffer(OSMesgQueue *queue)
{
  static int toggle=0;

  osWritebackDCache(bitmap_buf, sizeof(u16) * SCREEN_SIZE_X * SCREEN_SIZE_Y);

  if (MQ_IS_FULL(queue))
    (void)osRecvMesg(queue, NULL, OS_MESG_BLOCK);
  (void)osRecvMesg(queue, NULL, OS_MESG_BLOCK);

  osViSwapBuffer( bitmap_buf );

  toggle ^= 1;
  if( toggle == 1 ) {
    bitmap_buf = bitmap_buf2;
  } else {
    bitmap_buf = bitmap_buf1;
  }
}

void init_draw(void)
{
  int mode=RESOLUTION;
  int i;
  u16 *r;
  bitmap_buf = bitmap_buf1;
  if(mode == SCREEN_LOW) {
    osViSetMode(&osViModeTable[OS_VI_NTSC_LPF1]);
  } else {
    osViSetMode(&osViModeTable[OS_VI_NTSC_HPF1]);
  }
  
  osViSwapBuffer( bitmap_buf );

  r = bitmap_buf1;
  for (i = 0; i < SCREEN_SIZE_X * SCREEN_SIZE_Y; i ++) 
    *r++  = bg_color;
  r = bitmap_buf2;
  for (i = 0; i < SCREEN_SIZE_X * SCREEN_SIZE_Y; i ++)
    *r++  = bg_color;
}

void clear_draw(void)
{
  int i;
  u16 *r;
  r = bitmap_buf;
  for (i = 0; i < SCREEN_SIZE_X * SCREEN_SIZE_Y; i++, *r++  = bg_color);
}

void draw_char( int in_code )
{
  int i,j;
  int code;
  switch( in_code )  {
  case '\n':
    x_pos = 0;
    if( ++y_pos >= NUM_OF_Y ) {
      y_pos=0;
    }
    break;
  case '\r':
    x_pos = 0;
    break;
  case '\t':
    x_pos += 8;
    if( x_pos  >= NUM_OF_X ) {
      x_pos = 0;
      if( ++y_pos >= NUM_OF_Y ) {
	y_pos=0;
      }
    }
    break;
  case '\f':
    y_pos = 0;
    x_pos = 0;
    break;
  case NULL:
    x_pos ++;
    break;
  default:
    if( 0 > ( code = in_code - 0x20 )) {
      code = 0x1f;
    } else if( 0x5f < code) {
      code = 0x1f;
    }
    for(i = 0 ; i < PATTERN_SIZE_Y ; i++)
      for(j = 0 ; j < PATTERN_SIZE_X ; j++)
	bitmap_buf[ (y_pos * PATTERN_SIZE_Y + i) * SCREEN_SIZE_X +
		   (x_pos * PATTERN_SIZE_X + j) ]
		     = ( ((patterns[code][i]>>(7-j)) & 0x01)? color:bg_color);
    if( ++x_pos >= NUM_OF_X ) {
      x_pos = 0;
      if( ++y_pos >= NUM_OF_Y ) {
	y_pos=0;
      }
    }
    break;
  }
}  


void draw_uint( unsigned long num )
{
  int i;
  for(i = 0 ; i < 4; i++)
    draw_hex( num >> (8 * (3 - i)) );
}

void draw_ushort( unsigned short num )
{
  int i;
  for(i = 0 ; i < 2; i++)
    draw_hex( num >> (8 * (1 - i)) );
}

          
void draw_hex( int num )
{
  int	lo = num & 0x0f;
  int	hi = (num >> 4) & 0x0f;

  if(hi >= 10) {
    draw_char('a' + hi -10);
  } else {
    draw_char('0' + hi);
  }
  if(lo >= 10) {
    draw_char('a' + lo -10);
  } else {
    draw_char('0' + lo);
  }
}

#define	DEC_NUM	5
void draw_dec( int num )
{
  u8	s[DEC_NUM];
  int	i = num;
  int	j = DEC_NUM-2;

  for (j = DEC_NUM - 2 ; j >= 0 ; j-- ){ 
    s[j] = (i % 10) + '0';
    i =	(int)(i/10);
  }
  s[DEC_NUM-1] = '\0';

  draw_puts(s);
}

void draw_puts(char *s)
{
  while( *s )
    draw_char( *s++ );
}

u8 *n64c2asc(u8 *s, int length) {
  int	i;

  for (i = 0 ; i < length ; i ++ ) {
    if ( s[i] >= __0 && s[i] <= __9) {
      str[i] = s[i] + ('0' - __0);
    } else if ( s[i] >= __A && s[i] <= __Z) {
      str[i] = s[i] + ('A' - __A);
    } else if (s[i] == __NUL && i < length-1) {
      str[i] = ' ';
    } else {
      switch(s[i]) {
      case __NUL:
	str[i] = NULL;
	break;
      case __SPC:
	str[i] = ' ';
	break;
      case __EXCLAM:
	str[i] = '!';
	break;
      case __2QUOTE:
	str[i] = '\"';
	break;
      case __NUMBR:
	str[i] = '#';
	break;
      case __1QUOTE:
	str[i] = '\'';
	break;
      case __ASTER:
	str[i] = '*';
	break;
      case __PLUS:
	str[i] = '+';
	break;
      case __COMMA:
	str[i] = ',';
	break;
      case __MINUS:
	str[i] = '-';
	break;
      case __PERIOD:
	str[i] = '.';
	break;
      case __SLASH:
	str[i] = '/';
	break;
      case __COLON:
	str[i] = ':';
	break;
      case __EQUAL:
	str[i] = '=';
	break;
      case __QUEST:
	str[i] = '?';
	break;
      case __AT:
	str[i] = '@';
	break;
      default:
	str[i] = ' ';
      }
    }
  }    
  return str;
}

u8 *uint2str( u32 num )
{
  int	i;
  
  for(i = 0 ; i < 4; i++) {
    str[i] = num >> (8 * (3 - i));
  }
  return str;
}

void
mes_init(u8 *s1)
{
  u8 *s;
  
  err_mes_ptr = s1;
  s = CR21;
  while(*s) {
    *err_mes_ptr++ = *s++;
  }
  err_mes_ptr++;
}

void
mes_add(u8 *s)
{
  osSyncPrintf("%s",s);
  err_mes_ptr--;
  while(*s) {
    *err_mes_ptr++ = *s++;
  }
  *err_mes_ptr++ = '\0';
}
