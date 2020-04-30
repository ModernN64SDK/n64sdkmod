/*
	for malloc,realloc,calloc,free
*/


#ifndef _SIZE_T_DEF
#define _SIZE_T_DEF
typedef unsigned size_t;
#endif

struct mem_hdr {
    size_t	size;		/* bit0==1 free block */
    struct mem_hdr *prev;
    struct mem_hdr *next;
};

#define MH_SIZE		0x10l	/* hrader size (boundary) 16byte */

void _ins_free(struct mem_hdr *,size_t,struct mem_hdr *,struct mem_hdr *);
