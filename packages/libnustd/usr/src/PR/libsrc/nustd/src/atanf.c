/*
=============================================================================
        Copyright (C) 1997-1999 NINTENDO Co.,Ltd.
        
        $RCSfile: atanf.c,v $
        $Revision: 1.1 $
        $Date: 1999/04/14 07:18:39 $
=============================================================================
Function name : atanf
-----------------------------------------------------------------------------
Format :  #include <math.h>
          float atanf(float x);
Argument :   x (no limit)
Return value : arc tangent (range of -p/2 ~ p/2)
Explanation : Finds arc tangent of argument x
-----------------------------------------------------------------------------
*/
#include "math.h"

float atanf(float x)
{
    return atan2f(x,1.0);
}


