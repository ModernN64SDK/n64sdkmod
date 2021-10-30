/*
=============================================================================
        Copyright (C) 1997-1999 NINTENDO Co.,Ltd.
        
        $RCSfile: _malloc_memdsp.c,v $
        $Revision: 1.1 $
        $Date: 1999/04/30 02:12:08 $
-----------------------------------------------------------------------------
*/
#include    <ultra64.h>
#include    "string.h"
#include    "malloc.h"
#include    "_malloc.h"

int sprintf(char *s, const char *format, ...);

int _malloc_memdsp(void *ptr, unsigned char *s, int size)
{
    int a;
    struct mallocST *malloc_st_ptr;
    unsigned int    *maxptr;

    malloc_st_ptr = ptr;
    maxptr = ptr;
    maxptr += (malloc_st_ptr->allsize)/sizeof(unsigned int *);

    a = 0;
    while(1)    {
        if (a/2 == size/2)  break;
        sprintf(&s[(a++)*60], "%08x:%08x-%08x", (unsigned int)(malloc_st_ptr),
		(unsigned int)(malloc_st_ptr)+MALLOC_HEADSIZE,
		(unsigned int)(malloc_st_ptr)+MALLOC_HEADSIZE
		+malloc_st_ptr->size-1);
        if( !malloc_st_ptr->flag ){
            if (malloc_st_ptr->size){
		sprintf(&s[(a++)*60], "        (%08x)[%08x] FREE",
			malloc_st_ptr->size, malloc_st_ptr->next);
            }else{
		sprintf(&s[(a++)*60], "        (%08x)[%08x] ALIN",
			malloc_st_ptr->size, malloc_st_ptr->next);
	    }
        }else{
	    sprintf(&s[(a++)*60], "        (%08x)[%08x] USE",
		    malloc_st_ptr->size, malloc_st_ptr->next);
	}
        if (!malloc_st_ptr->next)   break;
        if ((malloc_st_ptr->next < (unsigned int *)ptr)
	    || (malloc_st_ptr->next > maxptr)
	    || ((int)malloc_st_ptr->next & 3)) {
	    sprintf(&s[(a++)*60],
		    "malloc_memdsp error:%08x over area %08x-%08x",
		    malloc_st_ptr->next, ptr, maxptr);
	    sprintf(&s[(a++)*60], "");
	    return  a;
        }
        malloc_st_ptr = (struct mallocST *)malloc_st_ptr->next;
    }
    return  a;
}
