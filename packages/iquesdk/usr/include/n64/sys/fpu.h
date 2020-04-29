#ifndef __SYS_FPU_H__
#define __SYS_FPU_H__

/**************************************************************************
 *									  *
 * 		 Copyright (C) 1990-1992 Silicon Graphics, Inc.		  *
 *									  *
 *  These coded instructions, statements, and computer programs  contain  *
 *  unpublished  proprietary  information of Silicon Graphics, Inc., and  *
 *  are protected by Federal copyright law.  They  may  not be disclosed  *
 *  to  third  parties  or copied or duplicated in any form, in whole or  *
 *  in part, without the prior written consent of Silicon Graphics, Inc.  *
 *									  *
 **************************************************************************/

/* ------------------------------------------------------------------ */
/* | Copyright Unpublished, MIPS Computer Systems, Inc.  All Rights | */
/* | Reserved.  This software contains proprietary and confidential | */
/* | information of MIPS and its suppliers.  Use, disclosure or     | */
/* | reproduction is prohibited without the prior express written   | */
/* | consent of MIPS.                                               | */
/* ------------------------------------------------------------------ */
/* %Q% %I% %M% */

#ident "$Revision: 1.1 $"

/*
 * fpu.h -- fpu specific defines
 */

#ifdef _LANGUAGE_C
/*
 * Structure and constant definisions for floating-point instructions.
 */
union fpu_instr {
	unsigned int instr;

	struct {
#ifdef _MIPSEB
		unsigned op : 6;
		unsigned base : 5;
		unsigned rt : 5;
		signed immed : 16;
#endif /* _MIPSEB */
#ifdef _MIPSEL
		signed immed : 16;
		unsigned rt : 5;
		unsigned base : 5;
		unsigned op : 6;
#endif /* _MIPSEL */
	} itype;

	struct {
#ifdef _MIPSEB
		unsigned op : 6;
		unsigned sub : 5;
		unsigned rt : 5;
		unsigned rs : 5;
		unsigned : 11;
#endif /* _MIPSEB */
#ifdef _MIPSEL
		unsigned : 11;
		unsigned rs : 5;
		unsigned rt : 5;
		unsigned sub : 5;
		unsigned op : 6;
#endif /* _MIPSEL */
	} mtype;

	struct {
#ifdef _MIPSEB
		unsigned op : 6;
		unsigned one : 1;
		unsigned fmt : 4;
		unsigned rt : 5;
		unsigned rs : 5;
		unsigned rd : 5;
		unsigned func : 6;
#endif /* _MIPSEB */
#ifdef _MIPSEL
		unsigned func : 6;
		unsigned rd : 5;
		unsigned rs : 5;
		unsigned rt : 5;
		unsigned fmt : 4;
		unsigned one : 1;
		unsigned op : 6;
#endif /* _MIPSEL */
	} rtype;

	struct {
#ifdef _MIPSEB
		unsigned op : 6;
		unsigned rr : 5;
		unsigned rt : 5;
		unsigned rs : 5;
		unsigned rd : 5;
		unsigned func : 3;
		unsigned fmt : 3;
#endif /* _MIPSEB */
#ifdef _MIPSEL
		unsigned fmt : 3;
		unsigned func : 3;
		unsigned rd : 5;
		unsigned rs : 5;
		unsigned rt : 5;
		unsigned rr : 5;
		unsigned op : 6;
#endif /* _MIPSEL */
	} r4type;
};
#endif /* _LANGUAGE_C */

#define FMT_SINGLE	0
#define FMT_DOUBLE	1
#define FMT_EXTENDED	2
#define FMT_QUAD	3
#define FMT_WORD	4
#define FMT_LONGWORD	5
#define FMT_MAX		5

#define FUNC_ADD	0
#define FUNC_SUB	1
#define FUNC_MUL	2
#define FUNC_DIV	3

#define	FUNC_SQRT	4
#define	FUNC_ABS	5
#define	FUNC_MOV	6
#define FUNC_NEG	7

#define FUNC_ROUNDL	0x08
#define FUNC_TRUNCL	0x09
#define FUNC_CEILL	0x0a
#define FUNC_FLOORL	0x0b

#define FUNC_ROUND	0x0c
#define FUNC_TRUNC	0x0d
#define FUNC_CEIL	0x0e
#define FUNC_FLOOR	0x0f

#define FUNC_RECIP	0x19
#define FUNC_RSQRT	0x1a

#define FUNC_CVTS	0x20
#define FUNC_CVTD	0x21
#define FUNC_CVTE	0x22
#define FUNC_CVTQ	0x23
#define FUNC_CVTW	0x24
#define FUNC_CVTL	0x25

/*
 * op codes for madd, msub, nmadd, nmsub; notice that these are
 * the same as some of the other floating point op codes, however
 * they are COP1X functions.  To distinguish them in code, do
 * something like adding 64 to the op code.  Then you can do
 * a switch statement with cases like
 * case FUNC_MADD+64:
 */

#define FUNC_MADD	0x4
#define FUNC_MSUB	0x5
#define FUNC_NMADD	0x6
#define FUNC_NMSUB	0x7

/*
 * The func field of floating-point compare opcodes are FUNC_FC with
 * some combination of conditions (COND_*).
 */
#define FUNC_FC		0x30
#define COND_UN_MASK	0x1
#define COND_EQ_MASK	0x2
#define COND_LT_MASK	0x4
#define COND_IN_MASK	0x8

