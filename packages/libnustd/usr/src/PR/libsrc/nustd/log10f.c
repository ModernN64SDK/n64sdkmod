
/*
=============================================================================
        Copyright (C) 1997-1999 NINTENDO Co.,Ltd.
        
        $RCSfile: log10f.c,v $
        $Revision: 1.1 $
        $Date: 1999/04/14 07:18:40 $
=============================================================================
function name: log10f
-----------------------------------------------------------------------------
format:   #include <math.h>
        float log10f(float x);
argument:   x
return value: common logarithm of argument x 
explanation:  calculates common logarithm of argument x
        log10f(x) = logf(x) / logf(10)
-----------------------------------------------------------------------------
*/
#include "math.h"

float log10f(float x)
{
    return(logf(x) / FVAL_LOG10);
}
