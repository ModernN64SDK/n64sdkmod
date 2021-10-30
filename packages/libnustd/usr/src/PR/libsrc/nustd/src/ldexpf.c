/*
=============================================================================
        Copyright (C) 1997-1999 NINTENDO Co.,Ltd.
        
        $RCSfile: ldexpf.c,v $
        $Revision: 1.1 $
        $Date: 1999/04/14 07:18:40 $
=============================================================================
function name: ldexpf
-----------------------------------------------------------------------------
format:   #include <math.h>
        float   ldexpf(float x, int exp);
argument:   x
        exp
return value: returns x * (2^exp) 
explanation:  calculates  x * (2^exp)  
-----------------------------------------------------------------------------
*/
#include    "math.h"

float   ldexpf(float x, int exp)
{
    return  x * powf(2, exp);
}

