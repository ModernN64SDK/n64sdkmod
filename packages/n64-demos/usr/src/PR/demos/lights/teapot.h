
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
 * File:	teapot.h
 * Creator:	hsa@sgi.com
 * Create Date:	Sun Jun  4 18:39:48 PDT 1995
 *
 */

#define	STATIC_SEGMENT		1
#define	CFB_SEGMENT		1

#define	SCREEN_HT 240
#define	SCREEN_WD 320

#define	STACKSIZE 0x2000

#define RDP_OUTPUT_LEN (4096*16)

/*
 * ifdef needed because this file is included by "spec"
 */
#ifdef _LANGUAGE_C

/*
 * Layout of our dynamic segment
 */
typedef struct {
	Mtx	projection;
	Mtx	viewing;

	LookAt	lookat[2];
	Hilite	hilite[2];

        Mtx     teapot_scale;
        Mtx     teapot_rotate;
        Mtx     teapot_trans;

        Mtx     floor_model0;
        Mtx     floor_model1;
        Mtx     floor_model2;
        Mtx     floor_model3;
        Mtx     floor_model4;
        Mtx     floor_model5;
        Mtx     floor_model6;
        Mtx     floor_model7;
        Mtx     floor_model8;

        Mtx     shadow_scale;
        Mtx     shadow_rotate;
        Mtx     shadow_trans;

	Gfx	glist[2048];
} Dynamic;

extern Dynamic	dynamic;

extern unsigned short 	cfb_16_a[];
extern unsigned short 	cfb_16_b[];
extern unsigned short	zbuffer[];

extern Gfx 	rspinit_dl[];
extern Gfx 	rdpinit_dl[];
extern Gfx 	teapot_setup_dl[];
extern Gfx 	teapot_geom_dl[];
extern Gfx 	floor_setup_dl[];
extern Gfx 	floor_geom_dl[];
extern Gfx 	shadow_setup_dl[];
extern Gfx 	shadow_geom_dl[];

extern u64 dram_stack[]; /* used for matrix stack */
extern u64 rdp_output[]; /* buffer for RDP DL */

#endif	/* _LANGUAGE_C */
