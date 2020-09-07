
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
 * File:	floor.c
 * Creator:	hsa@sgi.com
 * Create Date:	Tue Oct 31 15:06:56 PST 1995
 *
 */

#include <ultra64.h>
#include "gtdemo.h"

static void
force_translate(Mtx *mp, int x, int y, int z)
{
    mp->m[1][2] = (((short int)x << 16) | ((short int)y));
    mp->m[1][3] = ((short int)z << 16) | (1);
}

/* builds the display list for the floor */
void
draw_floor(Dynamic *dynamicp)
{
    gSPDisplayList(glistp++, floor_setup_dl);

#define FLOOR_SCALE	2.0
    guScale(&dynamicp->floor_model[0], FLOOR_SCALE, 1.0, FLOOR_SCALE);
    guScale(&dynamicp->floor_model[1], FLOOR_SCALE, 1.0, FLOOR_SCALE);
    guScale(&dynamicp->floor_model[2], FLOOR_SCALE, 1.0, FLOOR_SCALE);
    guScale(&dynamicp->floor_model[3], FLOOR_SCALE, 1.0, FLOOR_SCALE);
    guScale(&dynamicp->floor_model[4], FLOOR_SCALE, 1.0, FLOOR_SCALE);
    guScale(&dynamicp->floor_model[5], FLOOR_SCALE, 1.0, FLOOR_SCALE);
    guScale(&dynamicp->floor_model[6], FLOOR_SCALE, 1.0, FLOOR_SCALE);
    guScale(&dynamicp->floor_model[7], FLOOR_SCALE, 1.0, FLOOR_SCALE);
    guScale(&dynamicp->floor_model[8], FLOOR_SCALE, 1.0, FLOOR_SCALE);

    force_translate(&dynamicp->floor_model[0], -128.0*FLOOR_SCALE, 0.0, -128.0*FLOOR_SCALE);
    force_translate(&dynamicp->floor_model[1], 0.0, 0.0, -128.0*FLOOR_SCALE);
    force_translate(&dynamicp->floor_model[2], 128.0*FLOOR_SCALE, 0.0, -128.0*FLOOR_SCALE);
    force_translate(&dynamicp->floor_model[3], -128.0*FLOOR_SCALE, 0.0, 0.0);
    force_translate(&dynamicp->floor_model[4], 0.0, 0.0, 0.0);
    force_translate(&dynamicp->floor_model[5], 128.0*FLOOR_SCALE, 0.0, 0.0);
    force_translate(&dynamicp->floor_model[6], -128.0*FLOOR_SCALE, 0.0, 128.0*FLOOR_SCALE);
    force_translate(&dynamicp->floor_model[7], 0.0, 0.0, 128.0*FLOOR_SCALE);
    force_translate(&dynamicp->floor_model[8], 128.0*FLOOR_SCALE, 0.0, 128.0*FLOOR_SCALE);
#undef FLOOR_SCALE

    gSPMatrix(glistp++, OS_K0_TO_PHYSICAL(&(dynamicp->floor_model[0])),
	      G_MTX_MODELVIEW|G_MTX_LOAD|G_MTX_NOPUSH);
    gSPDisplayList(glistp++, floor_geom_dl);

    gSPMatrix(glistp++, OS_K0_TO_PHYSICAL(&(dynamicp->floor_model[1])),
	      G_MTX_MODELVIEW|G_MTX_LOAD|G_MTX_NOPUSH);
    gSPDisplayList(glistp++, floor_geom_dl);

    gSPMatrix(glistp++, OS_K0_TO_PHYSICAL(&(dynamicp->floor_model[2])),
	      G_MTX_MODELVIEW|G_MTX_LOAD|G_MTX_NOPUSH);
    gSPDisplayList(glistp++, floor_geom_dl);

    gSPMatrix(glistp++, OS_K0_TO_PHYSICAL(&(dynamicp->floor_model[3])),
	      G_MTX_MODELVIEW|G_MTX_LOAD|G_MTX_NOPUSH);
    gSPDisplayList(glistp++, floor_geom_dl);

    gSPMatrix(glistp++, OS_K0_TO_PHYSICAL(&(dynamicp->floor_model[4])),
	      G_MTX_MODELVIEW|G_MTX_LOAD|G_MTX_NOPUSH);
    gSPDisplayList(glistp++, floor_geom_dl);

    gSPMatrix(glistp++, OS_K0_TO_PHYSICAL(&(dynamicp->floor_model[5])),
	      G_MTX_MODELVIEW|G_MTX_LOAD|G_MTX_NOPUSH);
    gSPDisplayList(glistp++, floor_geom_dl);

    gSPMatrix(glistp++, OS_K0_TO_PHYSICAL(&(dynamicp->floor_model[6])),
	      G_MTX_MODELVIEW|G_MTX_LOAD|G_MTX_NOPUSH);
    gSPDisplayList(glistp++, floor_geom_dl);

    gSPMatrix(glistp++, OS_K0_TO_PHYSICAL(&(dynamicp->floor_model[7])),
	      G_MTX_MODELVIEW|G_MTX_LOAD|G_MTX_NOPUSH);
    gSPDisplayList(glistp++, floor_geom_dl);

    gSPMatrix(glistp++, OS_K0_TO_PHYSICAL(&(dynamicp->floor_model[8])),
	      G_MTX_MODELVIEW|G_MTX_LOAD|G_MTX_NOPUSH);
    gSPDisplayList(glistp++, floor_geom_dl);

    gSPTexture(glistp++, 0, 0, 0, G_TX_RENDERTILE, G_OFF); /* done; turn texture off */
}
