/*====================================================================
 * gfx.h
 *
 * Synopsis:
 *
 * Graphics constants and structures.
 *
 *
 * Copyright 1993, Silicon Graphics, Inc.
 * All Rights Reserved.
 *
 * This is UNPUBLISHED PROPRIETARY SOURCE CODE of Silicon Graphics,
 * Inc.; the contents of this file may not be disclosed to third
 * parties, copied or duplicated in any form, in whole or in part,
 * without the prior written permission of Silicon Graphics, Inc.
 *
 * RESTRICTED RIGHTS LEGEND:
 * Use, duplication or disclosure by the Government is subject to
 * restrictions as set forth in subdivision (c)(1)(ii) of the Rights
 * in Technical Data and Computer Software clause at DFARS
 * 252.227-7013, and/or in similar or successor clauses in the FAR,
 * DOD or NASA FAR Supplement. Unpublished - rights reserved under the
 * Copyright Laws of the United States.
 *====================================================================*/

/*---------------------------------------------------------------------*
        Copyright (C) 1998 Nintendo. (Originated by SGI)
        
        $RCSfile: gfx.h,v $
        $Revision: 1.1.1.1 $
        $Date: 2002/05/02 03:27:21 $
 *---------------------------------------------------------------------*/


#ifndef __simplegfx__
#define __simplegfx__

#include <sched.h>

#define	SCREEN_HT	240
#define	SCREEN_WD	320
#define GFX_DL_BUF_SIZE	6000

#ifdef _LANGUAGE_C /* needed because file is included by "spec" */

/*
 * Layout of our dynamic segment
 */
typedef struct {
	Mtx	projection;
	Mtx	viewing;
        Mtx     bg_model;
        Mtx     logo_scale;
        Mtx     logo_rotate;
        Mtx     logo_trans;
	Gfx	glist[512];	/* buffer to hold display list */
} Dynamic;

typedef union {    

    struct {
        short   type;
    } gen;
    
    struct {
        short   type;
    } done;
    
    OSScMsg      app;
    
} GFXMsg;

typedef struct {
    OSScTask    task;
    Dynamic     dp;
    GFXMsg      msg;
    u16		*cfb;
} GFXInfo;

/* dynamic segment structure: */
extern Dynamic	dynamic;

/* some static display lists: */
extern Gfx	rdpstateinit_dl[];
extern Gfx	setup_rdpstate[];
extern Gfx	setup_rspstate[];
extern Gfx 	logo_dl[];
extern Gfx 	bg_dl[];

/* global pointer for display list: */
extern Gfx	*glistp;

/* frame buffer, zbuffer: */
extern unsigned short cfb_16_a[];
extern unsigned short cfb_16_b[];
extern unsigned short	zbuffer[];

/* yield buffer: */
extern u64	gfxYieldBuf[];

/* matrix stack buffer: */
extern u64          dram_stack[];

/* rdp fifo buffer */
extern u64	rdp_output[];

#endif	/* _LANGUAGE_C */
#endif /* __simplegfx__ */
