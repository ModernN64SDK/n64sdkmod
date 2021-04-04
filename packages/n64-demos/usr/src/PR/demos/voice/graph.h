/*---------------------------------------------------------------------
        Copyright (C) 1998 Nintendo.
        
        File            graph.h
        Coded    by     Tetsuyuki Ootsuka.      July, 1998.
        Comments        functions for graphics
   
        $Id: 
   ---------------------------------------------------------------------*/
#ifndef	_graph_h_
#define	_graph_h_

#define WHITE		0xffff
#define BLACK		0x0000
#define GRAY		GPACK_RGBA5551(127,127,127,1)
#define BLUE		GPACK_RGBA5551(0,0,255,1)
#define GREEN		GPACK_RGBA5551(0,255,0,1)
#define RED		GPACK_RGBA5551(255,0,0,1)
#define PURPLE		GPACK_RGBA5551(255,0,255,1)
#define YELLOW		GPACK_RGBA5551(255,255,0,1)
#define CYAN		GPACK_RGBA5551(0,255,255,1)
#define DARK		GPACK_RGBA5551(50,50,128,1)
#define MARKCOLOR	GPACK_RGBA5551(50,250,50,1)
#define DARKGRAY        GPACK_RGBA5551(31,31,31,1)
#define BGCOLOR		0x0000

#define CHAR_WD		8
#define CHAR_HT		16

#define TEXT_WD		(SCREEN_WD/CHAR_WD)
#define TEXT_HT		(SCREEN_HT/CHAR_HT)

extern	void	putkanji(u16 *, int, int, u16, s16);
extern	void	printkanji(u16 *, int, int, u16, s16 *);
extern	void	prints(int, int, u16, char *);
extern	void	putc(int, int, u16, char);
extern  void    gcls(u16 *);

extern  u8      pat[][CHAR_HT];
extern	u8	kanji_font[];

#endif
