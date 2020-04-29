/*******************************************************************
 *
 *  menu.h
 *
 *******************************************************************/

#ifndef   __MENU_HEADER__
#define   __MENU_HEADER__

#include  <nusys.h>

#include  "define.h"

#define   MENU0_START     0
#define   MENU0_OPTIONS   1
#define   MENU1_RESTART   0
#define   MENU1_EFFECTS   1

extern NUContData   contData;

extern Vp           viewport;
extern Gfx          rspinit_dl[], rdpinit_dl[];

extern Gfx          glist[GLIST_LENGTH];
extern Gfx *        glistp;

extern void         ClearBackground(u8 r, u8 g, u8 b);

extern void         Menu0(void);
extern void         Menu1(void);

#endif /* __MENU_HEADER__ */
