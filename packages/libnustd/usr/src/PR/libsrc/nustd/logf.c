/*
=============================================================================
        Copyright (C) 1997-1999 NINTENDO Co.,Ltd.
        
        $RCSfile: logf.c,v $
        $Revision: 1.1 $
        $Date: 1999/04/14 07:18:40 $
=============================================================================
function name: logf
-----------------------------------------------------------------------------
format:  #include <math.h>
        float logf(float x);
argument:  x
return value: natural logarithm of argument x
explanation:  calculates natural logarithm of argument x

        log(x) = 2*((x-1)/(x+1)) + 2/3*((x-1)/(x+1))^3 
               + (2/(2k+1))((x-1)/(x+1))^(2k+1) (k=0,1,2....)
        log(2^k * x) =  k * log(2) + log(x)
-----------------------------------------------------------------------------
*/
#include "math.h"

float logf(float x)
{
    int     n;
    float   x0, ak, z, zbk;

    if (x <  0) { return FVAL_ZERO; }       /* error */
    if (x == 0) { return FVAL_ZERO; }       /* error */
    z = 0;
    while(x>=2) { x/=2; z+=FVAL_LOG2; }
    while(x< 1) { x*=2; z-=FVAL_LOG2; }

    n = 1;
    x0 = (x-1)/(x+1);
    ak = 2 * x0;
    x0 = x0 * x0;
    do {
        zbk = z;
        z += ak/n;
        ak *= x0;
        n += 2;
    } while (z != zbk);
    return z;
}

