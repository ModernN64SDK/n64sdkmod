 /*---------------------------------------------------------------------
  File : textlib.h

  Coded     by 
  Modified  by Koji Mitsunari.      Feb 27, 1997.
  Copyright by Nintendo, Co., Ltd.          1997.
  ---------------------------------------------------------------------*/
#ifndef _textlib_h_
#define _textlib_h_

#include	<ultra64.h>


extern u16 color;
extern u16 bg_color;

#define setcolor(r, g, b)	(color = GPACK_RGBA5551(r,g,b,1))
#define setbgcolor(r, g, b)	(bg_color = GPACK_RGBA5551(r,g,b,1))

#define SCREEN_LOW  1
#define SCREEN_HIGH 2
#define RESOLUTION SCREEN_HIGH
#define PATTERN_SIZE_X 8
#define PATTERN_SIZE_Y 16
#define SCREEN_SIZE_X 320*RESOLUTION
#define SCREEN_SIZE_Y 240*RESOLUTION
#define SCREEN_SIZE_VY 240*2*RESOLUTION
#define NUM_OF_X SCREEN_SIZE_X/PATTERN_SIZE_X
#define NUM_OF_Y SCREEN_SIZE_Y/PATTERN_SIZE_Y
#define NUM_OF_VY SCREEN_SIZE_VY/PATTERN_SIZE_Y

#define NUM_OF_CHAR 0x5f

#ifdef __MWERKS__
extern  u8      CR25[];
extern  u8      CR21[];
#else
#define	CR25	"\f\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n"
#define CR21	"\f\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\t"
#endif

extern void draw_char(int code);
extern void draw_uint( unsigned long num );
extern void draw_ushort( unsigned short num );
extern void draw_hex(int code);
extern void draw_dec(int code);
extern void init_draw(void);
extern void clear_draw(void);
extern void draw_puts(char *s);
extern void v_sync_double_buffer(OSMesgQueue *queue);
extern u8 *n64c2asc(u8 *, int);
extern u8 *uint2str(u32);
extern void mes_init(u8 *);
extern void mes_add(u8 *);

#endif

