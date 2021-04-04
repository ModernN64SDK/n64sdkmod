/*
=============================================================================
        Copyright (C) 1997-1999 NINTENDO Co.,Ltd.
        
        $RCSfile: sinf.c,v $
        $Revision: 1.2 $
        $Date: 1999/04/30 10:43:06 $
=============================================================================
Function name: _nsinf
-----------------------------------------------------------------------------
Format:  #include <math.h>
        float _nsinf(float t);
Argument:  t radian angle
Return value: sine
Explanation:  finds a sine using argument
        sin(t) = (t^1/1!) - (t^3/3!) + (t^5/5!) - (t^7/7!)
               +- ... t^(2n+1)/(2n+1)!
        sin(t) = t (1 + t^2 * (-1/3! + t^2 * (1/5! + t^2 * (-1/7! +  .... )))
-----------------------------------------------------------------------------
*/

#include "math.h"

#define S0  (-1.666666666667e-1 )       /* -1/(3!)  */
#define S1  ( 8.333333333333e-3 )       /*  1/(5!)  */
#define S2  (-1.984126984127e-4 )       /* -1/(7!)  */
#define S3  ( 2.755731922399e-6 )       /*  1/(9!)  */
#define S4  (-2.505210838544e-8 )       /* -1/(11!) */
#define S5  ( 1.605904383682e-10)       /*  1/(13!) */
#define S6  (-7.64716373182e-13 )       /* -1/(15!) */
#define S7  ( 2.811457254346e-15)       /*  1/(17!) */


float _nsinf(float t)
{
    float   q, qq;
    long    tn;

    tn = (t / DVAL_PI) + (t>=0 ? 0.5 : -0.5);
    t = t - tn * FVAL_PI;
    if (fabsf(t) > FLT_EPSILON) {
        q = t * t;
        qq = q * (S0 + q * (S1 + q * (S2 + q * (S3 + q * (S4 + q * (S5 + q * (S6 + q * S7)))))));
        t = t + t * qq;
    }
    if (tn & 0x01) return -t;
    else return t;
}


