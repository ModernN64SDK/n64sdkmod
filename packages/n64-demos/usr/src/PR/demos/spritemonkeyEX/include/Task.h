#ifndef _Task_h_
#define _Task_h_

#ifdef __cplusplus
extern "C" {
#endif

/* this stack size is in bytes, and is a lot larger
 * than this program needs.
 */
#define	STACKSIZE	0x2000
#define	GLIST_LEN	2048*4

#ifdef _LANGUAGE_C

#ifndef _SGI_MAKEROM_
typedef struct {
    Mtx	projection;
    Mtx	modeling;
    Mtx	viewing;
    Mtx     identity;
    Gfx	glist[GLIST_LEN];
} Dynamic;

extern Dynamic	dynamic;

/* RSP matrix stack */
extern u64  dram_stack[SP_DRAM_STACK_SIZE64];

extern void init_task( void );
extern void start_task( Gfx *glist_ptr, Dynamic *dyamic_ptr );
#endif

#endif /* _LANGUAGE_C */

#ifdef __cplusplus
}
#endif

#endif




