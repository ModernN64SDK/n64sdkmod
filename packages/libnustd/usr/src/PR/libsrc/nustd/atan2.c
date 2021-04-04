/*
=============================================================================
        Copyright (C) 1997-1999 NINTENDO Co.,Ltd.
        
        $RCSfile: atan2.c,v $
        $Revision: 1.1 $
        $Date: 1999/04/14 07:18:39 $
=============================================================================
Function name : atan2
-----------------------------------------------------------------------------
Format :  #include <math.h>
        double atan2(double y, double x);
Argument :  x
            y 
Return value : returns arc tangent of argument y, argument x
Explanation :  returns arc tangent of argument y, argument x
        atan(t) = t / (1+(t^2/(3+(2^2*t^2/(5+(3^2*t^2/(7+....)))))))
-----------------------------------------------------------------------------
*/
#include "math.h"

double atan2(double y, double x)
{
    double  xx, z;
    double  a;
    int     i;

    if (x==0 && y==0) return DVAL_ZERO;
    if (x==0) {
        if (y>0) return DVAL_PI/2;
        else return -DVAL_PI/2;
    }

    xx = y/x;
    if (fabs(xx)<=1) z = xx;
    else z = 1/xx;
    a = 0;
    for (i=14; i>=1; i--) a = i*i*z*z/(1+2*i+a);
    z = z/(1+a);
    if (xx> 1) z = DVAL_PI/2 - z;
    if (xx<-1) z = -DVAL_PI/2 - z;

    if (x>0) return z;
    else {
        if (y>0) return z+DVAL_PI;
        else return z-DVAL_PI;
    }
}


