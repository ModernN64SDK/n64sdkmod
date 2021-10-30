/*
=============================================================================
        Copyright (C) 1997-1999 NINTENDO Co.,Ltd.
        
        $RCSfile: pow.c,v $
        $Revision: 1.1 $
        $Date: 1999/04/14 07:18:41 $
=============================================================================
function name: pow
-----------------------------------------------------------------------------
format:   #include <math.h>
        double pow(double x, double y);
argument:   x   base
        y     exponent
return value: x to the y power 
explanation:  gets the y power of x  
-----------------------------------------------------------------------------
*/
#include    "math.h"

double pow(double x, double y)
{
    int a;
    double b;

    a = (int)y;

    if (x != 0.0 && y == 0.0)   return  1.0;
    if (x == 0.0)               return  0.0;
    if ((x == 0.0 && y == 0.0) || (x < 0 && y != (double)a))    return 0.0;

    if (y != (double)a) x = exp(y * log(x));
    else    {
        if (y>0)    {
            y--;
            b = x;
            while(y--)  x *=b;
        }   else    {
            b = 1.0;
            while(y++)  b /= x;
            x = b;
        }
    }
    return  x;
}

