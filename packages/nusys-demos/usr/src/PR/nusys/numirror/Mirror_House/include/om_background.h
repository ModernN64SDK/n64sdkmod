/*======================================================================*/
/*          OBJECT MANAGER                                              */
/*          om_background.h                                             */
/*                                                                      */
/*      Copyright (C) 1998, NINTENDO Co,Ltd.                            */
/*                                                                      */
/*======================================================================*/
#ifndef _OM_BACKGROUND_H_
#define _OM_BACKGROUND_H_

/* ----------------------------------------------------------------------------
     Backgraound Information Structures
---------------------------------------------------------------------------- */
typedef struct  {                       /*  Background object */
    int     rendermode;                 /*      Rendering mode(*1) */
    float   x,y,z;                      /*      Display address (offset)*/
    void    *dsegbase[4];               /*      Segment address for display(Gfx, Vtex, Texture, Tlut) */
    void    *hsegbase[2];               /*      Segment address for hit(Gfx, Vtex) */
} OM_BACKGROUND_OBJECT_ST;

typedef struct  {                       /*  Bakcground group (grouping of multiple objects) */
    float   x,y,z;                      /*      Display position (world coordinates) */
    float   xo, yo, zo;                 /*      Display position and deviation from the center of the sphere described below, which is used for display evaluation */
    float   s;                          /*      Radius of the sphere used for display evaluation */
    int     object_n;                   /*      Number of background object arrays */
    OM_BACKGROUND_OBJECT_ST *bo;        /*      Pointer to structure BACKGROUND_OBJECT_ST */
} OM_BACKGROUND_GROUP_ST;

typedef struct om_background_st {               /*  Structure containing data for each section of background */
    OM_BACKGROUND_GROUP_ST  *bg;                /*      Pointer to structure BACKGROUND_GROUP_ST */
    int     status;                             /*      Status(*2) */
    int     entry_number;                       /*      Entry number at time of entry(*3) */
    char    zone_number[3];
} OM_BACKGROUND_ST;

#endif
