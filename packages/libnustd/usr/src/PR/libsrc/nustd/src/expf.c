/*
=============================================================================
        Copyright (C) 1997-1999 NINTENDO Co.,Ltd.
        
        $RCSfile: expf.c,v $
        $Revision: 1.1 $
        $Date: 1999/04/14 07:18:39 $
=============================================================================
function name: expf
-----------------------------------------------------------------------------
format:   #include <math.h>
        float expf(float x);
argument:   x   exponent
return value: argument of exponential function e to the x power 
explanation:   calculates argument of exponential function e to the x power 
        exp(x) = 1 + x + x^2/2! + x^3/3! + x^4 /4! +.......   (-1<= x <=1)

        exp(nk*log2+xx) = exp(nk * log2) * exp(xx)
                        = 2^nk * exp(xx)
-----------------------------------------------------------------------------
*/
#include "math.h"

float expf(float x)
{
    float   e, a, ebk;
    float   xx, xa;
    int     n, nk;

    xa = fabsf(x);
    if (xa < FLT_EPSILON) return 1.0;           /* exp(0) */
    nk = xa / FVAL_LOG2;
    if (nk > 1024) {
        if (x>=0) {return FLT_MAX; }            /* ovver flow */
        else { return FVAL_ZERO; }              /* under flow */
    }
    xx = xa - (nk * FVAL_LOG2);
    n = 1; e = 1; a = 1;
    do {
        ebk = e;
        a *= xx/n;
        e += a;
        n++;
    }   while (e != ebk);

    while(nk) {
        e *= 2.0;
        nk--;
    }

    if (x>=0) return e;
    else return 1/e;
}


