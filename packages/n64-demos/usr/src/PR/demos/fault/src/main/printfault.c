/*
 * Copyright 1995, Silicon Graphics, Inc.
 * All Rights Reserved.
 *
 * This is UNPUBLISHED PROPRIETARY SOURCE CODE of Silicon Graphics, Inc.;
 * the contents of this file may not be disclosed to third parties, copied or
 * duplicated in any form, in whole or in part, without the prior written
 * permission of Silicon Graphics, Inc.
 *
 * RESTRICTED RIGHTS LEGEND:
 * Use, duplication or disclosure by the Government is subject to restrictions
 * as set forth in subdivision (c)(1)(ii) of the Rights in Technical Data
 * and Computer Software clause at DFARS 252.227-7013, and/or in similar or
 * successor clauses in the FAR, DOD or NASA FAR Supplement. Unpublished -
 * rights reserved under the Copyright Laws of the United States.
 *
 * $Revision: 1.1 $
 */

#include <ultra64.h>

#define PRINTF	osSyncPrintf


typedef struct {
	u32 mask;
	u32 value;
	char *string;
} regDesc_t;

static char	errMessage[256];
static char	*errFmtEntry[65536];
static char	*logFmtEntry[65536];

static void	printRegister(u32 , char *, regDesc_t *);
int		printeFaultData(OSThread *);


static regDesc_t causeDesc[] = {
	{ CAUSE_BD,	CAUSE_BD,		"BD" },
	{ CAUSE_IP8,	CAUSE_IP8,		"IP8" },
	{ CAUSE_IP7,	CAUSE_IP7,		"IP7" },
	{ CAUSE_IP6,	CAUSE_IP6,		"IP6" },
	{ CAUSE_IP5,	CAUSE_IP5,		"IP5" },
	{ CAUSE_IP4,	CAUSE_IP4,		"IP4" },
	{ CAUSE_IP3,	CAUSE_IP3,		"IP3" },
	{ CAUSE_SW2,	CAUSE_SW2,		"IP2" },
	{ CAUSE_SW1,	CAUSE_SW1,		"IP1" },
	{ CAUSE_EXCMASK,EXC_INT,
			"Interrupt" },
	{ CAUSE_EXCMASK,EXC_MOD,
			"TLB modification exception" },
	{ CAUSE_EXCMASK,EXC_RMISS,
			"TLB exception on load or instruction fetch" },
	{ CAUSE_EXCMASK,EXC_WMISS,
			"TLB exception on store" },
	{ CAUSE_EXCMASK,EXC_RADE,
			"Address error on load or instruction fetch" },
	{ CAUSE_EXCMASK,EXC_WADE,
			"Address error on store" },
	{ CAUSE_EXCMASK,EXC_IBE,
			"Bus error exception on instruction fetch" },
	{ CAUSE_EXCMASK,EXC_DBE,
			"Bus error exception on data reference" },
	{ CAUSE_EXCMASK,EXC_SYSCALL,
			"System call exception" },
	{ CAUSE_EXCMASK,EXC_BREAK,
			"Breakpoint exception" },
	{ CAUSE_EXCMASK,EXC_II,
			"Reserved instruction exception" },
	{ CAUSE_EXCMASK,EXC_CPU,
			"Coprocessor unusable exception" },
	{ CAUSE_EXCMASK,EXC_OV,
			"Arithmetic overflow exception" },
	{ CAUSE_EXCMASK,EXC_TRAP,
			"Trap exception" },
	{ CAUSE_EXCMASK,EXC_VCEI,
			"Virtual coherency exception on intruction fetch" },
	{ CAUSE_EXCMASK,EXC_FPE,
			"Floating point exception (see fpcsr)" },
	{ CAUSE_EXCMASK,EXC_WATCH,
			"Watchpoint exception" },
	{ CAUSE_EXCMASK,EXC_VCED,
			"Virtual coherency exception on data reference" },
	{ 0,		0,		"" }
};


