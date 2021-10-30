/*
=============================================================================
        Copyright (C) 1997-1999 NINTENDO Co.,Ltd.
        
        $RCSfile: initheap.c,v $
        $Revision: 1.1 $
        $Date: 1999/04/30 02:12:54 $
=============================================================================
Function name: InitHeap
-----------------------------------------------------------------------------
Syntax:          #include <malloc.h>
                 int InitHeap(void *head, unsigned int  size);
Arguments:       head  Pointer to head of reserved region.
                 size  Size of the region.
Returned value:  0 if correctly reserved, -1 if reservation fails.
Explanation:     Reserves a region for memory allocation and performs initialization. 
-----------------------------------------------------------------------------
*/
#include    <ultra64.h>
#include    "string.h"
#include    "malloc.h"
#include    "_malloc.h"

char    *malloc_ptr = (char *)-1;

void *_InitHeap(void *ptr, unsigned int size)
{
    unsigned int    p;
    struct mallocST *malloc_st_ptr;

    if (size < 0x20)    return  (void *)-1;

    p = (unsigned int)ptr;
    p += 15;
    p &= (~15);
    size -= p - (unsigned int)(ptr);
    malloc_st_ptr = (struct mallocST *)p;
    malloc_st_ptr->next = 0;
    malloc_st_ptr->size = size - MALLOC_HEADSIZE;
    malloc_st_ptr->flag = 0;
    malloc_st_ptr->allsize = size;
    return  malloc_st_ptr;
}

int InitHeap(void *head, unsigned int  size)
{
    malloc_ptr = _InitHeap(head, size);
    if ((int)malloc_ptr == -1)  return  -1;
    return  0;
}
