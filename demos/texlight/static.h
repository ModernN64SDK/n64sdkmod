
/**************************************************************************
 *                                                                        *
 *               Copyright (C) 1995, Silicon Graphics, Inc.               *
 *                                                                        *
 *  These coded instructions, statements, and computer programs  contain  *
 *  unpublished  proprietary  information of Silicon Graphics, Inc., and  *
 *  are protected by Federal copyright  law.  They  may not be disclosed  *
 *  to  third  parties  or copied or duplicated in any form, in whole or  *
 *  in part, without the prior written consent of Silicon Graphics, Inc.  *
 *                                                                        *
 *************************************************************************/

/*
 * File:	static.h
 * Create Date:	Thu Dec 14 13:52:23 PST 1995
 *
 */

/*
 * static display lists
 */
extern Gfx 	init_dl[];
extern Gfx	clearcfb_dl[];
extern Gfx	clearzbuffer_dl[];

extern Gfx	setrend[];
extern Gfx	setrend_bilerp[];
extern Gfx	setrend_trilerp[];
extern Gfx	setrend_bilerp_prim[];
extern Gfx	setrend_trilerp_prim[];
extern Gfx	setrend_light[];
extern Gfx	setrend_light_bilerp[];
extern Gfx	setrend_light_trilerp[];
extern Gfx	setrend_1spec[];
extern Gfx	setrend_2spec[];

extern Gfx	load_tex_brick_dl[];
extern Gfx	load_tex_checker_dl[];
extern Gfx	load_tex_molecule_dl[];
extern Gfx	load_tex_chainlink_dl[];



extern Gfx	teapot_dl[];
extern Gfx	cube_dl[];

