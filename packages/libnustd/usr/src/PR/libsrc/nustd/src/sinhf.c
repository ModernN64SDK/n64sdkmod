/*
=============================================================================
        Copyright (C) 1997-1999 NINTENDO Co.,Ltd.
        
        $RCSfile: sinhf.c,v $
        $Revision: 1.1 $
        $Date: 1999/04/14 07:18:41 $
=============================================================================
function name: sinhf
-----------------------------------------------------------------------------
format:   #include <math.h>
        float sinhf(float t);
argument:   t   radian angle
return value: hyperbolic sine of argument  t  
explanation:  finds hyperbolic sine of argument t
-----------------------------------------------------------------------------
*/
#include "math.h"

float sinhf(float t)
{
    float   p, m;
    
    p = expf(t);
    m = expf(-t);
    return (p-m)/2;
}


