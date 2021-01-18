/*---------------------------------------------------------------------*
	Copyright (C) 1997, Nintendo.
	
	File		dirtydcache16.s
	Coded    by	Yoshitaka Yasumoto.	Nov 25, 1997.
	
	$Id: dirtydcache16.s,v 1.2 1997/11/25 11:22:19 yasu Exp $
 *---------------------------------------------------------------------*/
#include	"dirtydcache.h"

/*---------------------------------------------------------------------------*
 *  osDirtyDCache16(void *addr)	addr==>a0
 *  osDirtyDCache32(void *addr)
 *  osDirtyDCache40(void *addr)
 *  osDirtyDCache48(void *addr)
 *  osDirtyDCache64(void *addr)
 *  osDirtyDCache80(void *addr)
 *---------------------------------------------------------------------------*/
	.globl	osDirtyDCache80
	.globl	osDirtyDCache64
	.globl	osDirtyDCache48
	.globl	osDirtyDCache40
	.globl	osDirtyDCache32
	.globl	osDirtyDCache16
	.set	noreorder
osDirtyDCache80:	
	cache	CACHE_OP, 4*DCACHE_LINESIZE+8(a0)
osDirtyDCache64:	
	cache	CACHE_OP, 3*DCACHE_LINESIZE+8(a0)
osDirtyDCache48:	
	cache	CACHE_OP, 2*DCACHE_LINESIZE+8(a0)
osDirtyDCache40:
	cache	CACHE_OP, 2*DCACHE_LINESIZE+0(a0)
osDirtyDCache32:
	cache	CACHE_OP, 1*DCACHE_LINESIZE+8(a0)
osDirtyDCache16:
	jr	ra
	cache	CACHE_OP, 0*DCACHE_LINESIZE+8(a0)
	.set	reorder
	
/*======== End of dirtydcache16.s ========*/
