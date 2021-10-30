
#ifndef __MALLOC_H__
#define __MALLOC_H__

#ifndef _ULTRA64_H_
#include <ultra64.h>
#endif

/* ===========================================================================
	defines
 =========================================================================== */

/* User's heap memory area is 2M*/
#define MALLOC_SPACE (2048*1024/8)

/* OCTARY is a macro that does not make an 8-byte alignment but converts the byte length to an element number as an 8-byte array.
   
   ex.) OCTARY(7) = 1;
	OCTARY(8) = 1;
	OCTARY(9) = 2;
*/
#define OCTARY(len) ((((len)-1)>>3)+1)

/* ===========================================================================
	type definisions
 =========================================================================== */
typedef enum {
    MHFLG_USED = 0x01,
    MHFLG_BEFOREUSED = 0x02
} MHFLAG;

/* MEMHEADER is fixed at an 8-byte*/
typedef struct{
    u32 flag;
    u32 length;		/* Byte length including MEMHEADER */
} MEMHEADER;

/* MEMCTRL must make an 8-byte boundary */
typedef struct{
    MEMHEADER *topadrs;
    MEMHEADER *endadrs;
    MEMHEADER *searchtop;
} MEMCTRL;

/* ===========================================================================
	public functions
 =========================================================================== */
void minit(void);
void *amalloc(int want, void *ap);
void afree( void *ptr, void *ap );
void *malloc(int want);
void free( void *ptr );
void *acreate(int want);
void adelete(void *ap);

#endif   /* #ifndef __MALLOC_H__ */
