/*
=============================================================================
        Copyright (C) 1997-1999 NINTENDO Co.,Ltd.
        
        $RCSfile: mt_memalign.c,v $
        $Revision: 1.2 $
        $Date: 1999/05/27 07:44:21 $
=============================================================================
Function name: mt_memalign
-----------------------------------------------------------------------------
Syntax:      #include <malloc.h>
             void *mt_memalign(int alignment, size_t size);
Explanation: Multithread support version of memalign(). The arguments, returned values and other explanations are the same as for memalign(), so reference that function.
-----------------------------------------------------------------------------
*/
#include    <ultra64.h>
#include    "string.h"
#include    "malloc.h"
#include    "_malloc.h"

void *mt_memalign(int alignment, size_t size)
{
    int size2;
    unsigned int    *next2, *p, *r;
    unsigned char   *c;
    struct mallocST *malloc_st_ptr;
    OSIntMask svintmask ;

    if ((int)malloc_ptr == -1)  return  NULL;

    if (!size)  return  NULL;

    svintmask = osSetIntMask( OS_IM_NONE ) ; /* Prohibit all interrupts */

    if( alignment <= 16 ){
	/* Use r for everything */
	r = (unsigned int *)_malloc( malloc_ptr, size ) ;
	osSetIntMask( svintmask ) ; /* Reset interrupt mask  */
	return (void *)r ;
    }

    malloc_st_ptr = (struct mallocST *)malloc_ptr;
    size += 15;
    size &= (~15);

    size2 = 0;
    next2 = 0;
    while(1)    {
        if (!malloc_st_ptr->flag)   {
            c = (unsigned char *)malloc_st_ptr;
            c += (MALLOC_HEADSIZE + alignment - 1);
            c = (unsigned char *)((int)c & (~(alignment-1)));
            if (c >= (unsigned char *)malloc_st_ptr+MALLOC_HEADSIZE && c+size<(unsigned char *)malloc_st_ptr+malloc_st_ptr->size) {
                //  Is the address after alignment within the block? 
                if ((size2 > malloc_st_ptr->size) || !size2)    {
                    r = (unsigned int *)c;
                    p = (unsigned int *)malloc_st_ptr;
                    size2 = malloc_st_ptr->size;
                    next2 = malloc_st_ptr->next;
                }
            }
        }
        if (!(malloc_st_ptr->next)) break;
        malloc_st_ptr = (struct mallocST *)malloc_st_ptr->next;
    }

    if (size2)  {
        if (p != (unsigned int *)malloc_ptr)    {
            if ((int)r - (int)p<= 32)    {   //  For alignment, increase the capacity of the free memory in front if it is less than 16bytes.
                if (r-p >15)    {
                    malloc_st_ptr = (struct mallocST *)malloc_ptr;
                    while(1)    {
                        if (malloc_st_ptr->next == 0)   break;
                        if (malloc_st_ptr->next == p)   {
                            malloc_st_ptr->next = r-MALLOC_HEADSIZE;
                            malloc_st_ptr->size += 16;
                            break;
                        }
                        malloc_st_ptr = (struct mallocST *)malloc_st_ptr->next;
                    }
                }
            }   else    {           //  Create block, since the free memory in front is greater than 16bytes after alignment.
                malloc_st_ptr = (struct mallocST *)p;
                malloc_st_ptr->next = r -(MALLOC_HEADSIZE/sizeof(unsigned int *));
                malloc_st_ptr->size = (int)r - (int)p - MALLOC_HEADSIZE * 2;
                malloc_st_ptr->flag = 0;
            }
        }   else    {
            malloc_st_ptr = (struct mallocST *)p;
            malloc_st_ptr->next = r - (MALLOC_HEADSIZE / sizeof(int *));
            malloc_st_ptr->size = (r - p)*sizeof(int) - MALLOC_HEADSIZE * 2;
            malloc_st_ptr->flag = 0;
        }
        c = (char *)r;
        c -= MALLOC_HEADSIZE;
        malloc_st_ptr = (struct mallocST *)c;

        if (size2>=size+MALLOC_HEADSIZE+16) {
            malloc_st_ptr->next = (unsigned int *)((unsigned int)r + size);         /*  Reserve memory */
            malloc_st_ptr->size = size;
            malloc_st_ptr->flag = 1;

            malloc_st_ptr = (struct mallocST *)malloc_st_ptr->next;                     /*  Release the surplus memory */
            malloc_st_ptr->next = next2;
            malloc_st_ptr->size = size2 - size - (r - p) * sizeof(int);

            malloc_st_ptr->flag = 0;
        }   else    {
            malloc_st_ptr->next = next2;
            malloc_st_ptr->size = (int)(next2 - r);
            malloc_st_ptr->flag = 1;
        }
	osSetIntMask( svintmask ) ; /* Reset interrupt mask */
        return  (unsigned char *)r;
    }
    osSetIntMask( svintmask ) ; /* Reset interrupt mask */
    return  NULL;
}
