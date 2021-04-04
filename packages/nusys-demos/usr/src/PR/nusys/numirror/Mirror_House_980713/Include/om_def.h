/*======================================================================*/
/*        OBJECT MANAGER                                                */
/*          om_def.h                                                    */
/*                                                                      */
/*      Copyright (C) 1998, NINTENDO Co,Ltd.                            */
/*                                                                      */
/*======================================================================*/

#ifndef _OM_DEF_H_
#define _OM_DEF_H_

/* Character number */
enum {
    OM_FRAKE         = 0,
    OM_CORNFRAKE     = 1,
    OM_GOAL_PLATE    = 2,
    OM_FRAKE_SHADOW  = 3,
    OM_FRAKE_SELECT1 = 4,
    OM_FRAKE_SELECT2 = 5,
    OM_FRAKE_SELECT3 = 6,
    OM_FRAKE_SELECT4 = 7,
    OM_FRAKE_KIME    = 8,
    OM_PUMP          = 9
};

/* Challenge action flag */
enum {
    OM_FRAKE_STOP = 0,
    OM_FRAKE_WALK = 1,
    OM_FRAKE_DOWN = 2
};

/* Character node number */
enum {
    OM_FRAKE_WALK_NODE = 0,
    OM_FRAKE_DOWN_NODE = 1
};

#endif

