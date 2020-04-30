/**************************************************************************
 *                                                                        *
 *               Copyright (C) 1994, Silicon Graphics, Inc.               *
 *                                                                        *
 *  These coded instructions, statements, and computer programs  contain  *
 *  unpublished  proprietary  information of Silicon Graphics, Inc., and  *
 *  are protected by Federal copyright  law.  They  may not be disclosed  *
 *  to  third  parties  or copied or duplicated in any form, in whole or  *
 *  in part, without the prior written consent of Silicon Graphics, Inc.  *
 *                                                                        *
 *************************************************************************/

#define	PHYSICAL_SEGMENT	0
#define	STATIC_SEGMENT		1
#define	DYNAMIC_SEGMENT		2

#define	MAX_STATIC_GFX_SIZE	256

#define	SCREEN_HT	240
#define	SCREEN_WD	320

/* Stacksize in bytes */
#define	STACKSIZE	0x2000
#define MAXHIST         256

#define REALCLEAR 1

/*
 * ifdef needed because this file is included by "spec"
 */
#ifdef _LANGUAGE_C

/*
 * Layout of dynamic segment
 */
typedef struct {
	Mtx	projection;
	Mtx	viewing;
	Vtx	v[MAXHIST * 3];
	Gfx	glist[2048];
} Dynamic;

extern unsigned short	zbuffer[];
extern Gfx	setup_rdpstate[];
extern Gfx	setup_rspstate[];
extern Gfx	clear_fb[];
extern Gfx	init[];
extern Gfx	grid[];
extern Gfx	walls[];

extern Gfx	*glistp;	/* global for test case procs */

extern u64	dram_stack[];
extern u64	dram_yield[];

extern unsigned short cfb_16_a[];
extern unsigned short cfb_16_b[];

#endif	/* _LANGUAGE_C */
