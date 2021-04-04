/*
=============================================================================
        Copyright (C) 1997-1999 NINTENDO Co.,Ltd.
        
        $RCSfile: calloc.c,v $
        $Revision: 1.1 $
        $Date: 1999/04/30 02:12:19 $
=============================================================================
Function name: calloc
-----------------------------------------------------------------------------
Syntax:          #include <malloc.h>
                 void *calloc(size_t num, size_t size);
Arguments:       num    Number of elements
                 size   Size of elements
Returned value:  Pointer to the start of the reserved area.
                 NULL is returned if reservation fails.
        
Explanation:     Reserves a memory block holding "num" number of elements of "size" bytes in the region reserved by InitHeap.
-----------------------------------------------------------------------------
*/
#include    <ultra64.h>
#include    "string.h"
#include    "malloc.h"
#include    "_malloc.h"

void *calloc(size_t num, size_t size)
{
    size_t i ;
    char *ret, *dmy ;

    if( (int)malloc_ptr == -1 ){
	return  NULL ;
    }

    if( (ret = _malloc(malloc_ptr, num * size)) == NULL ){
	return  NULL ;
    }

    /* Pad the reserved region with 0  */
    i = num * size;
    dmy = ret ;
    while( i-- ){
	*dmy++ = 0;
    }

    return ret ;
}
