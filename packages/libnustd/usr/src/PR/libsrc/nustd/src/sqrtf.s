/*
=============================================================================
        Copyright (C) 1997-1999 NINTENDO Co.,Ltd.
        
        $RCSfile: sqrtf.s,v $
        $Revision: 1.2 $
        $Date: 1999/04/30 10:43:16 $
=============================================================================
Function name: _nsqrtf
-----------------------------------------------------------------------------
Format: #include <math.h>
	float _nsqrt(float x);
Argument:	x original value to find square root  
return value:  square root of x 
explanation:   finds square root of x  
-----------------------------------------------------------------------------
*/
	.text
	.set	reorder
	.align	4
	.ent	_nsqrtf
	.globl	_nsqrtf
_nsqrtf:
	sqrt.s	$f0,$f12
	jr	$31
	.end	_nsqrtf
