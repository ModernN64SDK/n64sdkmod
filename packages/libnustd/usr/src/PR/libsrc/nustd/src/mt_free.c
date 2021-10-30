/*
=============================================================================
        Copyright (C) 1997-1999 NINTENDO Co.,Ltd.
        
        $RCSfile: mt_free.c,v $
        $Revision: 1.2 $
        $Date: 1999/05/27 07:44:21 $
=============================================================================
Function name: mt_free
-----------------------------------------------------------------------------
Syntax:       #include <malloc.h>
              void mt_free(void *ptr);
Explanation:  Multithread support version of free().  The arguments, returned values and other explanations are the same as for free(), so reference that function for details.
-----------------------------------------------------------------------------
*/
#include    <ultra64.h>
#include    "string.h"
#include    "malloc.h"
#include    "_malloc.h"

void mt_free(void *ptr)
{
    OSIntMask svintmask ;

    if ((int)malloc_ptr == -1)  return;

    svintmask = osSetIntMask( OS_IM_NONE ) ; /* Prohibit all interrupts */

    _free(malloc_ptr, ptr);

    osSetIntMask( svintmask ) ; /* Reset interrupt mask  */
}
