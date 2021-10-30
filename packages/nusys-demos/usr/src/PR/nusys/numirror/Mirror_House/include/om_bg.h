/*======================================================================*/
/*          OBJECT MANAGER                                              */
/*          om_bg.h                                                     */
/*                                                                      */
/*      Copyright (C) 1998, NINTENDO Co,Ltd.                            */
/*                                                                      */
/*======================================================================*/
#ifndef _OM_BG_H_
#define _OM_BG_H_

#include "om_cornfrake.h"

/* ----------------------------------------------------------------------------
     Structures of maze wall data
---------------------------------------------------------------------------- */
typedef struct {
    u32      attribute;         /* Wall attributes (ordinary wall, start, finish) */
    float    x, y, z;           /* Center coordinates of wall on 1 grid */
} OM_BG_WALL_ST;

typedef struct {
    int    x, y, z;             /* Grid coordinates */
    OM_BG_WALL_ST *wall;        /* Data (maximum of 4) for wall on 1 grid */
} OM_BG_GRID_ST;

typedef struct {
    char   zone_number[3];
    u32    x, y, z;             /* Zone coordinates */
    OM_BG_GRID_ST **grid;       /* Structure divided into 2 x 2 grid */
} OM_BG_ZONE_ST;


typedef struct {
    int    stage_number;        /* Stage number */
    u32    area_dsp_flg[5];     /* Map display flag for each area */
    u32    zone_dsp_flg[49];
    u32    cornfrake_num;       /* Number of cornfrake on the stage */
    u32    clear_time;
    s32    help_num;
    OM_BG_ZONE_ST    **zone;    /* Maze structure divided into 5 x 5 zones */


    OM_CORNFRAKE_ST  *corn;     /* Cornflake arrangement data */
    OM_BACKGROUND_ST **bgst;    /* Map data for rendering */
} OM_BG_STAGE_ST;

#endif /* _OM_BG_H_ end */

