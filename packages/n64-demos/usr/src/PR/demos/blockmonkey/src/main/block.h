
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
#define MAXCONTROLLERMODE  4

#define XMIN             -5
#define XMAX            315

#define YMIN             40
#define YMAX            230

#define ZMIN              0
#define ZMAX            600

#define CUBEWIDTH        30
#define PLANEMINX        10
#define PLANEMAXX       300
#define PLANEMINY        15
#define PLANEMAXY       245
#define PLANEMINZ     -1500
#define PLANEMAXZ        -5

#define EYEZPOS         475

#define MAXCLIPRATIO      6
#define MAXBLOCKSPERAXIS  6

#define MAXBLOCKS         MAXBLOCKSPERAXIS*MAXBLOCKSPERAXIS*MAXBLOCKSPERAXIS
#define MAXTASKS	  500

#define RDP_OUTPUT_BUF		0x8004b400	/* End of Z buffer */
#define Z_BUFFER		0x803da800
#define COLOR_BUFFER		0x802c0000
#define RDP_OUTPUT_LEN          (COLOR_BUFFER-RDP_OUTPUT_BUF)

#define RDP_OUTBUF_SIZE_MIN 	0x100		/* min fifo buffer size */


#define PRIMCOLOR 0
#define GOURAUD   1
#define LIGHTING  2


/* this stack size is in bytes, and is a lot larger
 * than this program needs.
 */
#define	STACKSIZE	0x2000

/* Large enough to handle maximum possible number
   of triangles, and then some */
#define	GLIST_LEN	200000
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

  Mtx   RotationX;
  Mtx   RotationY;

  Mtx   Translation;

  Mtx   TranslateIn;
  Mtx   TranslateOut;
  
  Mtx	modeling1[MAXBLOCKS+1];
  Mtx   modeling2[MAXBLOCKS+1];
  Mtx   modeling3[MAXBLOCKS+1];
  Mtx   modeling4[MAXBLOCKS+1];

  Mtx	viewing;
  Mtx   identity;
  Gfx	glist[GLIST_LEN];

  LookAt	lookat[10];
  Hilite	hilite[10];

} Dynamic;

extern Dynamic	dynamic;

extern unsigned short cfb_16_a[];
extern unsigned short cfb_16_b[];
extern unsigned short zbuffer[];

extern void      *cfb_ptrs[2];
extern int       draw_buffer;

extern Gfx       *glistp;
extern Dynamic   *dynamicp;

extern u64 *rdp_output;

extern u64 dram_stack[];	/* used for matrix stack */

extern float  theta;

extern int    ActiveController;
extern int    ControllerInput;
extern int    CreateNewCubes;
extern int    CreateNewPlanes;

extern float  TICKSPERSEC;

extern int    UseAAMode;
extern int    UseZMode;

extern float  TimePerFrame[MAXTASKS];
extern float  TimePerFrameSp[MAXTASKS];
extern float  TimePerFrameDp[MAXTASKS];

extern int    SuperBlock;

extern int    SelfScaleTimer;

extern int    numtasks;

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
extern int    ShadingMode;
extern int    UseGlobalTransforms;
extern int    ChangeVideoModes;

extern int    UseViModeX;
extern int    ScreenWidth;
extern int    ScreenWidthDelta;
extern int    rdp_flag;
extern int    OutLen;
extern int    DumpGbi;

extern int    HorizontalOffset;
extern int    VerticalOffset;

extern OSViMode *CurrentVideoModePtr;
extern OSViMode OriginalVideoMode;

float frand(void);
void  ApplyGlobalTransformations(void);

#endif	/* _LANGUAGE_C */
