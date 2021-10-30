#ifndef ___MALLOC_H__
#define ___MALLOC_H__

#include "malloc.h"

#define MALLOC_HEADSIZE     (((sizeof(struct mallocST))*2-1)&(~15))

struct  mallocST    {
    unsigned int    *next;          //  Next memory pointer
    unsigned int    size;           //  Current size of memory space
    unsigned int    flag;           //  Flag (0: not in use  1: in use)
    unsigned int    allsize;        //  Total size (but only valid for the first block)
} ;

extern char *malloc_ptr ;

int _free( void *msp, void *ptr ) ;
void *_malloc( void *ptr, int size ) ;
int _malloc_memdsp( void *ptr, unsigned char *s, int size ) ;
int _malloc_memcheck( void *ptr, struct malloc_status_st *st ) ;

#endif /* ___MALLOC_H__ */
