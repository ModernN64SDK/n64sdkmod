/*---------------------------------------------------------------------
 *	Copyright (C) 1997, Nintendo.
 *	
 *	File		dirtydcache.s
 *	Coded    by	Yoshitaka Yasumoto.	Nov 25, 1997.
 *	
 *	$Id: dirtydcache.s,v 1.2 1997/11/25 11:22:18 yasu Exp $
 *---------------------------------------------------------------------*/
#include	"dirtydcache.h"
	
/*---------------------------------------------------------------------------*
 *  osDirtyDCache(void *addr, int size)	addr==>a0, size==>a1
 *---------------------------------------------------------------------------*/
	.globl	osDirtyDCache
	.set	noreorder
1:	cache	CACHE_OP, 8-DCACHE_LINESIZE*4(a0)
	cache	CACHE_OP, 8-DCACHE_LINESIZE*3(a0)
	cache	CACHE_OP, 8-DCACHE_LINESIZE*2(a0)
	cache	CACHE_OP, 8-DCACHE_LINESIZE*1(a0)
osDirtyDCache:	
	addi	a1, a1,-DCACHE_LINESIZE*4
	bgez	a1, 1b
	addi	a0, a0, DCACHE_LINESIZE*4
	addi	a1, a1, DCACHE_LINESIZE*4
	blez	a1, 4f
	addi	a1, a1,-DCACHE_LINESIZE
	blez	a1, 3f
	addi	a1, a1,-DCACHE_LINESIZE
	blez	a1, 2f
	nop
	cache	CACHE_OP, 8-DCACHE_LINESIZE*2(a0)
2:	cache	CACHE_OP, 8-DCACHE_LINESIZE*3(a0)
3:	cache	CACHE_OP, 8-DCACHE_LINESIZE*4(a0)
4:	jr	ra
	nop
	.set	reorder
	
/*======== End of dirtydcache.s ========*/
