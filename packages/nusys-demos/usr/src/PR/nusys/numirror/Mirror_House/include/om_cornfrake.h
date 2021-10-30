/*======================================================================*/
/*         OBJECT MANAGER                                               */
/*          om_cornfrake.h                                              */
/*                                                                      */
/*      Copyright (C) 1998, NINTENDO Co,Ltd.                            */
/*                                                                      */
/*======================================================================*/
#ifndef _OM_CORNFRAKE_H_
#define _OM_CORNFRAKE_H_

/* ----------------------------------------------------------------------------
     Cornflake Arrangment Structure 
---------------------------------------------------------------------------- */
typedef struct {
    int      area_number;           /* Number of the area in which cornflakes are present */
    int      dsp_status;
    float    x, y, z;               /* Center coordinates of the cornflake arrangment */
    float    r;
} OM_CORNFRAKE_ST;

#endif /* _OM_CORNFRAKE_H_ end */

