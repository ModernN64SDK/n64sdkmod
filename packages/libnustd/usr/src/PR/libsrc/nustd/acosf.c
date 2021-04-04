/*
=============================================================================
        Copyright (C) 1997-1999 NINTENDO Co.,Ltd.
        
        $RCSfile: acosf.c,v $
        $Revision: 1.3 $
        $Date: 1999/04/30 10:36:48 $
=============================================================================
Function name : acosf
-----------------------------------------------------------------------------
Format :  #include <math.h>
        float acosf(float x)
Argument :   x (range of -1 ~ 1)
Return value : arc cosine (range of 0 ~ p)
Explanation : Finds arc cosine of argument x
-----------------------------------------------------------------------------
*/
#include "math.h"

float acosf(float x)
{
    float   z;

    if (fabsf(x)>1) return FVAL_ZERO;
    z = _nsqrtf(1-x*x);
    if (z != 0) return FVAL_PI/2-atan2f(x,z);
    if (x>0) return FVAL_ZERO;
    else return FVAL_PI;
}
