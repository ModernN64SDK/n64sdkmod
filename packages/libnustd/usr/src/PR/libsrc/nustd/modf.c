/*
=============================================================================
        Copyright (C) 1997-1999 NINTENDO Co.,Ltd.
        
        $RCSfile: modf.c,v $
        $Revision: 1.1 $
        $Date: 1999/04/14 07:18:41 $
=============================================================================
function name: modf
-----------------------------------------------------------------------------
format:   #include <math.h>
        double modf(double x, double *intptr)
argument:   x     original value which divides into fractional part and integer part
        intptr    integer part
return value:  fractional part
explanation:   divides argumenet x into fractional part and integer part
-----------------------------------------------------------------------------
*/
double modf(double x, double *intptr)
{

    int a;
    a = x;
    *intptr = (double)a;
    return  x - (double)a;
}
