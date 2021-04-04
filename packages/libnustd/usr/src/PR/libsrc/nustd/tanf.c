/*
=============================================================================
        Copyright (C) 1997-1999 NINTENDO Co.,Ltd.
        
        $RCSfile: tanf.c,v $
        $Revision: 1.2 $
        $Date: 1999/04/30 10:40:25 $
=============================================================================
Function name: tanf
-----------------------------------------------------------------------------
Format:   #include <math.h>
        float tanf(float t);
Argument:   t     radian angle
Return value: tangent
Explanation:   finds tangent  using argument t
-----------------------------------------------------------------------------
*/
#include "math.h"

float tanf(float t)
{
    float   c, s;
    long    tn;

    tn = (t / FVAL_PI) + ((t>=0) ? 0.5 : -0.5);
    t = t - tn * FVAL_PI;
    if (t == 0) return FVAL_ZERO;
    s = _nsinf(t);
    c = _ncosf(t);
    if (c != 0) return s/c;
    else return FLT_MAX;
}


