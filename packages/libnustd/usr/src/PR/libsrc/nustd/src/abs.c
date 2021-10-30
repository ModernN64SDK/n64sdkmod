/*
=============================================================================
        Copyright (C) 1997-1999 NINTENDO Co.,Ltd.
        
        $RCSfile: abs.c,v $
        $Revision: 1.1 $
        $Date: 1999/04/14 07:18:38 $
=============================================================================
Function name : Fabs
-----------------------------------------------------------------------------
Format:  #include <stdlib.h>
        int abs(int n);
Argument: n   integer value
Return value:  absolute value of n
Explanation:   Returns absolute value of argument n
-----------------------------------------------------------------------------
*/

#include <stdlib.h>

int abs(int n)
{
    if (n<0)    return  -n;
    return  n;
}
