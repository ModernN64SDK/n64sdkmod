/*
=============================================================================
	Copyright (C) 1997-1999 NINTENDO Co.,Ltd.
        
        $RCSfile: setjmp.s,v $
        $Revision: 1.1 $
        $Date: 1999/04/14 07:18:41 $
=============================================================================
function name: setjmp
-----------------------------------------------------------------------------
format: 	#include <setjmp.h>
	int setjmp(jmp_buf env);
argument: 	env    stack environment buffer   
return value: At default,().  When returning from longjmp function, it is a value of argument value of longjmp function. 
explanation: reserves a stack environment to return by longjmp function. 
=============================================================================
function name: longjmp
-----------------------------------------------------------------------------
format: 	#include <setjmp.h>
	void longjmp(jmp_buf env, int value);
argument: 	env    stack environment buffer   
	value    result of setjmp function (if () is specified, it will be 1) 
return value: none
explanation:  return to a function registered by setjmp function  
	argument value will become a result of setjmp function  
-----------------------------------------------------------------------------
*/
	.text
	.set	reorder

	.align	4
	.ent	setjmp
	.globl	setjmp
setjmp:
	sw	$16,0x00($4)
	sw	$17,0x04($4)
	sw	$18,0x08($4)
	sw	$19,0x0c($4)
	sw	$20,0x10($4)
	sw	$21,0x14($4)
	sw	$22,0x18($4)
	sw	$23,0x1c($4)
	sw	$29,0x20($4)
	sw	$30,0x24($4)
	sw	$31,0x28($4)
	move	$2,$0
	jr	$31
	.end	setjmp


	.text
	.set	reorder

	.align	4
	.ent	longjmp
	.globl	longjmp
longjmp:
	lw	$16,0x00($4)
	lw	$17,0x04($4)
	lw	$18,0x08($4)
	lw	$19,0x0c($4)
	lw	$20,0x10($4)
	lw	$21,0x14($4)
	lw	$22,0x18($4)
	lw	$23,0x1c($4)
	lw	$29,0x20($4)
	lw	$30,0x24($4)
	lw	$31,0x28($4)
	move	$2,$5
	bne	$2,$0,.ZERO
	li	$2,1
.ZERO:
	jr	$31
	.end	longjmp

