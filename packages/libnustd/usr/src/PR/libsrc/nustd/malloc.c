/*
=============================================================================
        Copyright (C) 1997-1999 NINTENDO Co.,Ltd.
        
        $RCSfile: malloc.c,v $
        $Revision: 1.2 $
        $Date: 1999/04/30 02:05:03 $
=============================================================================
Function name: malloc
-----------------------------------------------------------------------------
Format:  #include <malloc.h>
        void *malloc(int size);
Argument:  size     size to reserve 
Return value: head pointer of reserved area. if failed to reserve, NULL is 
		  returned.  explanation: reserves the memory block that is more
		  than size in the area reserved by
InitHeap. The memory to be reserved has alignment of 16. 
-----------------------------------------------------------------------------
*/
#include    <ultra64.h>
#include    "string.h"
#include    "malloc.h"
#include    "_malloc.h"

void *malloc(int size)
{
    if ((int)malloc_ptr == -1)  return  NULL;
    return  _malloc(malloc_ptr, size);
}
