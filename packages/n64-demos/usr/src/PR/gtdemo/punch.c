
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

/*
 * File:	punch.c
 * Creator:	hsa@sgi.com
 * Create Date:	Wed Nov  8 16:25:27 PST 1995
 *
 */

#include <ultra64.h>
#include "gtdemo.h"
#include "character.h"
#include "controller.h"
#include "punchtable.h"

/*
 * punches and walks are just playbacks; we have tables of joint
 * angles and for each frame of the cycle we set the right joint
 * with the right angle.
 */

static int	left_punch_count = 0;
static int	right_punch_count = 0;

/* do a punch with the left arm */
void
left_punch(Dynamic *dynamicp, Node_t *obj)
{
    float	armf[4][4], wristf[4][4];
    Node_t	*armp, *wristp;

    guRotateF(armf, left_arm[left_punch_count], 1.0, 0.0, 0.0);
    guRotateF(wristf, left_wrist[left_punch_count], 1.0, 0.0, 0.0);
    
    /* treating obj like an array here assumes a fixed offset
     * for the appendage node from the obj body. we can assume
     * that, since we wrote the object creation routine too...
     */
    armp = (Node_t *) &(obj[LEFT_ARM_NODE]);
    wristp = (Node_t *) &(obj[LEFT_WRIST_NODE]);

    character_update(armp, OBJ_MTX_LOAD, armf, 0, 0, 0);
    character_update(wristp, OBJ_MTX_LOAD, wristf, 0, 0, 0);

    left_punch_count++;
    if (left_punch_count >= PUNCH_CYCLE_LEN) {
	left_punch_count = 0;
	do_left_punch = FALSE;
    }
}

/* do a punch with the right arm */
void
right_punch(Dynamic *dynamicp, Node_t *obj)
{
    float	armf[4][4], wristf[4][4];
    Node_t	*armp, *wristp;

    /* treating obj like an array here assumes a fixed offset
     * for the appendage node from the obj body. we can assume
     * that, since we wrote the object creation routine too...
     */
    guRotateF(armf, right_arm[right_punch_count], 1.0, 0.0, 0.0);
    guRotateF(wristf, right_wrist[right_punch_count], 1.0, 0.0, 0.0);
    
    armp = (Node_t *) &(obj[RIGHT_ARM_NODE]);
    wristp = (Node_t *) &(obj[RIGHT_WRIST_NODE]);

    character_update(armp, OBJ_MTX_LOAD, armf, 0, 0, 0);
    character_update(wristp, OBJ_MTX_LOAD, wristf, 0, 0, 0);

    right_punch_count++;
    if (right_punch_count >= PUNCH_CYCLE_LEN) {
	right_punch_count = 0;
	do_right_punch = FALSE;
    }
}


