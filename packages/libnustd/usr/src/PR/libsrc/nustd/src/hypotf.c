/*
=============================================================================
        Copyright (C) 1997-1999 NINTENDO Co.,Ltd.
        
        $RCSfile: hypotf.c,v $
        $Revision: 1.2 $
        $Date: 1999/04/30 10:39:32 $
=============================================================================
function name: hypotf
-----------------------------------------------------------------------------
format:   #include <math.h>
        float   hypotf(float x, float y);
argument:   x  length of one side 
            y  length of one side  
return value: returns length of hypotenuse of right angled triangle from two 
              sides, x and y  explanation : calculates length of hypotenuse 
              of right angled triangle from two sides x and y.  
-----------------------------------------------------------------------------
*/
#include    "math.h"

float   hypotf(float x, float y)
{
    return  _nsqrtf(x*x + y*y);
}
