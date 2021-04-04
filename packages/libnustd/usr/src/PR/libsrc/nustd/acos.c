/*
=============================================================================
        Copyright (C) 1997-1999 NINTENDO Co.,Ltd.
        
        $RCSfile: acos.c,v $
        $Revision: 1.1 $
        $Date: 1999/04/14 07:18:38 $
=============================================================================
Function name : acos
-----------------------------------------------------------------------------
Format  #include <math.h>
        double acos(double x);
Argument :  x (range of -1 ~ 1)
Return value : arc cosine (range of 0 ~ p)
Explanation : Finds arc cosine of argument x
-----------------------------------------------------------------------------
*/
#include "math.h"

double acos(double x)
{
    double  z;

    if (fabs(x)>1) return DVAL_ZERO;
    z = sqrt(1-x*x);
    if (z != 0) return DVAL_PI/2-atan2(x,z);
    if (x>0) return DVAL_ZERO;
    else return DVAL_PI;
}


