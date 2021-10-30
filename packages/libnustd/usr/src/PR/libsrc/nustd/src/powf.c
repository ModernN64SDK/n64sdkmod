/*
=============================================================================
        Copyright (C) 1997-1999 NINTENDO Co.,Ltd.
        
        $RCSfile: powf.c,v $
        $Revision: 1.2 $
        $Date: 1999/04/30 01:19:44 $
=============================================================================
Function name: powf
-----------------------------------------------------------------------------
Format:   #include <math.h>
        float powf(float x, float y);
Argument:   x   base
        y   exponent
Return value: x to the y power
Explanation:  gets the y power of x  
-----------------------------------------------------------------------------
*/
#include    "math.h"

float powf(float x, float y)
{
    int a;
    float b;

    a = (int)y;

    if (x != 0.0 && y == 0.0)   return  1.0;
    if (x == 0.0)               return  0.0;
    if ((x == 0.0 && y == 0.0) || (x < 0 && y != (float)a)) return 0.0;

    if (y != (float)a) x = expf(y * logf(x));
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
