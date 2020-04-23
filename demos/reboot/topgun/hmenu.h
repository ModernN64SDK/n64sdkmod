
/*---------------------------------------------------------------------*
        Copyright (C) 1998 Nintendo.
        
        $RCSfile: hmenu.h,v $
        $Revision: 1.1 $
        $Date: 1999/07/05 02:20:13 $
 *---------------------------------------------------------------------*/
#ifndef __MAIN_H__
#define __MAIN_H__

#include "hddproc.h"

#define SCREEN_MENU 		0
#define SCREEN_TOPGUN 		1
#define SCREEN_RTC 		    2

#define SCREEN_SUMMON	    4

#define BIGBUF_SIZE         0x200000

extern void main_setScreen(s32 i);
extern s32  main_getScreen(void);

extern void scr_menu(void);
extern void scr_rtc(void);

extern void scr_summon(void);

extern OSMesgQueue myQ;

extern u8   bigbuf[];

extern u16  _trig;

#endif

