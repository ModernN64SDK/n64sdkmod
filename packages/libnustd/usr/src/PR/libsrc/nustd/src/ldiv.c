/*
=============================================================================
        Copyright (C) 1997-1999 NINTENDO Co.,Ltd.
        
        $RCSfile: ldiv.c,v $
        $Revision: 1.2 $
        $Date: 1999/04/30 10:41:25 $
=============================================================================
Function name: _nldiv
-----------------------------------------------------------------------------
Format:  #include <stdlib.h>
        ldiv_t *_nldiv(long int numer, long int denom);
Argument:  number  number to be divided
        denom  number to dibvide by
Return value: returns ldiv_t type structure
Explanation:  divides argument number by argument denom, and returns ldiv_t
structure that consists of quotient and remainder 
-----------------------------------------------------------------------------
*/
#include    "stdlib.h"

ldiv_t _nldiv(long int numer, long int denom)
{
    ldiv_t  ldiv_t_n;
    
    ldiv_t_n.quot = numer / denom;
    ldiv_t_n.rem = numer % denom;
    return  ldiv_t_n;
}
