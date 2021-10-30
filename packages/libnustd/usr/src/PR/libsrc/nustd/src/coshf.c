/*
=============================================================================
        Copyright (C) 1997-1999 NINTENDO Co.,Ltd.
        
        $RCSfile: coshf.c,v $
        $Revision: 1.1 $
        $Date: 1999/04/14 07:18:39 $
=============================================================================
Function name:coshf
-----------------------------------------------------------------------------
Format:  #include <math.h>
        float coshf(float t);
Argument:  t radian angle
Return value: hyperbolic cosine of argument t
Explanation: finds hyperbolic cosine of argument t
-----------------------------------------------------------------------------
*/
#include "math.h"

float coshf(float t)
{
    float   p, m;
    
    p = expf(t);
    m = expf(-t);
    return (p+m)/2;
}


