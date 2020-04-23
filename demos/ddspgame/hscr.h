/*---------------------------------------------------------------------*
        Copyright (C) 1998 Nintendo.
        
        $RCSfile: hscr.h,v $
        $Revision: 1.1 $
        $Date: 1998/11/13 12:39:09 $
 *---------------------------------------------------------------------*/

#ifndef __HSCR_H__
#define __HSCR_H__

#include "main.h"
#include "font.h"
#include "hdd.h"
#include "herr.h"

#define	MES_CMD_X	30
#define	MES_CMD_Y	14
#define MES_CMD_DY      18
#define MES_DATE_X      80
#define MES_DATE_Y      96
#define MES_END_X       120
#define MES_END_Y       120

#define TEST_START_LBA  1418

#define SCREEN_INIT 0
#define SCREEN_MENU 1
#define SCREEN_RESULT  2
#define SCREEN_SETRTC  3
#define SCREEN_SETMOTOR 4

#define A_BUTTON_PUSH (conts[0].nowtrg & A_BUTTON)

#define RGBA_WHITE  	0xFFFFFFFF
#define RGBA_SELECTED   0x64FFDCFF
#define RGBA_UNSELECTED 0x78A082FF
#define RGBA_YELLOW 	0xFFFF00FF

#define FUNC_NUM 13

extern s32 sb_clip(s32 val ,s32 from ,s32 to);
extern u8 fromBCD(u8 data);
extern u8 toBCD(u8 data);

extern void printDate(Gfx **gpp);
extern void _setString(Gfx **gpp,u8 * str,s32 x,s32 y,u32 color);
extern void _setJStr(Gfx **gpp,s32 str_no,s32 x,s32 y,u32 color);

extern void scr_result(Gfx **gpp);
extern void scr_init(Gfx **gpp);
extern void scr_menu(Gfx **gpp);
extern void scr_setrtc(Gfx **gpp);
extern void scr_setmotor(Gfx **gpp);

extern void test_64dd(Gfx **gpp);
extern void _setScreen(s32 i);
extern s32 _getCurrentScreen(void);
extern s32 _getCursor(void);
extern s32 dd_errbuf[];

extern void _printErrorNo(Gfx **gpp);



#endif /* __HSCR_H__ */
