/*
=============================================================================
        Copyright (C) 1997-1999 NINTENDO Co.,Ltd.
        
        $RCSfile: tanh.c,v $
        $Revision: 1.1 $
        $Date: 1999/04/14 07:18:42 $
=============================================================================
function name: tanh
-----------------------------------------------------------------------------
format:   #include <math.h>
        double tanh(double t);
argument:   t    radian angle
return value: hyperbolic tangent of argument t
explanation:  finds hyperbolic tangent of argument t
-----------------------------------------------------------------------------
*/
#include "math.h"

double tanh(double t)
{
    double  p, m;
    
    p = exp(t);
    m = exp(-t);
    return (p-m)/(p+m);
}


