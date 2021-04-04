/*
=============================================================================
        Copyright (C) 1997-1999 NINTENDO Co.,Ltd.
        
        $RCSfile: fmodf.c,v $
        $Revision: 1.1 $
        $Date: 1999/04/14 07:18:40 $
=============================================================================
function name: fmod
-----------------------------------------------------------------------------
format:   #include <math.h>
        float fmodf(float x, float y);
argument:   x   number to be divided 
        	y  number to divide by
return value: returns remainder of x / y 
explanation:  calculates remainder of x / y 
-----------------------------------------------------------------------------
*/

float fmodf(float x, float y)
{
    int a;

    if (!y) return  0;
    a = x / y;
    return  (x - a * y);
}
