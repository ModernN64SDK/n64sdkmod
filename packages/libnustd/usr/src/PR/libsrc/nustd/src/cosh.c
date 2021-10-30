/*
=============================================================================
        Copyright (C) 1997-1999 NINTENDO Co.,Ltd.
        
        $RCSfile: cosh.c,v $
        $Revision: 1.1 $
        $Date: 1999/04/14 07:18:39 $
=============================================================================
Function name:cosh
-----------------------------------------------------------------------------
Format:  #include <math.h>
        double cosh(double t);
Argument:  t radian angle
Return value: hyperbolic cosine of argment t 
Explanation: finds hyperbolic cosine of argument t
-----------------------------------------------------------------------------
*/
#include "math.h"

double cosh(double t)
{
    double  p, m;
    
    p = exp(t);
    m = exp(-t);
    return (p+m)/2;
}


