/*
=============================================================================
        Copyright (C) 1997-1999 NINTENDO Co.,Ltd.
        
        $RCSfile: asin.c,v $
        $Revision: 1.1 $
        $Date: 1999/04/14 07:18:38 $
=============================================================================
Function name : asin
-----------------------------------------------------------------------------
Format :  #include <math.h>
        double asin(double x)
Argument :  x (range of -1 ~ 1)
Return value : arc sine (range of -p/2 ~ p/2)
Explanation : Finds arc sine of argument x
-----------------------------------------------------------------------------
*/
#include "math.h"


double asin(double x)
{
    double  z;

    if (fabs(x)>1) return DVAL_ZERO;
    z = sqrt(1-x*x);
    if (z != 0) return atan2(x,z);
    if (x>0) return DVAL_PI/2;
    else return -DVAL_PI/2;
}


