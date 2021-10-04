
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

/*
 * File:	letters.h
 * Creator:	hsa@sgi.com
 * Create Date:	Sun Jun  4 18:39:48 PDT 1995
 *
 */

#define	STATIC_SEGMENT		1
#define	CFB_SEGMENT		1

#define	SCREEN_HT	240
#define	SCREEN_WD	320

#define	STACKSIZE	0x2000

/*
 * ifdef needed because this file is included by "spec"
 */
#ifdef _LANGUAGE_C


/* buffer size for RDP DL */
#define RDP_OUTPUT_LEN	(4096*16)

/* used for matrix stack */
extern u64	dram_stack[];

/* buffer for RDP DL */
extern u64	rdp_output[];

/*
 * Layout of our dynamic segment
 */
typedef struct {
	Mtx	projection;
	Mtx	viewing;

	Mtx	projectionBG;
	Mtx	viewingBG;
        Mtx     identity;

	LookAt	lookat[2];
	Hilite	hilite[2];

        Mtx     letters_scale;
        Mtx     letters_rotate;
        Mtx     letters_trans;

	Gfx	glist[2048];
} Dynamic;

extern Dynamic	dynamic;

extern unsigned short 	cfb_16_a[];
extern unsigned short 	cfb_16_b[];
extern unsigned short	zbuffer[];

extern Gfx 	rspinit_dl[];
extern Gfx 	rdpinit_dl[];
extern Gfx 	letters_setup_dl[];
extern Gfx 	u64_chrome1D_dl[];
extern Gfx 	u64_chrome2D_dl[];
extern Gfx 	u64_geom_dl[];
extern Gfx 	setting_setup_dl[];
extern Gfx 	setting_geom_dl[];

#endif	/* _LANGUAGE_C */
