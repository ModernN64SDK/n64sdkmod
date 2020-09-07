
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
 * File:	gtdemo.h
 * Creator:	hsa@sgi.com
 * Create Date:	Tue Oct 31 14:21:46 PST 1995
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

#include <PR/gt.h>

typedef struct {
    float	x, y, z;
} xyz_t;

/*
 * each 'character' is composed of a hierarchical group of these nodes.
 * each node describes an individual body part.
 */
typedef struct {

    int		id;
    int		flags;
    char	name[64];

    struct Node_t	*parent;	/* pointer to parent */
    struct Node_t	*children;	/* pointer to descendents */
    struct Node_t	*sibling;	/* next equal-level node */

    gtState	obj_state;

    float	xtrns, ytrns, ztrns;	/* holds transform information */
    float	xscl, yscl, zscl;
    float	xrot, yrot, zrot;

    float	my_locate[4][4];	/* modeling matrix really */
    float	my_transform[4][4];	/* movement matrix */
    float	my_cumulative[4][4];

    xyz_t	orig_centroid;	/* for z-sort */
    xyz_t	centroid;

    Vtx		*vtx;		/* my points */
    gtTriN	*tri;		/* my triangles */

} Node_t;



/*
 * Layout of our dynamic segment
 */
typedef struct {
	float	projection[4][4];
	float	viewing[4][4];
	Mtx	projectionL;
	Mtx	viewingL;

	u16	perspNorm;

	LookAt	lookat[2];
	Hilite	hilite[2];

        Mtx     floor_model[9];		/* for tiled floor */

	int	free_node;		/* next available node */
	Node_t	obj_nodes[64];		/* node pool */
	Node_t	*char_order[64];	/* node list in sorted order */

	int	char_control;		/* which guy do we control? */
	Node_t	*characters[4];		/* "head" of character tree */

	gtGfx	gtlist[256];		/* turbo display list buffer */
	Gfx	glist[512];		/* fast display list buffer */
} Dynamic;


/*
 * external variables:
 */

extern u64 rdp_output[];
extern u64 rdp_output_len;

extern Dynamic	dynamic;
extern Gfx	*glistp;
extern u32	frame_count;

extern unsigned short 	cfb_16_a[];
extern unsigned short 	cfb_16_b[];
extern unsigned short	zbuffer[];

extern Gfx 	rspinit_dl[];
extern Gfx 	rdpinit_dl[];
extern Gfx 	floor_setup_dl[];
extern Gfx 	floor_geom_dl[];

extern gtState	dpGlobalObj;
extern gtState	dpFinalObj;


#endif	/* _LANGUAGE_C */
