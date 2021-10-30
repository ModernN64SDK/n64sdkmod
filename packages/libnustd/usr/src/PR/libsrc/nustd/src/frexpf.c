/*
=============================================================================
        Copyright (C) 1997-1999 NINTENDO Co.,Ltd.
        
        $RCSfile: frexpf.c,v $
        $Revision: 1.1 $
        $Date: 1999/04/14 07:18:40 $
=============================================================================
function name: frexp
-----------------------------------------------------------------------------
format:   #include <math.h>
        float   frexpf(float x, int *expptr);
argument:   x  initial value 
        pointer to store value of n in expptr x = m * (2^n) 
return value: x = returns value of m in m * (2^n)  
explanation:   x = finds m and n that fill the formula of m * (2^n)  
               absolute value of m is smaller than 1.0 and larger than 0.5.  
-----------------------------------------------------------------------------
*/
#include    "math.h"

float   frexpf(float x, int *expptr)
{
    int a;
    float  b, c;

    if (!x) {
        *expptr = 0;
        return  0;
    }
    a = 1;
    if (x<0)    b = -x;
    else        b = x;
    c = b;

    if (b > 0.5)    {
        while(c>=b) b = powf(2, a++);
        a --;
    }   else    {
        while(c<=b) c = powf(2, (a--)-2);
        b = powf(2, a);
    }
    *expptr =  a;
    return  x / b;
}
