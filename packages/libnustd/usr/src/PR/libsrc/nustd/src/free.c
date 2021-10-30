/*
=============================================================================
        Copyright (C) 1997-1999 NINTENDO Co.,Ltd.
        
        $RCSfile: free.c,v $
        $Revision: 1.1 $
        $Date: 1999/04/30 02:12:34 $
=============================================================================
Function name: free
-----------------------------------------------------------------------------
Syntax:          #include <malloc.h>
                 void free(void *ptr);
Arguments:       ptr  Pointer to the start of the reserved area.
Returned value:  none
Explanation:     Releases the memory block reserved in the region reserved by InitHeap.
-----------------------------------------------------------------------------
*/
#include    <ultra64.h>
#include    "string.h"
#include    "malloc.h"
#include    "_malloc.h"

void free(void *ptr)
{
    if ((int)malloc_ptr == -1)  return;
    _free(malloc_ptr, ptr);
}
