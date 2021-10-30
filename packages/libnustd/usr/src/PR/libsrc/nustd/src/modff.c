/*
=============================================================================
        Copyright (C) 1997-1999 NINTENDO Co.,Ltd.
        
        $RCSfile: modff.c,v $
        $Revision: 1.2 $
        $Date: 1999/04/30 01:19:31 $
=============================================================================
Function name: modff
-----------------------------------------------------------------------------
Format:   #include <math.h>
        float modff(float x, float *intptr);
Argument:   x   original value which divides into fractional part and integer part
        intptr   integer part
Return value: fractional part
Explanation:   divides argument x into fractional part and integer part
-----------------------------------------------------------------------------
*/
float modff(float x, float *intptr)
{

    int a;
    a = x;
    *intptr = (float)a;
    return  x - (float)a;
}
