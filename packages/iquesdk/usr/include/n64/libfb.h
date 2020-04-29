/*====================================================================
  based on:
  File  : graph.h

  Created   by Koji Mitsunari.    Mar,14 1996.
  Copyright by Nintendo, Co., Ltd.       1996.
  =====================================================================*/
#ifndef	__libfb_h__
#define	__libfb_h__
#include <ultra64.h>

#define FB_LOW_RES 0
#define FB_HIGH_RES 1

extern int FB_SCREEN_WD;
extern int FB_SCREEN_HT;
extern u16 FB_BGCOLOR;

#define fbWhite		0xffff
#define FB_WHITE	0xffff
#define fbBlack		0x0000
#define FB_BLACK	0x0000
#define fbGray		GPACK_RGBA5551(127,127,127,1)
#define fbBlue		GPACK_RGBA5551(0,0,255,1)
#define fbGreen		GPACK_RGBA5551(0,255,0,1)
#define fbRed		GPACK_RGBA5551(255,0,0,1)
#define fbYellow	GPACK_RGBA5551(255,255,0,1)
#define fbMagenta	GPACK_RGBA5551(255,0,255,1)
#define fbCyan		GPACK_RGBA5551(0,255,255,1)
#define fbDark		GPACK_RGBA5551(50,50,128,1)
#define fbMarkColor	GPACK_RGBA5551(50,250,50,1)

#define FB_CHAR_WD	8
#define FB_CHAR_HT	16

#define FB_TEXT_WD	(FB_SCREEN_WD/FB_CHAR_WD)
#define FB_TEXT_HT	(FB_SCREEN_HT/FB_CHAR_HT)

extern	void	fbInit(int res);
extern	void	fbPutChar(u16 color, int x, int y, char c);
extern	void	fbRevChar(u16 color, int x, int y, char c);
extern	void	fbPutHex(u16 color, int x, int y, int v, char width);
extern	void	fbPutDec(u16 color, int x, int y, int v);
extern	void	fbPrintStr(u16 color, int x, int y, const char* str);
extern	void	fbCircle(int x, int y, int r, u16 color);
extern	void	fbPixel(int x, int y, u16 color);
extern	void	fbLine(int x0, int y0, int x1, int y1, u16 color);
extern	void	fbBox(int x0, int y0, int x1, int y1, u16 color);
extern	void	fbRect(int x0, int y0, int x1, int y1, u16 color);
extern	void	fbClear(void);
extern	void	fbBigPutc(u16 color, int x, int y, char c, int xsize, int ysize);
extern	void	fbBigPrintStr(u16 color, int x, int y, const char*, int xsize, int ysize);
extern  void    fbSetBg(u16 bg);
extern	int	fbPrintf(u16 color, int curs_x, int curs_y, const char *fmt, ...);

extern  u8 fb_font[][FB_CHAR_HT];
#endif
