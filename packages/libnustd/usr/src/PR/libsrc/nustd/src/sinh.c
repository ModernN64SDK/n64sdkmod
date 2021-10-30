/*
=============================================================================
        Copyright (C) 1997-1999 NINTENDO Co.,Ltd.
        
        $RCSfile: sinh.c,v $
        $Revision: 1.1 $
        $Date: 1999/04/14 07:18:41 $
=============================================================================
function name: sinh
-----------------------------------------------------------------------------
format:   #include <math.h>
        double sinh(double t);
argument:   t  radian angle
return value: hyperbolic sine of argument t   
explanation:  finds hyperbolic sine of argument t  
-----------------------------------------------------------------------------
*/
#include "math.h"

double sinh(double t)
{
    double  p, m;
    
    p = exp(t);
    m = exp(-t);
    return (p-m)/2;
}


