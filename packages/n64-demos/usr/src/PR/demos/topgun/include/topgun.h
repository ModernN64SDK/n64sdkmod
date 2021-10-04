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
 * File:	topgun.h
 *
 */

#define	STATIC_SEGMENT		1
#define	DYNAMIC_SEGMENT		2

#define	SCREEN_HT	240
#define	SCREEN_WD	320

#define	STACKSIZE	0x2000

#define TOPGUN_ZBUFFER 	0xd0000

/*
 * ifdef needed because this file is included by "spec"
 */
#ifdef _LANGUAGE_C

/*
 * Layout of our dynamic segment
 */
typedef struct {
	Mtx	projectionBG;
	Mtx	projection;
	Mtx	viewingBG;
	Mtx	viewing;
	Mtx	translate;
	Mtx	rotate0;
	Mtx	rotate1;
	Mtx	rotate2;
	Gfx	glist[2048];
} Dynamic;

extern u16      zbuffer[];

extern unsigned short cfb_16_a[];
extern unsigned short cfb_16_b[];

extern Dynamic	dynamic;

extern Gfx 	rspinit_dl[];
extern Gfx 	rdpinit_dl[];
extern Gfx 	clear_zbuffer[];
extern Gfx 	av8b[];
extern Gfx	ground[];
extern Gfx 	sky[];
extern Gfx 	onetri_dl[];
extern Gfx 	subtri_dl[];

extern u64	dram_stack[];
extern u64	rdp_output[];
extern u64	rdp_output_len;

#define RDP_OUTPUT_LEN (4096*16)

#endif	/* _LANGUAGE_C */
