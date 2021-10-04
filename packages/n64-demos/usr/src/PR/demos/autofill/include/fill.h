
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
 * File:	fill.h
 *
 */

/*
 *  These to define output graph
 */

#define ASPECT_LO	1.0
#define ASPECT_HI	2.0
#define ASPECT_STEP	3.0

#define AREA_HI		500
#define AREA_LO		10
#define AREA_STEP	10


/*
 *  Set vertex
 */
#define V(p, x, y, z, f, s, t, r, g, b, a) {				\
        (p)->v.ob[0] = (x);						\
        (p)->v.ob[1] = (y);						\
	(p)->v.ob[2] = (z);						\
	(p)->v.flag  = (f);						\
	(p)->v.tc[0] = (s);						\
	(p)->v.tc[1] = (t);						\
	(p)->v.cn[0] = (r);						\
	(p)->v.cn[1] = (g);						\
	(p)->v.cn[2] = (b);						\
	(p)->v.cn[3] = (a);						\
}

/*
 *  My color combine mode (noise*prim_color)
 */
#define G_CC_MYNOISE    NOISE, 0, PRIMITIVE, 0, 0, 0, 0, PRIMITIVE

/*
 *  Screen Size defines
 */
#define	SCREEN_HT		240
#define	SCREEN_WD		320
#define MAXCLIPRATIO      	6

/* this stack size is in bytes, and is a lot larger
 * than this program needs.
 */
#define	STACKSIZE	0x2000

/* 
 *  Large enough to handle maximum possible number
 *  of triangles
 */
#define RDPTRIS		3000					/* # tris output by rsp at a time */
#define RDPATTS		256						/* guess storage for attributes, etc. */
#define	RDPLIST_LEN	(RDPTRIS*6 + RDPATTS)	/* total storage, 4 edge+2 Z */
#define	TRILIST_LEN	(2*RDPTRIS+1) 			/* one vertex load, one tri + end list */
#define VTXLIST_LEN	(3*RDPTRIS)				/* 3 verts per tri */
#define	GLIST_LEN	256

/*
 * ifdef needed because this file is included by "spec"
 */
#ifdef _LANGUAGE_C

/*
 * Layout of dynamic data.
 *
 * This structure holds the things which change per frame. It is advantageous
 * to keep dynamic data together so that we may selectively write back dirty
 * data cache lines to DRAM prior to processing by the RCP.
 *
 */
typedef struct {
  Mtx	projection;
  Mtx	start;
  Mtx	viewing;
  Mtx   identity;
  Vtx   vtxlist[RDPTRIS*3];
  Gfx	trilist[TRILIST_LEN];
  Gfx	glist[GLIST_LEN];
} Dynamic;

extern Dynamic	dynamic;

extern unsigned short cfb_16_a[];
extern unsigned short cfb_16_b[];
extern unsigned short cfb_16_a_cozy[];
extern unsigned short cfb_16_b_cozy[];
extern unsigned short cfb_16_b_other[];
extern unsigned short zbuffer[];

extern Gfx       *glistp;
extern Dynamic   *dynamicp;

extern int    ActiveController;
extern int    ControllerInput;

extern float  TICKSPERSEC;

extern int    UseAAMode;
extern int    DoClrClr;
extern int    UseZMode;

extern float  TimePerFrame;

extern int    SuperBlock;

extern int    SelfScaleTimer;

extern int    ClipRatio;
extern int    TimerTicks;

extern int    TriSortOrder;

extern u64		dram_stack[];
extern u64		rdp_output[];

float frand(void);
void  ApplyGlobalTransformations(void);

#endif	/* _LANGUAGE_C */
