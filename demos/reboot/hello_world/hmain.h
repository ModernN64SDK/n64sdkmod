
/*---------------------------------------------------------------------*
        Copyright (C) 1998 Nintendo.
        
        $RCSfile: hmain.h,v $
        $Revision: 1.1 $
        $Date: 1999/07/05 02:19:36 $
 *---------------------------------------------------------------------*/
#ifndef __MAIN_H__
#define __MAIN_H__

#include "KERNEL64.h"

#define SCREEN_MENU 		0
#define SCREEN_ONETRI 		1
#define SCREEN_RTC 		    2
#define SCREEN_FOOK		    3

extern void main_setScreen(s32 i);
extern s32  main_getScreen(void);

extern void scr_menu(void);
extern void scr_rtc(void);
extern void scr_fook(void);

extern OSMesgQueue myQ;

extern u16  _trig;

#endif

