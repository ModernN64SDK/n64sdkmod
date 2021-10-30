/*======================================================================*/
/*		NIFFprev						*/
/*		expad.h							*/
/*									*/
/*		Copyright (C) 1997, NINTENDO Co,Ltd.			*/
/*									*/
/*======================================================================*/
#ifndef __EXPAD_H
#define __EXPAD_H

/* ==========================================================================
	define symbols and structure definitions.		%define
 ========================================================================== */
typedef struct {
	u16     button;
	u16     trigger;
	u16     repeat;
	u8	repcnt[16];
	s8      stick_x;		/* -80 <= stick_x <= 80 */
	s8      stick_y;		/* -80 <= stick_y <= 80 */
	s8      about_x;		/*  -5 <= stick_x <=  5 */
	s8      about_y;		/*  -5 <= stick_y <=  5 */
	s8      delta_x;		/* -80 <= stick_x <= 80 */
	s8      delta_y;		/* -80 <= stick_y <= 80 */
	u8	errno;
} EXCONTPAD;

typedef struct{
    s8 stx, sty;
    u16 button;
} CPAD;

typedef union{
    CPAD pad;
    int ival;
} CPADPACK;

#define CONT_C_ALL (CONT_CU | CONT_CD | CONT_CL | CONT_CR)

/* ==========================================================================
	public functions and variables.				%public
 ========================================================================== */
void expadInitControllers( int maxcont, int repeatstart, int repeatstep );
EXCONTPAD *expadReadController(void);

#endif  /* #ifndef __EXPAD_H */
