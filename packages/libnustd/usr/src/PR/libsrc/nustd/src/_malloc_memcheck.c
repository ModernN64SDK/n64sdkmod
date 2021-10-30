/*
=============================================================================
        Copyright (C) 1997-1999 NINTENDO Co.,Ltd.
        
        $RCSfile: _malloc_memcheck.c,v $
        $Revision: 1.1 $
        $Date: 1999/04/30 02:11:58 $
-----------------------------------------------------------------------------
*/
#include    <ultra64.h>
#include    "string.h"
#include    "malloc.h"
#include    "_malloc.h"

int sprintf(char *s, const char *format, ...);

int _malloc_memcheck(void *ptr, struct malloc_status_st *st)
{
    int     mem = 0, free = 0, sz, h = 0;
    struct mallocST *malloc_st_ptr;
    unsigned int    *maxptr, maxsize;;

    malloc_st_ptr = ptr;
    st->useMaxMemSize = st->freeMaxMemSize = 0;
    st->useMinMemSize = st->freeMinMemSize = malloc_st_ptr->allsize;
    maxsize = malloc_st_ptr->allsize;
    if (!maxsize)   return  -1;
    maxptr = ptr;
    maxptr += maxsize/sizeof(unsigned int *);

    while(1)    {
        sz = malloc_st_ptr->size;
        if (!malloc_st_ptr->flag && sz) {
            free += sz;
            if (sz > st->freeMaxMemSize)    st->freeMaxMemSize  = sz;
            if (sz < st->freeMinMemSize)    st->freeMinMemSize  = sz;
        }   else    {
            mem += sz;
            if (sz > st->useMaxMemSize) st->useMaxMemSize  = sz;
            if (sz < st->useMinMemSize) st->useMinMemSize  = sz;
        }
        h++;
        if (!malloc_st_ptr->next)   break;
        if (malloc_st_ptr->next < (unsigned int *)ptr || malloc_st_ptr->next > maxptr
             || ((int)malloc_st_ptr->next & 3))  return  maxsize;
        malloc_st_ptr = (struct mallocST *)malloc_st_ptr->next;
    }
    st->allMemSize = mem + free + h*MALLOC_HEADSIZE;
    st->useMemSize = mem;
    st->freeMemSize = free;
    if (!mem)   st->useMinMemSize = 0;
    if (!free)  st->freeMinMemSize = 0;
    malloc_st_ptr = ptr;
    if (st->allMemSize != malloc_st_ptr->allsize)   return  maxsize;
    return  0;
}
