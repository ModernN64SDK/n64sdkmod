/*
=============================================================================
        Copyright (C) 1997-1999 NINTENDO Co.,Ltd.
        
        $RCSfile: tan.c,v $
        $Revision: 1.1 $
        $Date: 1999/04/14 07:18:42 $
=============================================================================
function name: tan
-----------------------------------------------------------------------------
format:   #include <math.h>
        double tan(double t)
argument:   t   radian angle   
return value: tangent
explanation:   finds tangent using argument t  
-----------------------------------------------------------------------------
*/

#include "math.h"

double tan(double t)
{
    double  c, s;
    long    tn;

    tn = (t / DVAL_PI) + ((t>=0) ? 0.5 : -0.5);
    t = t - tn * DVAL_PI;
    if (t == 0) return DVAL_ZERO;
    s = sin(t);
    c = cos(t);
    if (c != 0) return s/c;
    else return DBL_MAX;
}


