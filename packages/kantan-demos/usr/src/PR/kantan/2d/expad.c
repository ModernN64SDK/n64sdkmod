
#define __EXPAD_C

/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	Extended pad data input routine
 :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */

#include "pack.h"

/* ===========================================================================
	static variables
 =========================================================================== */
static OSMesgQueue controllerMsgQ;
static OSContPad dummycontrollerdata = { 0, 0, 0, 0 };
static OSMesg controllerMsgBuf;
static OSContStatus statusdata[MAXCONTROLLERS];
static OSContPad controllerdata[MAXCONTROLLERS];
static OSContPad *validcontrollerdata[MAXCONTROLLERS];

static OSContPad lastpad[MAXCONTROLLERS];
static u16 lasttrg[MAXCONTROLLERS];
static EXCONTPAD expad[MAXCONTROLLERS];
static int numControllers=0, repstart, repstep;

/* ===========================================================================
	public functions
 =========================================================================== */

/* ----------------------------------------------------------------------------
name:
    expadInitControllers
synopsis:
    int expadInitControllers( int maxcont, int repeatstart, int repeatstep );
description:
    Initialize pad read module
 parameter:
    int maxcont;
    int repeatstart;
    int repeatstep;
return:

notes:
---------------------------------------------------------------------------- */
int expadInitControllers( int maxcontrollers, int repeatstart, int repeatstep )
{
    int             i;
    u8              pattern;
    OSMesgQueue     serialMsgQ;
    OSMesg          serialMsg;

    osCreateMesgQueue(&serialMsgQ, &serialMsg, 1);
    osSetEventMesg(OS_EVENT_SI, &serialMsgQ, (OSMesg)1);

    osContInit(&serialMsgQ, &pattern, &statusdata[0]);

    osCreateMesgQueue(&controllerMsgQ, &controllerMsgBuf, 1);
    osSetEventMesg(OS_EVENT_SI, &controllerMsgQ, (OSMesg)0);

    for (i = 0; i < MAXCONTROLLERS; i++)
	validcontrollerdata[i] = &dummycontrollerdata;

    for (i = 0; i < MAXCONTROLLERS; i++) {
        if ((pattern & (1<<i)) &&
                !(statusdata[i].errno & CONT_NO_RESPONSE_ERROR)) {
	    validcontrollerdata[numControllers++] = &controllerdata[i];
	    if (numControllers == maxcontrollers) return numControllers;
	}
    }
    osContStartReadData(&controllerMsgQ);

    repstart = repeatstart;
    repstep = repeatstart+repeatstep;

    for(i=0; i<maxcontrollers; ++i){
	lastpad[i].button = 0;
	lastpad[i].stick_x = 0;
	lastpad[i].stick_y = 0;
	lasttrg[i] = 0;
    }

    return numControllers;
}

/* ----------------------------------------------------------------------------
name:
    expadReadController
synopsis:
    EXCONTPAD *expadReadController(void);
description:
    read pad
parameter:
return:
    EXCONTPAD
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
    OSContPad **vcd;

    if (osRecvMesg(&controllerMsgQ, NULL, OS_MESG_NOBLOCK) != -1) {
    	osContGetReadData(controllerdata);
	osContStartReadData(&controllerMsgQ);
    }

    vcd = validcontrollerdata;

    for (i=0; i<numControllers; i++) {
	/* digital button processing */
	expad[i].button = btn = vcd[i]->button;
	trg = btn &(~lastpad[i].button);
	expad[i].trigger = trg;
	for(j=0; j<16; ++j){
	    tmask = 1<<j;
	    /* If trigger is detected, initialize repeat counter */
	    if(trg & tmask)
	      expad[i].repcnt[j] = 0;
	    /*While button is pressed down */
	    else if(btn & tmask){
		repcnt = expad[i].repcnt[j];
		/* counter proceeded */
		++repcnt;
		if(repcnt == repstart)
		  /* enter repeat mode */
		  trg |= tmask;
		else if(repcnt > repstep){
		    /* repeat key */
		    repcnt = repstart;
		    trg |= tmask;
		}
		expad[i].repcnt[j] = repcnt;
	    }
	}
	expad[i].repeat = trg;

	/*analogue stick processing */
	expad[i].about_x = calcAbout(expad[i].stick_x = vcd[i]->stick_x);
	expad[i].about_y = calcAbout(expad[i].stick_y = vcd[i]->stick_y);
	expad[i].delta_x = vcd[i]->stick_x - lastpad[i].stick_x;
	expad[i].delta_y = vcd[i]->stick_y - lastpad[i].stick_y;
	expad[i].errno = vcd[i]->errno;

	lasttrg[i] = trg;
	lastpad[i] = *vcd[i];
    }

    return &expad[0];
}


/* ==========================================================================
	end of file.						%eof
 ========================================================================== */
