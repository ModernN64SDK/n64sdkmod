/*
=============================================================================
        Copyright (C) 1997-1999 NINTENDO Co.,Ltd.
        
        $RCSfile: mt_calloc.c,v $
        $Revision: 1.2 $
        $Date: 1999/05/27 07:44:20 $
=============================================================================
Function name: mt_calloc
-----------------------------------------------------------------------------
Syntax:       #include <malloc.h>
              void *mt_calloc(size_t num, size_t size);
Explanation:  Multithread support version of calloc().  The arguments, returned values and other explanations are the same as for calloc(), so please reference that function for details.
-----------------------------------------------------------------------------
*/
#include    <ultra64.h>
#include    "string.h"
#include    "malloc.h"
#include    "_malloc.h"

void *mt_calloc(size_t num, size_t size)
{
    size_t i ;
    char *ret, *dmy ;
    OSIntMask svintmask ;

    if( (int)malloc_ptr == -1 ){
	return  NULL ;
    }

    svintmask = osSetIntMask( OS_IM_NONE ) ; /* Prohibit all interrupts */

    if( (ret = _malloc(malloc_ptr, num * size)) == NULL ){
	osSetIntMask( svintmask ) ; /* Reset interrupt mask */
	return  NULL;
    }

    /* Pad the reserved region with 0 */
    i = num * size ;
    dmy = ret ;
    while( i-- ){
	*dmy++ = 0 ;
    }

    osSetIntMask( svintmask ) ; /* Reset the interrupt mask  */

    return ret ;
}
