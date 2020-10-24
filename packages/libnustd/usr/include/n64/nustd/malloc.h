/*---------------------------------------------------------------------*
        Copyright (C) 1997-1999 NINTENDO Co.,Ltd.
        
        $RCSfile: malloc.h,v $
        $Revision: 1.2 $
        $Date: 1999/04/30 02:04:19 $
 *---------------------------------------------------------------------*/
#ifndef __MALLOC_H__
#define __MALLOC_H__

#ifdef _LANGUAGE_C_PLUS_PLUS
extern "C" {
#endif

struct  malloc_status_st    {
    int     allMemSize;     /*  all the memory size		    */

    int     useMemSize;     /*  used memory size        	    */
    int     useMaxMemSize;  /*  used maximum memory size        */
    int     useMinMemSize;  /*  used minimum memory size        */

    int     freeMemSize;    /*  free memory size                */
    int     freeMaxMemSize; /*  free maximum memory size        */
    int     freeMinMemSize; /*  free minimum memory size        */
};

int InitHeap(void *head, unsigned int  size);

int malloc_memcheck(struct malloc_status_st *st);
int malloc_memdsp(unsigned char *s, int size);
void *malloc(int size);
void free(void *ptr);
void *realloc(void *ptr, int size);
void *calloc(size_t num, size_t size);
void *memalign(int alignment, size_t size);

int mt_malloc_memcheck(struct malloc_status_st *st);
int mt_malloc_memdsp(unsigned char *s, int size);
void *mt_malloc(int size);
void mt_free(void *ptr);
void *mt_realloc(void *ptr, int size);
void *mt_calloc(size_t num, size_t size);
void *mt_memalign(int alignment, size_t size);

#ifdef _LANGUAGE_C_PLUS_PLUS
}
#endif /* _LANGUAGE_C_PLUS_PLUS */

#endif /* __MALLOC_H__ */
