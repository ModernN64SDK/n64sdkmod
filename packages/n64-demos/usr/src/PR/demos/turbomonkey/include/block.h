
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
 * File:	lines.h
 * Create Date:	Thu Jun 22 09:28:01 PDT 1995
 *
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

#define	SCREEN_HT	240

#define SCREEN_WD_320   320
#define SCREEN_WD_MIN   320
#define SCREEN_WD_MAX   640
#define SCREEN_WD       SCREEN_WD_MAX

#define XSCALE_MAX      0x400
#define MAXCONTROLLERMODE  3

#define XMIN             -5
#define XMAX            315

#define YMIN             40
#define YMAX            230

#define ZMIN              0
#define ZMAX            600

#define CUBEWIDTH        30
#define PLANEMINX        20
#define PLANEMAXX       300
#define PLANEMINZ     -1500
#define PLANEMAXZ        -5

#define MAXCLIPRATIO      6
#define MAXBLOCKSPERAXIS  6

#define MAXBLOCKS         MAXBLOCKSPERAXIS*MAXBLOCKSPERAXIS*MAXBLOCKSPERAXIS

/* this stack size is in bytes, and is a lot larger
 * than this program needs.
 */
#define	STACKSIZE	0x1500

/* Large enough to handle maximum possible number
   of triangles, and then some */
#define	GLIST_LEN	200000
/*
 * ifdef needed because this file is included by "spec"
 */
#ifdef _LANGUAGE_C

#include <PR/gt.h>

/*
 * Layout of dynamic data.
 *
 * This structure holds the things which change per frame. It is advantageous
 * to keep dynamic data together so that we may selectively write back dirty
 * data cache lines to DRAM prior to processing by the RCP.
 *
 */
typedef struct {
  Mtx	AA;
  Mtx	BB;

  Mtx	projection;

  Mtx   RotationX;
  Mtx   RotationY;

  Mtx   Translation;

  Mtx   TranslateIn;
  Mtx   TranslateOut;
  
#if 0
  Mtx	modeling1[MAXBLOCKS+1];
  Mtx   modeling2[MAXBLOCKS+1];
  Mtx   modeling3[MAXBLOCKS+1];
  Mtx   modeling4[MAXBLOCKS+1];
#endif

  gtState	objState[MAXBLOCKS+1];

  Mtx	viewing;
  Mtx   identity;

  gtGfx	gtlist[512];

  Gfx	glist[GLIST_LEN];
} Dynamic;

/*
 * Turbo data:
 */
extern gtState	dpInitClearObj;
extern gtState	dpFinalObj;
extern gtTriN	cube0_tris[];


extern Dynamic	dynamic;

extern unsigned short cfb_16_a[];
extern unsigned short cfb_16_b[];
extern unsigned short zbuffer[];

extern void      *cfb_ptrs[2];
extern int       draw_buffer;

extern gtGfx     *gtlistp;
extern Gfx       *glistp;
extern Dynamic   *dynamicp;

extern float  theta;

extern int    ActiveController;
extern int    ControllerInput;
extern int    CreateNewCubes;
extern int    CreateNewPlanes;

extern float  TICKSPERSEC;

extern int    UseAAMode;
extern int    UseZMode;

extern float  TimePerFrame;

extern int    SuperBlock;

extern int    SelfScaleTimer;

extern int    NumPlanePolys;
extern int    NumCubePolys;
extern int    NumDivisions;

extern int    EyeZPosition;

extern int    TranslateHorizontal;
extern int    TranslateVertical;

extern int    RotateGlobalXAxis;
extern int    RotateGlobalYAxis;
extern int    ClipRatio;
extern int    TimerTicks;
extern int    UseTextureMode;
extern int    UseGlobalTransforms;

extern int    UseViModeX;
extern int    ScreenWidth;
extern int    ScreenWidthDelta;


float frand(void);
void  ApplyGlobalTransformations(void);

extern Vtx q0[];
extern gtTriN	tris[];

#endif	/* _LANGUAGE_C */