/*
 * The _MASK's are used to get a the specified field after it has been
 * shifted by _SHIFT and then bit patterns above can be used to test
 * the field.  These are useful in assembly code.
 */
#define	FPU_BASE_SHIFT	21
#define	FPU_BASE_MASK	0x1f
#define	FPU_FMT_SHIFT	21
#define	FPU_FMT_MASK	0xf
#define	FPU_RT_SHIFT	16
#define	FPU_RT_MASK	0x1f
#define	FPU_RT_FPRMASK	0x1e
#define	FPU_RS_SHIFT	11
#define	FPU_RS_MASK	0x1f
#define	FPU_RS_FPRMASK	0x1e
#define	FPU_RD_SHIFT	6
#define	FPU_RD_MASK	0x1f
#define	FPU_RD_FPRMASK	0x1e

#ifdef _LANGUAGE_C
/*
 * These functions are used to get and set the floating-point control
 * registers.  They are defined in the library module fp_control.s
 */
extern unsigned int get_fpc_csr(void);
extern unsigned int set_fpc_csr(unsigned int);
extern unsigned int get_fpc_irr(void);
extern unsigned int get_fpc_eir(void);

/*
 * Structure and constant definisions for the floating-point control
 * implementation and revision register (fpc_irr).
 */
union fpc_irr {
	unsigned int fi_word;
	struct {
#ifdef _MIPSEB
		unsigned reserved : 16;
		unsigned implementation : 8;
		unsigned revision : 8;
#endif /* _MIPSEB */
#ifdef _MIPSEL
		unsigned revision : 8;
		unsigned implementation : 8;
		unsigned reserved : 16;
#endif /* _MIPSEL */
	} fi_struct;
};
#endif /* _LANGUAGE_C */

/*
 * Constants for the implementation fieid of the fpc_irr structure for the
 * known implementations.
 */
#define IMPLEMENTATION_NONE	0	/* software */
#define IMPLEMENTATION_R2360	1	/* board */
#define IMPLEMENTATION_R2010	2	/* chip */

/*
 * The constant IRR_IMP_MASK is to get at the implementation fieid of the
 * implementation and revision register.  The other constants (IRR_IMP_*)
 * are constants for the known implementations.
 */
#define IRR_IMP_MASK		0x0000ff00

#define IRR_IMP_NONE		0x00000000
#define IRR_IMP_R2360		0x00000100
#define IRR_IMP_R2010		0x00000200

/*
 * Structure and constant definisions for the floating-point control
 * control and status register (fpc_csr).
 */
#ifdef _LANGUAGE_C
union fpc_csr {
	unsigned int fc_word;
	struct {
#ifdef _MIPSEB
		unsigned fcc : 7;	/* only used for mips4 */
		unsigned flush : 1;
		unsigned condition : 1;
		unsigned reserved1 : 5;

		unsigned ex_unimplemented : 1;
		unsigned ex_invalid : 1;
		unsigned ex_divide0 : 1;
		unsigned ex_overflow : 1;
		unsigned ex_underflow : 1;
		unsigned ex_inexact : 1;

		unsigned en_invalid : 1;
		unsigned en_divide0 : 1;
		unsigned en_overflow : 1;
		unsigned en_underflow : 1;
		unsigned en_inexact : 1;

		unsigned se_invalid : 1;
		unsigned se_divide0 : 1;
		unsigned se_overflow : 1;
		unsigned se_underflow : 1;
		unsigned se_inexact : 1;

		unsigned rounding_mode : 2;
#endif /* _MIPSEB */
#ifdef _MIPSEL
		unsigned rounding_mode : 2;

		unsigned se_inexact : 1;
		unsigned se_underflow : 1;
		unsigned se_overflow : 1;
		unsigned se_divide0 : 1;
		unsigned se_invalid : 1;

		unsigned en_inexact : 1;
		unsigned en_underflow : 1;
		unsigned en_overflow : 1;
		unsigned en_divide0 : 1;
		unsigned en_invalid : 1;

		unsigned ex_inexact : 1;
		unsigned ex_underflow : 1;
		unsigned ex_overflow : 1;
		unsigned ex_divide0 : 1;
		unsigned ex_invalid : 1;
		unsigned ex_unimplemented : 1;

		unsigned reserved1 : 5;
		unsigned condition : 1;
		unsigned flush : 1;
		unsigned fcc : 7;	/* only used for mips4 */
#endif /* _MIPSEL */
	} fc_struct;
};
#endif /* _LANGUAGE_C */

/*
 * Constants for the rounding_mode field of the fpc_csr
 */
#define ROUND_TO_NEAREST	0
#define ROUND_TO_ZERO		1
#define ROUND_TO_PLUS_INFINITY	2
#define ROUND_TO_MINUS_INFINITY	3

/* Masks to get at the unimplemented exception */
#define FPCSR_FLUSH_ZERO	0x01000000
#define	FPCSR_UNIMP		0x00020000
#define	FPCSR_EXCEPTIONS	0x0003f000	/* all exceptions */
#define	FPCSR_ENABLES		0x00000f80
#define	FPCSR_FLAGS		0x0000007c

#ifdef _LANGUAGE_ASSEMBLY
/*
 * Assembly register names for the floating-point control registers.
 */
#define fpc_irr	$0
#define fpc_led	$0
#define fpc_eir	$30
#define fpc_csr	$31

#endif /* _LANGUAGE_ASSEMBLY */

#endif /* __SYS_FPU_H__ */
