/*
=============================================================================
		Copyright (C) 1997-1999 NINTENDO Co.,Ltd.
        
        $RCSfile: ceil.s,v $
        $Revision: 1.1 $
        $Date: 1999/04/14 07:18:39 $
=============================================================================
Function name:ceil
-----------------------------------------------------------------------------
Format:	#include <math.h>
	      double ceil(double x);
Argument:	x input value
Return value: smallest integer which is equal to or larger than x
Explanation:  smallest integer which is equal to or larger than x 
-----------------------------------------------------------------------------
*/
	.text
	.set	reorder
	.align	4
	.ent	ceil
	.globl	ceil
ceil:
	ceil.w.d	$f14, $f12
	cvt.d.w	$f0,$f14
	jr	$31
	.end	ceil
