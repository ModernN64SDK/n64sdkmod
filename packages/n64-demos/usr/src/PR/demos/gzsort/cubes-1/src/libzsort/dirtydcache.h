/*---------------------------------------------------------------------*
	Copyright (C) 1997, Nintendo.
	
	File		dirtydcache.h
	Coded    by	Yoshitaka Yasumoto.	Nov 25, 1997.
	
	$Id: dirtydcache.h,v 1.1 1997/11/25 11:22:17 yasu Exp $
 *---------------------------------------------------------------------*/
#define	DCACHE_LINESIZE		16
#define	DCACHE_PRIM		0x01	/* Primary D Cache        */
#define	CREATE_DIRTY_EXCLUSIVE	0x0c	/* Create dirty exclusive */
#define	CACHE_OP		DCACHE_PRIM|CREATE_DIRTY_EXCLUSIVE

#define	zero	$0
#define	a0	$4
#define	a1	$5
#define	t0	$8
#define	t1	$9
#define	ra	$31

/*======== End of dirtydcache.h ========*/
