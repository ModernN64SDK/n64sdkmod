/*======================================================================*/
/*		NuSYS							*/
/*		nudebconsole_ex2.c					*/
/*									*/
/*		Copyright (C) 1997, NINTENDO Co,Ltd.			*/
/*									*/
/*======================================================================*/
/* $Id: nudebconbuf.c,v 1.3 1999/06/09 01:07:05 ohki Exp $		*/
/*======================================================================*/
#include <nusys.h>
/* Initial setting of console window structure */
NUDebConWindow	nuDebConWin[NU_DEB_CON_WINDOW_NUM] = {

    {NU_DEB_CON_WINDOW_ON , 0, NU_DEB_CON_SCROLL_ON, 7, 0, 0, 0, 0, 0, 0, NU_DEB_CON_ROW_MAX, NU_DEB_CON_COLUMN_MAX,},
    {NU_DEB_CON_WINDOW_ON, 0, NU_DEB_CON_SCROLL_ON, 7, 0, 0, 0, 0, 0, 0, NU_DEB_CON_ROW_MAX, NU_DEB_CON_COLUMN_MAX,},
    {NU_DEB_CON_WINDOW_ON, 0, NU_DEB_CON_SCROLL_ON, 7, 0, 0, 0, 0, 0, 0, NU_DEB_CON_ROW_MAX, NU_DEB_CON_COLUMN_MAX,},
    {NU_DEB_CON_WINDOW_ON, 0, NU_DEB_CON_SCROLL_ON, 7, 0, 0, 0, 0, 0, 0, NU_DEB_CON_ROW_MAX, NU_DEB_CON_COLUMN_MAX,},
};

