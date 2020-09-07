
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
 * File:	controller.h
 * Creator:	hsa@sgi.com
 * Create Date:	Tue Oct 31 14:22:52 PST 1995
 *
 */


/* exported variables: */
extern int	do_turbo_z;
extern float	world_theta;
extern int	do_view_rotate;
extern float	view_rotate_inc;
extern float	view_dist;
extern float	p1_tx, p1_ty, p1_tz;
extern float	p1_dx, p1_dy, p1_dz;
extern int	walking;
extern int	do_reset;
extern int	which_char;
extern int	do_left_punch, do_right_punch;
extern int	use_fifo_ucode;

/* exported functions: */
extern int resetControllers(void);
extern int initControllers(void);
extern void readController(int);


