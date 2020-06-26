/*====================================================================
  File  : graph.h

  Created   by Koji Mitsunari.    Mar,14 1996.
  Copyright by Nintendo, Co., Ltd.       1996.
  =====================================================================*/
#ifndef	_graph_h_
#define	_graph_h_

#define	BMP_DATA	0

#define white		0xffff
#define WHITE		0xffff
#define black		0x0000
#define BLACK		0x0000
#define gray		GPACK_RGBA5551(127,127,127,1)
#define blue		GPACK_RGBA5551(0,0,255,1)
#define green		GPACK_RGBA5551(0,255,0,1)
#define red		GPACK_RGBA5551(255,0,0,1)
#define yellow		GPACK_RGBA5551(255,255,0,1)
#define dark		GPACK_RGBA5551(50,50,128,1)
#define markcolor	GPACK_RGBA5551(50,250,50,1)
#define BGCOLOR		0x0000

#define CHAR_WD		8
#define CHAR_HT		16

#define TEXT_WD		SCREEN_WD/CHAR_WD
#define TEXT_HT		SCREEN_HT/CHAR_HT

#define	FBUF_ADRS(x,y)		(FBUF_TOP+((x)+(y)*SCREEN_WD)*2)
#define	canvas_cursor(x,y)	__cursor = (u16 *)FBUF_ADRS((x),(y))
#define	canvas_center(x,y)	canvas_cursor((SCREEN_WD-(x)*FONT_WSKIP)/2,(y))
#define	canvas_wskip(x)		__cursor += (x);
#define	canvas_putstr(n,fg,bg)	canvas_puts((n), sizeof(n)-1, (fg), (bg))


extern	void	putchar(u16, int, int, char);
extern	void	revchar(u16, int, int, char);
extern	void	putint_h(u16, int, int, int, char);
extern	void	putint_d(u16, int, int, int);
extern	void	printstr(u16, int, int, char*);
extern	void	circle(int, int, int, u16);
extern	void	pset(int, int, u16);
extern	void	line(int, int, int, int, u16);
extern	void	box(int, int, int, int, u16);
extern	void	gcls(void);
extern	void	bigputc(u16, int, int, char, int, int);
extern	void	printbig(u16, int, int, char*, int, int);
extern	void	WriteBG(u8 *, u16 *);
extern	void	SetBG(u8 *, u16 *);
extern	void	gputchar(u16, int, int, char, u16 *);
extern	void	gprintstr(u16, int, int, char*, u16 *);


extern	u16	*__cursor;
extern	u16	__color;

#endif
