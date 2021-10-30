/*
=============================================================================
        Copyright (C) 1997-1999 NINTENDO Co.,Ltd.
        
        $RCSfile: tanhf.c,v $
        $Revision: 1.1 $
        $Date: 1999/04/14 07:18:42 $
=============================================================================
function name: tanhf
-----------------------------------------------------------------------------
format:   #include <math.h>
        float tanhf(float t);
argument:   t   radian angle
return value: hyperbolic tangent of argument t
explanation:   finds hyperbolic tangent of argument t
-----------------------------------------------------------------------------
*/
#include "math.h"

float tanhf(float t)
{
    float   p, m;
    
    p = expf(t);
    m = expf(-t);
    return (p-m)/(p+m);
}


