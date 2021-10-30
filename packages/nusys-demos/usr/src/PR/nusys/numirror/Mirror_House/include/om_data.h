/*======================================================================*/
/*         OBJECT MANAGER                                               */
/*          om_data.h                                                   */
/*                                                                      */
/*      Copyright (C) 1998, NINTENDO Co,Ltd.                            */
/*                                                                      */
/*======================================================================*/

#ifndef _OM_H_
#define _OM_H_
#include    "om.h"
#endif

#ifndef _OM_INFO_H_
#define _OM_INFO_H_
#include    "om_info.h"
#endif

#ifndef _OM_DATA_H_
#define _OM_DATA_H_

#include    "om_def.h"

/* ===========================================================================
	External Reference Declarations for Character Resource Data
 =========================================================================== */
extern NDR_RESINFO resinfo_frake_walk;
extern NDR_RESINFO resinfo_frake_down;
extern NDR_RESINFO resinfo_frake_shadow;
extern NDR_RESINFO resinfo_CornGroup;
extern NDR_RESINFO resinfo_goal;
extern NDR_RESINFO resinfo_frake_select;
extern NDR_RESINFO resinfo_frake_kime;
extern NDR_RESINFO resinfo_pump;

/* ===========================================================================
	Character Resource Data Definitions 
 =========================================================================== */
static NDR_RESINFO *om_frake[]        =  {&resinfo_frake_walk,
                                          &resinfo_frake_down,
                                          NULL
};

static NDR_RESINFO *om_corn_frake[]   =  {&resinfo_CornGroup,
                                          NULL
};

static NDR_RESINFO *om_goal[]         =  {&resinfo_goal,
                                          NULL
};

static NDR_RESINFO *om_frake_shadow[] =  {&resinfo_frake_shadow,
                                          NULL
};

static NDR_RESINFO *om_frake_sel[] =   {&resinfo_frake_select,
	                                    NULL
};

static NDR_RESINFO *om_frake_kime[] =   {&resinfo_frake_kime,
	                                     NULL
};

static NDR_RESINFO *om_pump[] =   {&resinfo_pump,
	                                     NULL
};


/* ===========================================================================
	Character Data Header for the Game 
 =========================================================================== */
static OM_HEADER_ST om_info_header = { 0, 10 };

/* ===========================================================================
	Character Data for the Game
 =========================================================================== */
static OM_CHARACTER_INFO_DATA om_character_info_data [] = 
       { {OM_FRAKE,         2,  om_frake,       -2700, -100,  2700, 0.0,  0.0, 0.0, 100.0, 0, 1},
         {OM_CORNFRAKE,     1,  om_corn_frake,   2100,    0,  2400, 0.0,  0.0, 0.0, 100.0, 0, 1},
         {OM_GOAL_PLATE,    1,  om_goal,        -3300,  500, -2700, 0.0,  0.0, 0.0, 100.0, 0, 1},
         {OM_FRAKE_SHADOW,  1,  om_frake_shadow,-2700, -120,  2700, 0.0,  0.0, 0.0, 100.0, 0, 1},
         {OM_FRAKE_SELECT1, 1,  om_frake_sel,  -650.0, -430.0, -1800.0, 0.0,  0.0, 0.0, 100.0, 0, 1},
         {OM_FRAKE_SELECT2, 1,  om_frake_sel,  -190.0, -430.0, -1800.0, 0.0,  0.0, 0.0, 100.0, 0, 1},
         {OM_FRAKE_SELECT3, 1,  om_frake_sel,   250.0, -430.0, -1800.0, 0.0,  0.0, 0.0, 100.0, 0, 1},
         {OM_FRAKE_SELECT4, 1,  om_frake_sel,   720.0, -430.0, -1800.0, 0.0,  0.0, 0.0, 100.0, 0, 1},
         {OM_FRAKE_KIME,    1,  om_frake_kime,   600.0, -430.0, -1800.0, 0.0,  0.0, 0.0, 100.0, 0, 1},
         {OM_PUMP,          1,  om_pump,   -500.0, -580.0, -1800.0, 0.0,  0.0, 0.0, 100.0, 0, 1},
         {NULL, NULL, NULL, 0, 0, 0, 0, 0, 0, 0, 0},
};

static OM_CHARACTER_INFO om_character_info = { &om_info_header, om_character_info_data };

#endif

