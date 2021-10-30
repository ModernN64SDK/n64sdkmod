/*
=============================================================================
	Copyright (C) 1997-1999 NINTENDO Co.,Ltd.
        
        $RCSfile: floorf.s,v $
        $Revision: 1.1 $
        $Date: 1999/04/14 07:18:39 $
=============================================================================
function name: floorf
-----------------------------------------------------------------------------
format: #include <math.h>
	float floorf(float x);
argument: x  initial value 
return value: maximum integer which is less than x  
explanation: finds maximum integer which is less than x  
-----------------------------------------------------------------------------
*/
	.text
	.set	reorder
	.align	4
	.ent	floorf
	.globl	floorf
floorf:
	floor.w.s	$f14,$f12
	cvt.s.w		$f0,$f14
	jr	$31
	.end	floorf