static regDesc_t srDesc[] = {
	{ SR_CU3,	SR_CU3,		"CU3" },
	{ SR_CU2,	SR_CU2,		"CU2" },
	{ SR_CU1,	SR_CU1,		"CU1" },
	{ SR_CU0,	SR_CU0,		"CU0" },
	{ SR_RP,	SR_RP,		"RP" },
	{ SR_FR,	SR_FR,		"FR" },
	{ SR_RE,	SR_RE,		"RE" },
	{ SR_BEV,	SR_BEV,		"BEV" },
	{ SR_TS,	SR_TS,		"TS" },
	{ SR_SR,	SR_SR,		"SR" },
	{ SR_CH,	SR_CH,		"CH" },
	{ SR_CE,	SR_CE,		"CE" },
	{ SR_DE,	SR_DE,		"DE" },
	{ SR_IBIT8,	SR_IBIT8,	"IM8" },
	{ SR_IBIT7,	SR_IBIT7,	"IM7" },
	{ SR_IBIT6,	SR_IBIT6,	"IM6" },
	{ SR_IBIT5,	SR_IBIT5,	"IM5" },
	{ SR_IBIT4,	SR_IBIT4,	"IM4" },
	{ SR_IBIT3,	SR_IBIT3,	"IM3" },
	{ SR_IBIT2,	SR_IBIT2,	"IM2" },
	{ SR_IBIT1,	SR_IBIT1,	"IM1" },
	{ SR_KX,	SR_KX,		"KX" },
	{ SR_SX,	SR_SX,		"SX" },
	{ SR_UX,	SR_UX,		"UX" },
	{ SR_KSU_MASK,	SR_KSU_USR,	"USR" },
	{ SR_KSU_MASK,	SR_KSU_SUP,	"SUP" },
	{ SR_KSU_MASK,	SR_KSU_KER,	"KER" },
	{ SR_ERL,	SR_ERL,		"ERL" },
	{ SR_EXL,	SR_EXL,		"EXL" },
	{ SR_IE,	SR_IE,		"IE" },
	{ 0,		0,		"" }
};


static regDesc_t fpcsrDesc[] = {
	{ FPCSR_FS,	FPCSR_FS,	"FS" },
	{ FPCSR_C,	FPCSR_C,	"C" },
	{ FPCSR_CE,	FPCSR_CE,	"Unimplemented operation" },
	{ FPCSR_CV,	FPCSR_CV,	"Invalid operation" },
	{ FPCSR_CZ,	FPCSR_CZ,	"Division by zero" },
	{ FPCSR_CO,	FPCSR_CO,	"Overflow" },
	{ FPCSR_CU,	FPCSR_CU,	"Underflow" },
	{ FPCSR_CI,	FPCSR_CI,	"Inexact operation" },
	{ FPCSR_EV,	FPCSR_EV,	"EV" },
	{ FPCSR_EZ,	FPCSR_EZ,	"EZ" },
	{ FPCSR_EO,	FPCSR_EO,	"EO" },
	{ FPCSR_EU,	FPCSR_EU,	"EU" },
	{ FPCSR_EI,	FPCSR_EI,	"EI" },
	{ FPCSR_FV,	FPCSR_FV,	"FV" },
	{ FPCSR_FZ,	FPCSR_FZ,	"FZ" },
	{ FPCSR_FO,	FPCSR_FO,	"FO" },
	{ FPCSR_FU,	FPCSR_FU,	"FU" },
	{ FPCSR_FI,	FPCSR_FI,	"FI" },
	{ FPCSR_RM_MASK,FPCSR_RM_RN,	"RN" },
	{ FPCSR_RM_MASK,FPCSR_RM_RZ,	"RZ" },
	{ FPCSR_RM_MASK,FPCSR_RM_RP,	"RP" },
	{ FPCSR_RM_MASK,FPCSR_RM_RM,	"RM" },
	{ 0,		0,		"" }
};


