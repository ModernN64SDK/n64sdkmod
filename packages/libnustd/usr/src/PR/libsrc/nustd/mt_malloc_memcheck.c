/*
=============================================================================
        Copyright (C) 1997-1999 NINTENDO Co.,Ltd.
        
        $RCSfile: mt_malloc_memcheck.c,v $
        $Revision: 1.2 $
        $Date: 1999/05/27 07:44:21 $
=============================================================================
Function name: mt_malloc_memcheck
-----------------------------------------------------------------------------
Syntax:      #include <malloc.h>
             int mt_malloc_memcheck(struct malloc_status_st *st)

Explanation:  Multithread support version of malloc_memcheck().  The arguments, returned values and other explanations are the same as for malloc_memcheck(), so reference that function for details.

----------------------------------------------------------------------------- */
#include    <ultra64.h>
#include    "string.h"
#include    "malloc.h"
#include    "_malloc.h"

int mt_malloc_memcheck(struct malloc_status_st *st)
{
    OSIntMask svintmask ;
    int ret ;

    if ((int)malloc_ptr == -1)  return  -1;

    svintmask = osSetIntMask( OS_IM_NONE ) ; /* Prohibit all interrupts */

    ret = _malloc_memcheck(malloc_ptr, st);

    osSetIntMask( svintmask ) ; /* Reset interrupt mask */

    return ret ;
}
