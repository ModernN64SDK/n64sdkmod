#include <ultra64.h>

#if !defined(_SIZE_T) && !defined(_SIZE_T_)
#define _SIZE_T
#if (_MIPS_SZLONG == 32)
typedef unsigned int size_t;
#endif
#if (_MIPS_SZLONG == 64)
typedef unsigned long size_t;
#endif
#endif /* !_SIZE_T */

typedef void (*FuncPtr)(void *, int, int, int, int, int, int, int, int);
typedef void (*FuncPtrI)(void *, int ...);

#define BLOCK_MARK 0x7373
#define ALIGN_UP(x) ((x + 0xf) & ~0xf)
#define ALIGN_DOWN(x) (x & 0xf)

typedef struct OSMemBlock_s {
    s16         mark;                   /* magic cookie */
    s16         free;
    s32         size;
    struct      OSMemBlock_s *next;     /* pointer to the next header   */
    struct      OSMemBlock_s *prev;
} OSMemBlock;

typedef struct {
    OSMemBlock *start;  /* list of allocated blocks     */
} OSArena;

OSArena arena;

void __osMallocInit(OSArena *arena, u8 *base, s32 len) 
{
    OSMemBlock *block;
    u8 * newbase;
    
    /* crop to cache line boundaries */
    newbase = (u8 *)ALIGN_UP((u32)base);
    len    -= (newbase - base);
    len     = ALIGN_DOWN(len);    

    /* create one big free block */
    block = (OSMemBlock *) newbase;
    block->next = 0;
    block->prev = 0;
    block->size = len - sizeof(OSMemBlock);
    block->free = TRUE;
    block->mark = BLOCK_MARK;
    
    /* assign it to the arena */
    arena->start = block;
}

void *__osMalloc(OSArena *arena, size_t size) 
{
    OSMemBlock *block;
    OSMemBlock *nextBlock;
    u8 *ptr = 0;
    
    /* round up to next cache line size */
    size = ALIGN_UP(size);
    
    for (block = arena->start; block != 0; block = block->next) {

        if (block->free && (block->size > (size + sizeof(OSMemBlock)))) {

            /* initialize the block and create the next free block */
            nextBlock = (OSMemBlock *)((u8 *)block+sizeof(OSMemBlock)+size);
            nextBlock->next = block->next;
            nextBlock->prev = block;
            nextBlock->size = block->size  - sizeof(OSMemBlock) - size;
            nextBlock->free = TRUE;
            nextBlock->mark = BLOCK_MARK;
            
            block->next = nextBlock;   /* prev doesn't change */
            block->size = size;
            block->free = FALSE;
            ptr = (u8 *)block + sizeof(OSMemBlock);
            break;
        }
    }

    return ptr;    
}

void __osFree(OSArena *, void *ptr) 
{
    OSMemBlock *block = (OSMemBlock *)((u8 *)ptr - sizeof(OSMemBlock));

    if ((block->mark == BLOCK_MARK) && (block->free == FALSE)) {

        block->free = TRUE;

        if (block->next) {
            if (block->next->free) {
                block->size += block->next->size + sizeof(OSMemBlock);
                block->next = block->next->next;
                if (block->next)
                    block->next->prev = block;
            }
        }

        if (block->prev) {
            if (block->prev->free) {
                block->prev->size += block->size + sizeof(OSMemBlock);
                block->prev->next = block->next;
                if (block->next)
                    block->next->prev = block->prev;                
            }
        }
    }    
}

extern void* operator new(size_t size)
{
    OSIntMask mask = osSetIntMask(OS_IM_NONE);

    if (size == 0) {
	size = 1;
    }

    void *ptr = __osMalloc(&arena, size);

    osSetIntMask(mask);

    return ptr;
    
}

extern void operator delete(void *ptr)
{
    OSIntMask mask = osSetIntMask(OS_IM_NONE);

    if (ptr) {
	__osFree(&arena, ptr);
    }

    osSetIntMask(mask);

}

extern "C" void __pure_virtual_called() { }

typedef void (*constructor)(void *);
typedef void (*destructor)(void *,int);

extern "C" void __vec_ct(void *vec, int n, int size, void *f)
{
    OSIntMask mask = osSetIntMask(OS_IM_NONE);
    
    char *maximum = (char *)vec + n * size;
    constructor cons = constructor(f);
    for (char *p = (char *)vec; p < maximum; p += size) {
	(*cons)(p);
    }

    osSetIntMask(mask);
}

extern "C" void __vec_dt(void *vec, int n, int size, void *f)
{
    OSIntMask mask = osSetIntMask(OS_IM_NONE);
    
    char *maximum = (char *)vec + n * size;
    destructor des = destructor(f);
    for (char *p = (char *)vec; p < maximum; p += size) {
	(*des)(p, 2);
    }

    osSetIntMask(mask);
}

extern "C" void *__vec_new(void *vec, int n, int size, void *f)
{
    OSIntMask mask = osSetIntMask(OS_IM_NONE);

    if (vec == 0)
	vec = (void *)( new char[n*size] );
    if (vec && f) {
	register char* p = (char*) vec;
	register char* maximum = p + n*size;
	register FuncPtr fp = FuncPtr(f);
        while (p < maximum) {
            (*fp) ((void *)p,0,0,0,0,0,0,0,0);
            p += size;
        }
    }
    osSetIntMask(mask);

    return ((void *)vec);
}

extern "C" void __vec_delete(void * vec, int n, int size, void * f,
			     int del, int)
{
    OSIntMask mask = osSetIntMask(OS_IM_NONE);

    if (vec) {
	if (f) {
	    register char* cp = (char*) vec;
	    register char* p = cp;
	    register FuncPtrI fp = FuncPtrI(f);
	    p += n*size;
	    while (p > cp) {
		p -= size;
		(*fp)((void *) p, 2);
	    }
	}
	if (del) delete vec;
    }
    osSetIntMask(mask);    
}

/*  
 * List for objects that needs to be globally constructed and
 * destructed This is needed for global object construction in the
 * static share library
 */

typedef	void (*CTOR_DTOR_PTR)(void);

struct __linkl {
	struct __linkl 	*next;
	CTOR_DTOR_PTR	ctor;
	CTOR_DTOR_PTR	dtor;
};

/* head of the list of global objects */

static struct __linkl *__head = 0;

extern "C" void
__CallLibGlobalCtors( void )
{
    struct __linkl	*ptr = (struct __linkl *)&__head;
    struct __linkl	*previous = 0;
    long                offset = (long)__head;

    while (offset) {
        ptr = (struct __linkl *)((long)ptr + offset);
        if (ptr->ctor) {
            (*(ptr->ctor))();
        }

        // 
        // reverse the order of the list so that the destrusters are
        // called in reverse order
        //

        offset      = (long)ptr->next;
        ptr->next   = previous;
        previous    = ptr;
    }

    /* head points to the last object */

    __head = previous;
}

extern "C" void
osInitializeCPP( u8 *base, s32 len ) 
{
    __osMallocInit(&arena, base, len);
    __CallLibGlobalCtors();
}

extern "C"
void _main ()
{
    osSyncPrintf("_main\n");
}

extern "C" void _fini ()
{
    osSyncPrintf("_fini\n");
}