int
printFaultData(OSThread *t)
{
	__OSThreadContext *tc = &t->context;

	PRINTF( "\nFault in thread %d:\n\n", t->id);
	PRINTF("epc\t\t0x%08x\n", tc->pc);
	printRegister(tc->cause, "cause", causeDesc);
	printRegister(tc->sr, "sr", srDesc);
	PRINTF("badvaddr\t0x%08x\n\n", tc->badvaddr);

	PRINTF("at 0x%016llx v0 0x%016llx v1 0x%016llx\n",
		   tc->at, tc->v0, tc->v1);
	PRINTF("a0 0x%016llx a1 0x%016llx a2 0x%016llx\n",
		   tc->a0, tc->a1, tc->a2);
	PRINTF("a3 0x%016llx t0 0x%016llx t1 0x%016llx\n",
		   tc->a3, tc->t0, tc->t1);
	PRINTF("t2 0x%016llx t3 0x%016llx t4 0x%016llx\n",
		   tc->t2, tc->t3, tc->t4);
	PRINTF("t5 0x%016llx t6 0x%016llx t7 0x%016llx\n",
		   tc->t5, tc->t6, tc->t7);
	PRINTF("s0 0x%016llx s1 0x%016llx s2 0x%016llx\n",
		   tc->s0, tc->s1, tc->s2);
	PRINTF("s3 0x%016llx s4 0x%016llx s5 0x%016llx\n",
		   tc->s3, tc->s4, tc->s5);
	PRINTF("s6 0x%016llx s7 0x%016llx t8 0x%016llx\n",
		   tc->s6, tc->s7, tc->t8);
	PRINTF("t9 0x%016llx gp 0x%016llx sp 0x%016llx\n",
		   tc->t9, tc->gp, tc->sp);
	PRINTF("s8 0x%016llx ra 0x%016llx\n\n",
		   tc->s8, tc->ra);

	printRegister(tc->fpcsr, "fpcsr", fpcsrDesc);

	PRINTF("\nf0  %.7e f2  %.7e f4  %.7e f6  %.7e\n",
		tc->fp0.f.f_even, tc->fp2.f.f_even,
		tc->fp4.f.f_even, tc->fp6.f.f_even);
	PRINTF("f8  %.7e f10 %.7e f12 %.7e f14 %.7e\n",
		tc->fp8.f.f_even,  tc->fp10.f.f_even,
		tc->fp12.f.f_even, tc->fp14.f.f_even);
	PRINTF("f16 %.7e f18 %.7e f20 %.7e f22 %.7e\n",
		tc->fp16.f.f_even, tc->fp18.f.f_even,
		tc->fp20.f.f_even, tc->fp22.f.f_even);
	PRINTF("f24 %.7e f26 %.7e f28 %.7e f30 %.7e\n\n",
		tc->fp24.f.f_even, tc->fp26.f.f_even,
		tc->fp28.f.f_even, tc->fp30.f.f_even);

	PRINTF("d0  %.15e\td2  %.15e\n", tc->fp0.d,  tc->fp2.d);
	PRINTF("d4  %.15e\td6  %.15e\n", tc->fp4.d,  tc->fp6.d);
	PRINTF("d8  %.15e\td10 %.15e\n", tc->fp8.d,  tc->fp10.d);
	PRINTF("d12 %.15e\td14 %.15e\n", tc->fp12.d, tc->fp14.d);
	PRINTF("d16 %.15e\td18 %.15e\n", tc->fp16.d, tc->fp18.d);
	PRINTF("d20 %.15e\td22 %.15e\n", tc->fp20.d, tc->fp22.d);
	PRINTF("d24 %.15e\td26 %.15e\n", tc->fp24.d, tc->fp26.d);
	PRINTF("d28 %.15e\td30 %.15e\n", tc->fp28.d, tc->fp30.d);
}


static void
printRegister(u32 regValue, char *regName, regDesc_t *regDesc)
{
	int first = 1;

	PRINTF("%s\t\t0x%08x ", regName, regValue);
	PRINTF("<");
	while (regDesc->mask != 0) {
		if ((regValue & regDesc->mask) == regDesc->value) {
			if (first)
				first = 0;
			else
				PRINTF(",");
			PRINTF("%s", regDesc->string);
		}
		regDesc++;
	}
	PRINTF(">\n");
}


