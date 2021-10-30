/*
=============================================================================
        Copyright (C) 1997-1999 NINTENDO Co.,Ltd.
        
        $RCSfile: labs.c,v $
        $Revision: 1.1 $
        $Date: 1999/04/14 07:18:40 $
=============================================================================
function name: labs
-----------------------------------------------------------------------------
format:   #include <stdlib.h>
        long    labs(long n);
argument:   n   integer
return value:  absolute value of n 
explanation:   returns absolute value of argument n  
-----------------------------------------------------------------------------
*/

#include <stdlib.h>

long    labs(long n)
{
    if (n < 0)  return  -n;
    return  n;
}
