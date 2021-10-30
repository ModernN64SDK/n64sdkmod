/*
=============================================================================
        Copyright (C) 1997-1999 NINTENDO Co.,Ltd.
        
        $RCSfile: mt_malloc.c,v $
        $Revision: 1.2 $
        $Date: 1999/05/27 07:44:21 $
=============================================================================
Function name: mt_malloc
-----------------------------------------------------------------------------
Syntax:       #include <malloc.h>
              void *mt_malloc(int size);
Explanation:  Multithread support version of malloc().  The arguments, returned values and other explanations are the same as for malloc(), so reference that function for details.

-----------------------------------------------------------------------------
*/
#include    <ultra64.h>
#include    "string.h"
#include    "malloc.h"
#include    "_malloc.h"

void *mt_malloc(int size)
{
    OSIntMask svintmask ;
    void *ret ;

    if ((int)malloc_ptr == -1)  return  NULL;

    svintmask = osSetIntMask( OS_IM_NONE ) ; /* Prohibit all interrupts */

    ret = _malloc(malloc_ptr, size);

    osSetIntMask( svintmask ) ; /* Reset interrupt mask */

    return ret ;
}
