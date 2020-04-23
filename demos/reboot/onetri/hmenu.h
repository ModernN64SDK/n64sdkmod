
/*---------------------------------------------------------------------*
        Copyright (C) 1998 Nintendo.
        
        $RCSfile: hmenu.h,v $
        $Revision: 1.1 $
        $Date: 1999/07/05 02:20:04 $
 *---------------------------------------------------------------------*/
#ifndef __MAIN_H__
#define __MAIN_H__

#include "hddproc.h"

#define SCREEN_MENU 		0
#define SCREEN_ONETRI 		1

extern void main_setScreen(s32 i);
extern s32  main_getScreen(void);

extern void scr_menu(void);

extern u16  _trig;

#endif

