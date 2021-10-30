/*
=============================================================================
	Copyright (C) 1997-1999 NINTENDO Co.,Ltd.
        
        $RCSfile: fabsf.s,v $
        $Revision: 1.1 $
        $Date: 1999/04/14 07:18:39 $
=============================================================================
function name: fabsf
-----------------------------------------------------------------------------
format: #include <math.h>
	float fabsf(float x);
argument: x value to find absolute value 
return value: absolute value of x  
explanation: finds absolute value of x  
-----------------------------------------------------------------------------
*/
	.text
	.set	reorder
	.align	4
	.ent	fabsf
	.globl	fabsf
fabsf:
	abs.s	$f0,$f12
	jr	$31
	.end	fabsf

