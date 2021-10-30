/*
=============================================================================
        Copyright (C) 1997-1999 NINTENDO Co.,Ltd.
        
        $RCSfile: log10.c,v $
        $Revision: 1.1 $
        $Date: 1999/04/14 07:18:40 $
=============================================================================
function name: log10
-----------------------------------------------------------------------------
format:   #include <math.h>
        double log10(double x);
argument:   x
return value: common logarithm of argument x
explanation:  calculates common logarithm of argument x
        log10(x) = log(x) / log(10)
-----------------------------------------------------------------------------
*/
#include "math.h"

double log10(double x)
{
    return(log(x) / DVAL_LOG10);
}
