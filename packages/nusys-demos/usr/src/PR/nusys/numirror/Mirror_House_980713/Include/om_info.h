/*======================================================================*/
/*         OBJECT MANAGER                                               */
/*          om_info.h                                                   */
/*                                                                      */
/*      Copyright (C) 1998, NINTENDO Co,Ltd.                            */
/*                                                                      */
/*======================================================================*/
#include	"om.h"

#ifndef _OM_INFO_H_
#define _OM_INFO_H_

/* ----------------------------------------------------------------------------
    Initial Character Data Structures for the Game 
---------------------------------------------------------------------------- */
typedef struct {
    u32             entry_number;
    int             nd_list;            /* Number of character resource data items */
    NDR_RESINFO     **ndlist;           /* Pointer to character resource data */
                                        /* Registered with the structure OM_CHARACTER_CHILD_ST */
    float           x, y, z;            /* Initial character coordinates */
    float           rx, ry, rz;         /* Initial character rotation angles */
    float           r;                  /* Radius of the character from its center coordinates */
    int             chara_select_flg;   /* Selection flag for the character selection screen */
    int             dsp_status;         /* Character display/no-display flag */
} OM_CHARACTER_INFO_DATA;

typedef struct {
    OM_HEADER_ST            *header;    /* Header portion */
    OM_CHARACTER_INFO_DATA  *data;       /* Data portion */
} OM_CHARACTER_INFO;

#endif

