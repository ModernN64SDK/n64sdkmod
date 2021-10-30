/*
=============================================================================
        Copyright (C) 1997-1999 NINTENDO Co.,Ltd.
        
        $RCSfile: asinf.c,v $
        $Revision: 1.2 $
        $Date: 1999/04/30 10:37:18 $
=============================================================================
Function name : asinf
-----------------------------------------------------------------------------
Format :  #include <math.h>
        float asinf(float x);
Argument :   x (range of -1 ~ 1)
Return value : arc sine (range of -p/2 ~ p/2)
Explanation : Finds arc sine of argument x
-----------------------------------------------------------------------------
*/
#include "math.h"


float asinf(float x)
{
    float   z;

    if (fabsf(x)>1) return FVAL_ZERO;
    z = _nsqrtf(1-x*x);
    if (z != 0) return atan2f(x,z);
    if (x>0) return FVAL_PI/2;
    else return -FVAL_PI/2;
}


