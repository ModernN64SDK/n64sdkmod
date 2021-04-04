/*======================================================================*/
/*		NIFFprev						*/
/*		expad.c							*/
/*									*/
/*		Copyright (C) 1997, NINTENDO Co,Ltd.			*/
/*									*/
/*======================================================================*/
#define __EXPAD_C

/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	Expansion Pad Input Routines 
 :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */

#include "pack.h"
#include "nusys.h"

/* ===========================================================================
	static variables
 =========================================================================== */
static OSContPad lastpad[MAXCONTROLLERS];
static u16 lasttrg[MAXCONTROLLERS];
static EXCONTPAD expad[MAXCONTROLLERS];
static int numControllers=0, repstart, repstep;
static NUContData contdata[MAXCONTROLLERS];

/* ===========================================================================
	public functions
 =========================================================================== */

/* ----------------------------------------------------------------------------
name:
    expadInitControllers
synopsis:
    void expadInitControllers( int maxcont, int repeatstart, int repeatstep );
description:
    Initializes the pad-loading module
parameters:
    int maxcont;
	Number of controllers to fetch data 
    int repeatstart;
	Number of frames until the key repeat starts
    int repeatstep;
	The trigger frame interval from the start of the key repeat
returns:
notes:
---------------------------------------------------------------------------- */
void expadInitControllers( int maxcont, int repeatstart, int repeatstep )
{
    int             i;

    nuContInit();

    repstart = repeatstart;
    repstep = repeatstart+repeatstep;

    for(i=0; i<maxcont; ++i){
	lastpad[i].button = 0;
	lastpad[i].stick_x = 0;
	lastpad[i].stick_y = 0;
	lasttrg[i] = 0;
    }

    nuContDataReadStart();
    numControllers = maxcont;

    return;
}

/* ----------------------------------------------------------------------------
name:
    expadReadController
synopsis:
    EXCONTPAD *expadReadController(void);
description:
    Loads the pad.
parameters:
returns:
    EXCONTPAD *;
	Pointer to the pad buffer
notes:
---------------------------------------------------------------------------- */
static s8 calcAbout( s8 vec )
{
    s8 pow;

    pow = ( vec < 0 )? -vec: vec;
    pow = ( pow <= (1 << 3) )? 0: pow - (1 << 3);

    return ( vec < 0 )? -pow: pow;
}

EXCONTPAD *expadReadController(void)
{
    int i,j;
    u16 btn,trg, tmask, repcnt;

    nuContDataReadWait();

    for (i=0; i<numControllers; i++) {

	/* Process the digital button */
	nuContDataGetEx(&contdata[i], i);

	expad[i].button = btn = contdata[i].button;
	trg = contdata[i].trigger;

	expad[i].trigger = trg;
	for(j=0; j<16; ++j){
	    tmask = 1<<j;
	    /* Initialize the repeat counter if a trigger is present */
	    if(trg & tmask)
	      expad[i].repcnt[j] = 0;
	    /* When the button is pushed */
	    else if(btn & tmask){
		repcnt = expad[i].repcnt[j];
		/* Advance the counter */
		++repcnt;
		if(repcnt == repstart)
		  /* Enter repeat mode */
		  trg |= tmask;
		else if(repcnt > repstep){
		    /* Key repeat */
		    repcnt = repstart;
		    trg |= tmask;
		}
		expad[i].repcnt[j] = repcnt;
	    }
	}
	expad[i].repeat = trg;

	/* Process the analog stack */
	expad[i].about_x = calcAbout(expad[i].stick_x = contdata[i].stick_x);
	expad[i].about_y = calcAbout(expad[i].stick_y = contdata[i].stick_y);
	expad[i].delta_x = contdata[i].stick_x - lastpad[i].stick_x;
	expad[i].delta_y = contdata[i].stick_y - lastpad[i].stick_y;
	expad[i].errno = contdata[i].errno;

	lasttrg[i] = trg;
    }

    nuContDataReadStart();

    return &expad[0];
}


/* ==========================================================================
	end of file.						%eof
 ========================================================================== */

