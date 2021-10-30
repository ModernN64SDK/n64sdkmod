/*
=============================================================================
        Copyright (C) 1997-1999 NINTENDO Co.,Ltd.
        
        $RCSfile: mt_realloc.c,v $
        $Revision: 1.3 $
        $Date: 1999/05/27 07:44:21 $
=============================================================================
Function name: mt_realloc
-----------------------------------------------------------------------------
Syntax:      #include <malloc.h>
             void *mt_realloc(void *ptr, int size);
Explanation: Multithread support version of realloc(). The arguments, returned values and other explanations are the same as for realloc(), so reference that function.

-----------------------------------------------------------------------------
*/
#include    <ultra64.h>
#include    "string.h"
#include    "malloc.h"
#include    "_malloc.h"

void *mt_realloc(void *ptr, int size)
{
    unsigned int    *next2;
    unsigned int    size2;
    unsigned char   *p2;
    struct  mallocST  *malloc_st_ptr ;
    OSIntMask svintmask ;
    void *ret ;

    if( (int)malloc_ptr == -1 ){
	return  NULL;
    }

    svintmask = osSetIntMask( OS_IM_NONE ) ; /* Prohibit all interrupts */

    if( !ptr ){
	ret = _malloc( malloc_ptr, size ); /*  Newly reserve memory */
	osSetIntMask( svintmask ) ; /* Reset interrupt mask */
	return ret ;
    }

    if( !size ){ /* Release current memory */
        _free(malloc_ptr, ptr);
	osSetIntMask( svintmask ) ; /* Reset interrupt mask */
        return  NULL;
    }

    size += 15;
    size &= (~15);

    p2 = (unsigned char *)ptr - MALLOC_HEADSIZE;
    malloc_st_ptr = (struct mallocST *)p2;

    if( !malloc_st_ptr->flag ){
	osSetIntMask( svintmask ) ; /* Reset interrupt mask */
	return NULL;
    }

    next2 = malloc_st_ptr->next;
    size2 = malloc_st_ptr->size;

    malloc_st_ptr = (struct mallocST *)next2;
    if (next2 && !malloc_st_ptr->flag)  {
        size2 += malloc_st_ptr->size + MALLOC_HEADSIZE;
        next2 = (unsigned int *)malloc_st_ptr->next;
    }

    malloc_st_ptr = (struct mallocST *)p2;
    if( size > size2 ){
        /*  Cannot reserve specified size from location of current pointer */
        p2 = (unsigned char *)_malloc(malloc_ptr, size);
        if( p2 == NULL ){
	    osSetIntMask( svintmask ) ; /* Reset interrupt mask  */
	    return  NULL;
	}
        _nmemcpy(p2, ptr, size);
        _free(malloc_ptr, ptr);
	osSetIntMask( svintmask ) ; /* Reset interrupt mask  */
        return  p2;
    }else{
        /*  Change size while continuing to use current pointer */
        if( (size + MALLOC_HEADSIZE + 16) < size2 ){
	    /* If it is necessary to repartition blocks */
	    /*
            malloc_st_ptr->next
	      = (unsigned int *)((unsigned int)ptr + size + MALLOC_HEADSIZE);
	      */
            malloc_st_ptr->next
	      = (unsigned int *)((unsigned int)ptr + size) ;
            malloc_st_ptr->size = size;
    
            malloc_st_ptr = (struct mallocST *)malloc_st_ptr->next;
            malloc_st_ptr->next = next2;
            malloc_st_ptr->size = size2 - size - MALLOC_HEADSIZE;
            malloc_st_ptr->flag = 0;
        }else{
	    /* If it is not necessary to repartition blocks (i.e., if the size of the block is the same as the size intended to be reserved) */
            malloc_st_ptr->next = next2;
            malloc_st_ptr->size = size2;
        }
	osSetIntMask( svintmask ) ; /* Reset interrupt mask */
        return  ptr;
    }
}
