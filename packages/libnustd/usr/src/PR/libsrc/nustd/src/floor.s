/*
=============================================================================
	Copyright (C) 1997-1999 NINTENDO Co.,Ltd.
        
        $RCSfile: floor.s,v $
        $Revision: 1.1 $
        $Date: 1999/04/14 07:18:39 $
=============================================================================
function name: floor
-----------------------------------------------------------------------------
format: #include <math.h>
	double floor(doubel x);
argument: x   input value 
return value: maximum integer which is less than x  
explanation: finds maximum integer which is less than x  
-----------------------------------------------------------------------------
*/
	.text
	.set	reorder
	.align	4
	.ent	floor
	.globl	floor
floor:
	floor.w.d	$f14,$f12
	cvt.d.w		$f0,$f14
	jr	$31
	.end	floor

