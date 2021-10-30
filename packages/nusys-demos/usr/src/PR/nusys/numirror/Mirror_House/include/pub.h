/*======================================================================*/
/*		NIFFprev						*/
/*		pub.h							*/
/*									*/
/*		Copyright (C) 1997, NINTENDO Co,Ltd.			*/
/*									*/
/*======================================================================*/

/* --------------------------------------------------------------------------
   Segments and Other Define Statements 
   (also valid with spec )
 -------------------------------------------------------------------------- */

#define	PHISICAL_SEGMENT	0
#define	DATA_SEGMENT		1
#define	CFB_SEGMENT		2
#define	ZBUF_SEGMENT		3

#define	SCREEN_HT	240
#define	SCREEN_WD	320
#define	HRSCRN_HT       (SCREEN_HT*2)
#define	HRSCRN_WD	(SCREEN_WD*2)

#define CFB_SIZE (SCREEN_HT*SCREEN_WD*2)
#define ZBUF_SIZE CFB_SIZE
#define	STACKSIZE 0x8000

/* dram stack */
#define DRAMSTACK_ADDRESS_TOP	0x803b3000
#define DRAMSTACK_ADDRESS_END	0x803b4fff

#ifdef _LANGUAGE_C

/* --------------------------------------------------------------------------
	Convenient Macros
 -------------------------------------------------------------------------- */
/* Macros that can be used as formal values (no semicolon necessary) */
#define PI 3.1415926535897932384626433832795
#define PI2 (PI*2)
#define ABS(A) (((A)<0)? 0-(A): (A))
#define ABSF(A) (((A)<0.0F)? 0.0F-(A): (A))
#define ROUND(d) (int)(((d) >= 0.0) ? ((d) + 0.5) : ((d) - 0.5))
#define ROUNDF(d) (int)(((d) >= 0.0F) ? ((d) + 0.5F) : ((d) - 0.5F))

#define SIGN(A) (((A)<0)? -1: 1)
#define RAD2DEG(A) (360.0/PI2*(A))
#define DEG2RAD(A) (PI2/360.0*(A))

#define MODIFY(A,B) ((1.0-(A))+((A)*(B)))

#define ISDIGIT(v) ((v)>='0' && (v)<='9')

#define BITTEST1(reg, mask) ((reg) & (mask))
#define BITTEST(reg, mask) (((reg) & (mask))==(mask))
#define BITRESET(reg, mask) ((reg) &= ~(mask))
#define BITSET(reg, mask) ((reg) |= (mask))
#define BITINVERT(reg, mask) ((reg) ^= (mask))

/* The following are macros used as functions (semicolon required) */
#define AROUND(r) while((r)>PI) (r)-=PI2; while((r)<-PI) (r)+=PI2

/* N64 unique */

#define PADTHRESH(num,thresh)	((s16)(num)>(thresh)) ? (num)-(thresh) : (\
                		((s16)(num)<(-1*(thresh))) ? (num)+(thresh) : 0)

#define CONT_CU CONT_E
#define CONT_CD CONT_D
#define CONT_CL CONT_C
#define CONT_CR CONT_F


/* interrupt enabler, disabler */

#if 1
#define SETUP_INTERRUPT_MASK int intdummy
#define DISABLE_INTERRUPT 
#define ENABLE_INTERRUPT 
#else
#define SETUP_INTERRUPT_MASK OSPri	__pri
#define DISABLE_INTERRUPT __pri = osGetThreadPri(NULL); \
    osSetThreadPri(NULL, OS_PRIORITY_APPMAX)
#define ENABLE_INTERRUPT osSetThreadPri(NULL, __pri)
#endif

#endif  /* #ifdef _LANGUAGE_C */

