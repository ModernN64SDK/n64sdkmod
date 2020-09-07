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
 * File:	turbo.c
 * Creator:	hsa@sgi.com
 * Create Date:	Tue Oct 31 15:09:33 PST 1995
 *
 */

#include <ultra64.h>
#include "gtdemo.h"
#include "character.h"
#include "controller.h"
#include "timer.h"
#include "walk.h"
#include "punch.h"

/*
 * Turbo data.
 */

static Gfx	dpGlobDPBlock[] =
{
	gsDPPipeSync(),
	gsDPSetCombineMode(G_CC_SHADE, G_CC_SHADE),
	gsDPPipeSync(),
	gsDPEndDisplayList(),
};

/* global data structure */
static gtGlobState	dpGlobObjState =
{
    0xffff,						/* perspNorm */
    0x0, 0x0,						/* pad0, pad1 */
    gsDPClearOtherMode(),				/* rdpOthermode */
    { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 		/* segBases */
      0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0
    },
    {	SCREEN_WD*2, SCREEN_HT*2, G_MAXZ/2, 0,		/* viewport */
	SCREEN_WD*2, SCREEN_HT*2, G_MAXZ/2, 0,
    },
    &(dpGlobDPBlock[0])
};

/*
 * must be in BSS, not on the stack for this to work:
 */
static OSTask	turbo_tlist =
{
    M_GFXTASK,			/* task type */
    OS_TASK_DP_WAIT,		/* task flags */
    NULL,			/* boot ucode pointer (fill in later) */
    0,				/* boot ucode size (fill in later) */
    NULL,			/* task ucode pointer (fill in later) */
    SP_UCODE_SIZE,		/* task ucode size */
    NULL,			/* task ucode data pointer (fill in later) */
    SP_UCODE_DATA_SIZE,		/* task ucode data size */
    NULL,			/* task dram stack pointer */
    SP_DRAM_STACK_SIZE8,	/* task dram stack size */
    NULL,			/* task output buffer ptr (not always used) */
    NULL,			/* task output buffer size ptr */
    NULL,			/* task data pointer (fill in later) */
    0,				/* task data size (fill in later) */
    NULL,			/* task yield buffer ptr (not used here) */
    0				/* task yield buffer size (not used here) */
};


/* called while the RCP is busy rendering the background; does the
 * numerical work for the turbo objects.
 */
void
turbo_compute(Dynamic *dynamicp)
{
    Node_t	*myobj;
    float	m1[4][4];
    int		i;

    fg_cpu_start = osGetTime();

    myobj = dynamicp->characters[dynamicp->char_control];

    if (do_left_punch)
	left_punch(dynamicp, myobj);
    if (do_right_punch)
	right_punch(dynamicp, myobj);

    if (walking) {

	walk_cycle(dynamicp, myobj);

	/* update positions: */
	if (p1_dx != 0.0 || p1_dz != 0.0) {
	    myobj->xrot = -p1_tx;
	    myobj->yrot = -p1_ty;
	    myobj->zrot = -p1_tz;
	    guAlignF(m1, 0.0, myobj->xrot, myobj->yrot, myobj->zrot);
	    character_update(myobj, OBJ_MTX_LOAD, m1, 0, 0, 0);
	}
	myobj->xtrns += p1_tx;
	myobj->ztrns += p1_tz;
	character_update(myobj, OBJ_TRANSLATE, NULL, 
			 myobj->xtrns, myobj->ytrns, myobj->ztrns);
    }

    /* flatten matrices: */
    for (i=0; i<4; i++) {
	character_flatten(dynamicp, dynamicp->characters[i]);
    }

    /* sort: */
    character_sort(dynamicp);

    fg_cpu_end = osGetTime();
}

/*
 * create and send a turbo display list to the RCP.
 */
void
turbo_draw(Dynamic *dynamicp, char *staticSegment)
{
    gtGlobState	*ggsp = &(dpGlobObjState);
    OSTask	*tlistp = &turbo_tlist;
    gtGfx	*gtlistp;

    ggsp->sp.perspNorm = dynamicp->perspNorm;
    ggsp->sp.segBases[0] = 0x0;
    ggsp->sp.segBases[1] = osVirtualToPhysical(staticSegment);

    gtlistp = &(dynamicp->gtlist[0]);

    /* start the turbo display list: */
    gtlistp->obj.gstatep = (gtGlobState *) osVirtualToPhysical(ggsp);
    gtlistp->obj.statep = &dpGlobalObj;
    gtlistp->obj.vtxp = (Vtx *) NULL;
    gtlistp->obj.trip = (gtTriN *) NULL;
    gtlistp++;

    /* Now draw the turbo data: */
    character_draw(dynamicp, &gtlistp);

    WriteTimerBar(dynamicp, &gtlistp);	/* adds another turbo object */

    /* send a FullSync */
    gtlistp->obj.gstatep = (gtGlobState *) NULL;
    gtlistp->obj.statep = &dpFinalObj;
    gtlistp->obj.vtxp = (Vtx *) NULL;
    gtlistp->obj.trip = (gtTriN *) NULL;
    gtlistp++;
	
    /* end display list: */
    gtlistp->obj.gstatep = (gtGlobState *) NULL;
    gtlistp->obj.statep = (gtState *) NULL; /* signal end of object list */
    gtlistp->obj.vtxp = (Vtx *) NULL;
    gtlistp->obj.trip = (gtTriN *) NULL;
    gtlistp++;
	
    /* Build turbo graphics task */
    tlistp->t.ucode_boot = (u64 *) rspbootTextStart;
    tlistp->t.ucode_boot_size = ((u32)rspbootTextEnd - (u32)rspbootTextStart);
    tlistp->t.data_ptr = (u64 *) &(dynamicp->gtlist[0]); /* display list */
    tlistp->t.data_size = ((u32)((gtlistp - dynamicp->gtlist)*sizeof(gtGfx)));
    if (use_fifo_ucode) {
	tlistp->t.ucode = (u64 *) gspTurbo3D_fifoTextStart;/* which ucode? */
	tlistp->t.ucode_data = (u64 *) gspTurbo3D_fifoDataStart;
	tlistp->t.output_buff = rdp_output;
	tlistp->t.output_buff_size = (u64 *) ((int)rdp_output + 
					      (int)(0x400 << 3));
	tlistp->t.flags = 0x0;	/* we follow fifo task... */
    } else {
	tlistp->t.ucode = (u64 *) gspTurbo3DTextStart;/* which ucode? */
	tlistp->t.ucode_data = (u64 *) gspTurbo3DDataStart;
    }
	
    /* Write back dirty cache lines that need to be read by the RCP */
    osWritebackDCache(&dynamic, sizeof(dynamic));

    osSpTaskLoad(tlistp);
    fg_rcp_start = osGetTime();
    osSpTaskStartGo(tlistp);
}



