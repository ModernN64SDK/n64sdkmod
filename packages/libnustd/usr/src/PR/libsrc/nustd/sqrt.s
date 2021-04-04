/*
=============================================================================
	Copyright (C) 1997-1999 NINTENDO Co.,Ltd.
        
        $RCSfile: sqrt.s,v $
        $Revision: 1.1 $
        $Date: 1999/04/14 07:18:41 $
=============================================================================
function name: sqrt
-----------------------------------------------------------------------------
format: 	#include <math.h>
	double sqrt(double x);
argument: 	x    original value to find square root
return value:  square root of x 
explanation: finds square root of x 
-----------------------------------------------------------------------------
*/
	.text
	.set	reorder
	.align	4
	.ent	sqrt
	.globl	sqrt
sqrt:
	sqrt.d	$f0,$f12
	jr	$31
	.end	sqrt

