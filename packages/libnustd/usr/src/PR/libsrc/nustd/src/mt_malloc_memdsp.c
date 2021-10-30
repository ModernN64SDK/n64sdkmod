/*
=============================================================================
        Copyright (C) 1997-1999 NINTENDO Co.,Ltd.
        
        $RCSfile: mt_malloc_memdsp.c,v $
        $Revision: 1.2 $
        $Date: 1999/05/27 07:44:21 $
=============================================================================
Function name: mt_malloc_memdsp
----------------------------------------------------------------------------- 
Syntax:       #include <malloc.h>
              int mt_malloc_memdsp(unsigned char *s, int size)
Explanation:  Multithread support version of malloc_memdsp(). The arguments, returned values and other explanations are the same as for malloc_memdsp(), so reference that function for details.

-----------------------------------------------------------------------------
*/
#include    <ultra64.h>
#include    "string.h"
#include    "malloc.h"
#include    "_malloc.h"

int mt_malloc_memdsp(unsigned char *s, int size)
{
    OSIntMask svintmask ;
    int ret ;

    if ((int)malloc_ptr == -1)  {
            sprintf(&s[ 0], "malloc_memdsp error:check InitHeap()");
            sprintf(&s[60], "");
            return  2;
    }

    svintmask = osSetIntMask( OS_IM_NONE ) ; /* Prohibit all interrupts */

    ret = _malloc_memdsp(malloc_ptr, s, size);

    osSetIntMask( svintmask ) ; /* Reset interrupt mask */

    return ret ;
}
