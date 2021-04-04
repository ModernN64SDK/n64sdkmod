/*
=============================================================================
	Copyright (C) 1997-1999 NINTENDO Co.,Ltd.
        
        $RCSfile: fabs.s,v $
        $Revision: 1.1 $
        $Date: 1999/04/14 07:18:39 $
=============================================================================
function name: fabs
-----------------------------------------------------------------------------
format: #include <math.h>
	double fabs(doouble x);
argument: x value to find absolute value 
return value: absolute value of x  
explanation: finds absolute value of x  
-----------------------------------------------------------------------------
*/
	.text
	.set	reorder
	.align	4
	.ent	fabs
	.globl	fabs
fabs:
	abs.d	$f0,$f12
	jr	$31
	.end	fabs

