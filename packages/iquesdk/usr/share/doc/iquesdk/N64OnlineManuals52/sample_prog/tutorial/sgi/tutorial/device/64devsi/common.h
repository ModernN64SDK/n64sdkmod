/*******************************************************************
 *
 *  common.h
 *
 *******************************************************************/

#ifndef   __COMMON_H__
#define   __COMMON_H__

#include  <nusys.h>

/* Screen size */
#define   SCREEN_WD           320
#define   SCREEN_HT           240

/* Display list length (number of elements) */
#define   GLIST_LENGTH        4096

/* Rows displayed in title & subtitle */
#define   TITLE_ROW           2
#define   SUBTITLE_ROW        4

/* Main menu (top menu) identifier */
#define   MENU_ID_TOP         -1

/* Menu start row  */
#define   MENU_TOP_ROW        8

/* Menu start column */
#define   MENU_LEFT_COLUMN    10

extern NUContData   contData;

extern s32          menuID;

extern s8           messageBuffer[128];

void                ClearDisplay(void);
void                ReadController(void);
s32                 DrawAndProcessMenu(s32 index, u32 count, s8 ** items);
void                DrawTitle(s8 * title, s8 * subtitle);

#endif /* __COMMON_H__ */
