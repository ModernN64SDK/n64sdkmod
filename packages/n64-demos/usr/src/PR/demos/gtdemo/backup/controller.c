
/*
 * Copyright 1995, Silicon Graphics, Inc.
 * ALL RIGHTS RESERVED
 *
 * UNPUBLISHED -- Rights reserved under the copyright laws of the United
 * States.   Use of a copyright notice is precautionary only and does not
 * imply publication or disclosure.
 *
 * U.S. GOVERNMENT RESTRICTED RIGHTS LEGEND:
 * Use, duplication or disclosure by the Government is subject to restrictions
 * as set forth in FAR 52.227.19(c)(2) or subparagraph (c)(1)(ii) of the Rights
 * in Technical Data and Computer Software clause at DFARS 252.227-7013 and/or
 * in similar or successor clauses in the FAR, or the DOD or NASA FAR
 * Supplement.  Contractor/manufacturer is Silicon Graphics, Inc.,
 * 2011 N. Shoreline Blvd. Mountain View, CA 94039-7311.
 *
 * THE CONTENT OF THIS WORK CONTAINS CONFIDENTIAL AND PROPRIETARY
 * INFORMATION OF SILICON GRAPHICS, INC. ANY DUPLICATION, MODIFICATION,
 * DISTRIBUTION, OR DISCLOSURE IN ANY FORM, IN WHOLE, OR IN PART, IS STRICTLY
 * PROHIBITED WITHOUT THE PRIOR EXPRESS WRITTEN PERMISSION OF SILICON
 * GRAPHICS, INC.
 *
 */

/*---------------------------------------------------------------------*
        Copyright (C) 1997 Nintendo. (Originated by SGI)
        
        $RCSfile: controller.c,v $
        $Revision: 1.13 $
        $Date: 1998/09/30 13:22:37 $
 *---------------------------------------------------------------------*/

/*
 * File:	controller.c
 * Creator:	hsa@sgi.com
 * Create Date:	Tue Oct 31 14:20:15 PST 1995
 *
 */

#include <ultra64.h>
#include <ramrom.h>
#include "controller.h"

/* exported variables: */
int	do_turbo_z;
float	world_theta = 0.0;
int	do_view_rotate;
float	view_rotate_inc;
float	view_dist = 400.0;
float	p1_tx, p1_ty, p1_tz;
float	p1_dx, p1_dy, p1_dz;
int	walking;
int	do_reset = TRUE;
int	which_char;
int	do_left_punch, do_right_punch;
int	use_fifo_ucode;

/* extern functions we need to call: */
extern void	start_walk_cycle(void);
extern void	stop_walk_cycle(void);

/* private variables: */
static OSMesgQueue	controllerMsgQ;
static OSMesg		controllerMsgBuf;
static OSContStatus     statusdata[MAXCONTROLLERS];
static OSContPad        controllerdata[MAXCONTROLLERS];

int 
resetControllers()
{
    do_turbo_z = 0;
    which_char = 0;
    do_view_rotate = FALSE;
    world_theta = 0.0;
    view_rotate_inc = 0.0;
    view_dist = 400.0;
    p1_tx = p1_ty = p1_tz = 0.0;
    p1_dx = p1_dy = p1_dz = 0.0;
    walking = FALSE;
    do_reset = TRUE;
    do_left_punch = FALSE;
    do_right_punch = FALSE;
    use_fifo_ucode = FALSE;
}

/*
 *
 * Return the lowest number controller connected to system
 */
int 
initControllers()
{
    OSMesgQueue     serialMsgQ;
    OSMesg          serialMsg;
    int             i;
    u8              pattern;

    resetControllers();

    osCreateMesgQueue(&serialMsgQ, &serialMsg, 1);
    osSetEventMesg(OS_EVENT_SI, &serialMsgQ, (OSMesg)1);

    osContInit(&serialMsgQ, &pattern, &statusdata[0]);

    osCreateMesgQueue(&controllerMsgQ, &controllerMsgBuf, 1);
    osSetEventMesg(OS_EVENT_SI, &controllerMsgQ, (OSMesg)0);

    for (i = 0; i < MAXCONTROLLERS; i++) {
        if ((pattern & (1<<i)) &&
	    !(statusdata[i].errno & CONT_NO_RESPONSE_ERROR))
            return i;
    }
    return -1;
}

void 
readController(int controllerSlot)
{
    static u16		button, lastbutton;
    int			stick_x, stick_y;
    
    if (controllerSlot < 0) {
#ifdef DEBUG
	osSyncPrintf("no controller connection\n");
#endif
	return;
    }

    osContStartReadData(&controllerMsgQ);
    (void)osRecvMesg(&controllerMsgQ, NULL, OS_MESG_BLOCK);
    osContGetReadData(controllerdata);

    button = controllerdata[controllerSlot].button;
    stick_x = controllerdata[controllerSlot].stick_x;
    stick_y = controllerdata[controllerSlot].stick_y;
    
    if ((stick_x >= -5 && stick_x <= 5) &&
	(stick_y >= -5 && stick_y <= 5)) {
	stop_walk_cycle();
    } else {
	start_walk_cycle();
	p1_dx = (float) ((float)stick_x/90.0);
	p1_dz = (float) ((float)stick_y/90.0);
	p1_tx = (float) ((float)stick_x/90.0);
	p1_tz = (float) -((float)stick_y/90.0);
    }

    if (button & CONT_A && !(lastbutton & CONT_A)) {
	which_char++;
	which_char = which_char % 4;
    }
	    
    if (button & CONT_B && !(lastbutton & CONT_B)) {
	do_turbo_z ^= 0x01;
    }
	    
    if (button & CONT_G && !(lastbutton & CONT_G)) {
	if (do_view_rotate)
	    do_view_rotate = FALSE;
	else
	    do_view_rotate = TRUE;
    }
	    
    if (button & CONT_START) {
	resetControllers();
    }
	    
    if (button & CONT_UP) {
	view_dist -= 2.0;
    }
	    
    if (button & CONT_DOWN) {
	view_dist += 2.0;
    }

    if ((button & CONT_LEFT) && !(lastbutton & CONT_LEFT)) {
    }

    if ((button & CONT_RIGHT) && !(lastbutton & CONT_RIGHT)) {
    }

	    
    if (button & CONT_L) {
	view_rotate_inc = -(6.28/1200.0);
    }
	    
    if (button & CONT_R) {
	view_rotate_inc = (6.28/1200.0);
    }
	    
    if ((button & CONT_E) && !(lastbutton & CONT_E)) {
    }
	    
    if ((button & CONT_D) && !(lastbutton & CONT_D)) {
	use_fifo_ucode ^= TRUE;
    }
	    
    if ((button & CONT_C) && !(lastbutton & CONT_C)) {
	do_left_punch = TRUE;
    }
	    
    if ((button & CONT_F) && !(lastbutton & CONT_F)) {
	do_right_punch = TRUE;
    }
	    
    lastbutton = button;
}
