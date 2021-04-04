#include <ultra64.h>
#include "string.h"
#include "_malloc.h"

void *_malloc(void *ptr, int size)
{
    unsigned int    size2, sizeall;
    unsigned int    *next2, *p;
    struct mallocST *malloc_st_ptr;

    if (!size)  return NULL; /* Cannot reserve when size is 0 */

    malloc_st_ptr = ptr;

    /* The size is aligned to 16, and header size is added */
    size += 15;
    size &= (~15);
    sizeall = size + MALLOC_HEADSIZE;

    size2 = 0 ;
    next2 = NULL ;

    /* Search for free block that is larger than "size" */
    while(1){
        if( !malloc_st_ptr->flag && (malloc_st_ptr->size >= size) ){
	    /* Search for block that is closest in size to "size" */
            if( (size2 > malloc_st_ptr->size) || !size2 ){
                p = (unsigned int *)malloc_st_ptr;
                size2 = malloc_st_ptr->size;
                next2 = malloc_st_ptr->next;
            }
        }
        if( !(malloc_st_ptr->next) ){
	    break;
	}
        malloc_st_ptr = (struct mallocST *)malloc_st_ptr->next;
    }

    if( size2 ){
        malloc_st_ptr = (struct mallocST *)p;
        if( size2 >= (size + MALLOC_HEADSIZE + 16) ){
            malloc_st_ptr->next
	      = (unsigned int *)((unsigned int)p + sizeall); /* Reserve memory */
            malloc_st_ptr->size = size;
            malloc_st_ptr->flag = 1;

            malloc_st_ptr
	      = (struct mallocST *)malloc_st_ptr->next; /* Release the surplus memory */
            malloc_st_ptr->next = next2;
            malloc_st_ptr->size
	      = size2 - sizeall; /* sizeall = (size + MALLOC_HEADSIZE) */
            malloc_st_ptr->flag = 0;
        }else{
            malloc_st_ptr->next = next2;
            malloc_st_ptr->size = size2;
            malloc_st_ptr->flag = 1;
        }
        return  (unsigned char *)p + MALLOC_HEADSIZE ;
    }
    return  NULL;
}
