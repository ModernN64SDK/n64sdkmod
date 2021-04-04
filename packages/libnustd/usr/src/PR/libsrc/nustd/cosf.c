/*
=============================================================================
        Copyright (C) 1997-1999 NINTENDO Co.,Ltd.
        
        $RCSfile: cosf.c,v $
        $Revision: 1.2 $
        $Date: 1999/04/30 10:38:20 $
=============================================================================
Function name:_ncosf
-----------------------------------------------------------------------------
Format :  #include <math.h>
        float _ncosf(float t)
Argument :  t radian angle
Return value : cosine
Explanation : finds cosine from argument t 
        cos(t) = (t^0/0!) - (t^2/2!) + (t^4/4!) - (t^6/6!)
               +- ... t^(2n)/(2n)!
        cos(t) = 1 - t^2/2 + t^4 * (1/4! + t^2 * (-1/6! + t^2 * (1/8! +.....
-----------------------------------------------------------------------------
*/
#include "math.h"

#define C0  (-0.5   )                   /* -1/(2!)  1*/
#define C1  ( 4.166666666667e-2 )       /*  1/(4!)  24*/
#define C2  (-1.388888888889e-3 )       /* -1/(6!)  720*/
#define C3  ( 2.480158730159e-5 )       /*  1/(8!)  40320*/
#define C4  (-2.755731922399e-7 )       /* -1/(10!) 3628800*/
#define C5  ( 2.087675698787e-9 )       /*  1/(12!) 479001600*/
#define C6  (-1.147074559773e-11 )      /* -1/(14!) 87178291200*/
#define C7  ( 4.779477332387e-14 )      /*  1/(16!) */

float _ncosf(float t)
{
    float   q, qq;
    long    tn;

    tn = (t / FVAL_PI) + ((t>=0) ? 0.5 : -0.5);
    t = t - tn * FVAL_PI;

    q = t * t;
    qq = q * q * (C1 + q * (C2 + q * (C3 + q * (C4 + q * (C5 + q * (C6 + q * C7))))));
    t = 1.0 - (q / 2) + qq;

    if (tn & 0x01) return -t;
    else return t;
}


