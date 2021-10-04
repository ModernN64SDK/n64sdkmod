/*---------------------------------------------------------------------
        Copyright (C) 1998 Nintendo.
        
        File            main.h
        Coded    by     Koji Mitsunari. Apr 25, 1998.
        Modified by     Koji Mitsunari. Oct 27, 1998.
        Comments        64GB-PAK test program
   
        $Id: 
   ---------------------------------------------------------------------*/
#ifndef	_main_h_
#define	_main_h_

/*--------------------------------------------------------------------
                          Screen Display Message
  --------------------------------------------------------------------*/
#ifdef	LANG_ENGLISH
#include "mes_e.h"
#define	printmes	prints
#else
#include "mes.h"
#define	printmes	printkanji
#endif

/*--------------------------------------------------------------------
                              Macro Definition
  --------------------------------------------------------------------*/
/*--- RAM data size to be read from GameBoy ---*/
#define	DATA_SIZE	0x8000

#define	ERR_X		24
#define	ERR_DY		16
#define	ERR_Y1		64
#define	ERR_Y2		(ERR_Y1+ERR_DY)
#define	ERR_Y3		(ERR_Y1+ERR_DY*2)
#define	ERR_Y4		(ERR_Y1+ERR_DY*3)
#define	ERR_Y5		(ERR_Y1+ERR_DY*4)
#define	ERR_YY		(ERR_Y1+ERR_DY*6)

/*--------------------------------------------------------------------
                              External Statement
 ---------------------------------------------------------------------*/

extern s32	GbpakSramInit(OSPfs *);
extern s32	GbpakSramReadWrite(OSPfs *, u16, u32, u8 *, u32);

extern	u32	cfb_16_a[];

#endif /* _main_h_ */
