/*
=============================================================================
        Copyright (C) 1997-1999 NINTENDO Co.,Ltd.
        
        $RCSfile: div.c,v $
        $Revision: 1.1 $
        $Date: 1999/04/14 07:18:39 $
=============================================================================
function name: div
-----------------------------------------------------------------------------
format:  #include <stdlib.h>
        div_t *div(int numer, int denom);
argument:  	number   number to be divided
        	denom    number to divide by
return value: returns div_t type structure 
explanation: divides argument number by argument denom, and returns div_t structure
		 that consists of quotient and remainder 
-----------------------------------------------------------------------------
*/
#include    "stdlib.h"

div_t div(int numer, int denom)
{
    static  div_t   div_t_n;

    div_t_n.quot = numer / denom;
    div_t_n.rem = numer % denom;
    return  div_t_n;
}
