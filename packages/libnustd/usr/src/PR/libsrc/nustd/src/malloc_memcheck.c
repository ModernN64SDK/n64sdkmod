/*
=============================================================================
        Copyright (C) 1997-1999 NINTENDO Co.,Ltd.
        
        $RCSfile: malloc_memcheck.c,v $
        $Revision: 1.1 $
        $Date: 1999/04/30 02:13:13 $
=============================================================================
Function name: malloc_memcheck
-----------------------------------------------------------------------------
Syntax:          #include <malloc.h>
                 int malloc_memcheck(struct malloc_status_st *st)
Arguments:       st  Pointer to structure returning information on allocated memory regions.
Returned value:  0 if normal.
                 If not 0, then the content is incorrect.  In this case, the size of the total memory area allocated is returned.  If the content is incorrect when the total memory size is 0, then -1 is returned.
Explanation:     Checks allocated memory regions and gets information regarding whether they are normal or abnormal, and whether they are being used or not being used.
-----------------------------------------------------------------------------
*/
#include    <ultra64.h>
#include    "string.h"
#include    "malloc.h"
#include    "_malloc.h"

int malloc_memcheck(struct malloc_status_st *st)
{
    if ((int)malloc_ptr == -1)  return  -1;
    return  _malloc_memcheck(malloc_ptr, st);
}
