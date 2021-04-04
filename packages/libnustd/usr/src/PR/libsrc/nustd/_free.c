#include <ultra64.h>
#include "string.h"
#include "_malloc.h"

int _free(void *msp, void *ptr)
{
    unsigned int    *next2;
    unsigned int    size2;
    unsigned char   *p2;
    struct mallocST *malloc_st_ptr;

    if (!ptr)   return 1;
    p2 = (unsigned char *)ptr - MALLOC_HEADSIZE;
    malloc_st_ptr = (struct mallocST *)p2;
    if (!malloc_st_ptr->flag)   return 1;               //  Released from the beginning

    next2 = malloc_st_ptr->next;                        /*  Added process if next memory block is not used */
    size2 = malloc_st_ptr->size;
    malloc_st_ptr = (struct mallocST *)next2;
    if (next2 && !malloc_st_ptr->flag)  {
        size2 += malloc_st_ptr->size + MALLOC_HEADSIZE;
        next2 = (unsigned int *)malloc_st_ptr->next;
    }

    malloc_st_ptr = msp;
    while(1)    {
        if ((unsigned char *)malloc_st_ptr->next == p2) {
            if (!malloc_st_ptr->flag)   {
                p2 = (unsigned char *)malloc_st_ptr;
                size2 += malloc_st_ptr->size + MALLOC_HEADSIZE;
            }
            break;
        }
        if ((unsigned char *)malloc_st_ptr > p2)    break;
        if (!malloc_st_ptr->next)       break;
        malloc_st_ptr = (struct mallocST *)malloc_st_ptr->next;
    }
    malloc_st_ptr = (struct mallocST *)p2;              /*  Release memory */
    malloc_st_ptr->next = next2;
    malloc_st_ptr->size = size2;
    malloc_st_ptr->flag = 0;
    return  0;
}
