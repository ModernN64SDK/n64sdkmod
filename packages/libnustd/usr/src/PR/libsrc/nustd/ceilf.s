/*
=============================================================================
	Copyright (C) 1997-1999 NINTENDO Co.,Ltd.
        
        $RCSfile: ceilf.s,v $
        $Revision: 1.1 $
        $Date: 1999/04/14 07:18:39 $
=============================================================================
Function name:ceilf
-----------------------------------------------------------------------------
Format:	#include <math.h>
	      float ceilf(float x);
Argument:	x input value
Return value:   smallest integer which is equal to or larger than x
Explanation:    smallest integer which is equal to or larger than x
-----------------------------------------------------------------------------
*/
	.text
	.set	reorder
	.align	4
	.ent	ceilf
	.globl	ceilf
ceilf:
	ceil.w.s	$f14,$f12
	cvt.s.w		$f0,$f14
	jr	$31
	.end	ceilf
