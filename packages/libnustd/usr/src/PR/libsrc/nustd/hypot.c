/*
=============================================================================
        Copyright (C) 1997-1999 NINTENDO Co.,Ltd.
        
        $RCSfile: hypot.c,v $
        $Revision: 1.1 $
        $Date: 1999/04/14 07:18:40 $
=============================================================================
function name: hypot
-----------------------------------------------------------------------------
format:   #include <math.h>
        double  hypot(double x, double y);
argument:   x  length of one side  
            y  length of one side
return value: returns length of hypotenuse of right angled triangle from two sides, x and y  
explanation:  calculates length of hypotenuse of right angled triangle from two sides,  x and  y
-----------------------------------------------------------------------------
*/
#include    "math.h"

double  hypot(double x, double y)
{
    return  sqrt(x*x + y*y);
}
