
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
 * File:	walk.c
 * Creator:	hsa@sgi.com
 * Create Date:	Tue Nov  7 16:51:21 PST 1995
 *
 */

#include <ultra64.h>
#include "gtdemo.h"
#include "character.h"
#include "controller.h"
#include "walktable.h"

/*
 * punches and walks are just playbacks; we have tables of joint
 * angles and for each frame of the cycle we set the right joint
 * with the right angle.
 */

static int	which_step = 0;
static int	am_moving = 0;
static int	walk_count = 0;

/* start and end walk are necessary to prevent stopping in the middle
 * of a walk cycle.
 */
void
start_walk_cycle(void)
{
    am_moving = TRUE;
    if (!walking)
	walking = TRUE;
}

/* update the walk cycle */
void
walk_cycle(Dynamic *dynamicp, Node_t *obj)
{
    float	legf[4][4], shinf[4][4], legr[4][4], shinr[4][4];
    Node_t	*legfp, *shinfp, *legrp, *shinrp;

    guRotateF(legf, leg_front[walk_count], 1.0, 0.0, 0.0);
    guRotateF(shinf, shin_front[walk_count], 1.0, 0.0, 0.0);
    guRotateF(legr, leg_rear[walk_count], 1.0, 0.0, 0.0);
    guRotateF(shinr, shin_rear[walk_count], 1.0, 0.0, 0.0);
    
    /* treating obj like an array here assumes a fixed offset
     * for the appendage node from the obj body. we can assume
     * that, since we wrote the object creation routine too...
     */
    if (which_step) {	/* left foot forward */
	legfp = (Node_t *) &(obj[LEFT_LEG_NODE]);
	shinfp = (Node_t *) &(obj[LEFT_SHIN_NODE]);
	legrp = (Node_t *) &(obj[RIGHT_LEG_NODE]);
	shinrp = (Node_t *) &(obj[RIGHT_SHIN_NODE]);
    } else {		/* right foot forward */
	legfp = (Node_t *) &(obj[RIGHT_LEG_NODE]);
	shinfp = (Node_t *) &(obj[RIGHT_SHIN_NODE]);
	legrp = (Node_t *) &(obj[LEFT_LEG_NODE]);
	shinrp = (Node_t *) &(obj[LEFT_SHIN_NODE]);
    }

    character_update(legfp, OBJ_MTX_LOAD, legf, 0, 0, 0);
    character_update(shinfp, OBJ_MTX_LOAD, shinf, 0, 0, 0);
    character_update(legrp, OBJ_MTX_LOAD, legr, 0, 0, 0);
    character_update(shinrp, OBJ_MTX_LOAD, shinr, 0, 0, 0);

    walk_count++;
    if (walk_count >= WALK_CYCLE_LEN) {
	walk_count = 0;
	which_step ^= 1;
    }

    if (walk_count == 0 && !am_moving)
	walking = FALSE;
}

/* start and end walk are necessary to prevent stopping in the middle
 * of a walk cycle.
 */
void
stop_walk_cycle(void)
{
    am_moving = FALSE;
}

