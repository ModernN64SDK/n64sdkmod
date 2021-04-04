
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
 * File:	game.h
 * Create Date:	Thu Dec 14 13:52:23 PST 1995
 *
 */

#include "boot.h"

/*
 * Message queues
 */
extern OSMesgQueue
                rdpMessageQ,
                retraceMessageQ;

/*
 * global variables
 */
extern char	*staticSegment;
extern char	*textureSegment;

#define	GLIST_LEN	2048

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
	Mtx	modeling;
	Mtx	modeling2;
	Mtx	viewing;
	LookAt	lookat;
	Hilite	hilite;
	Lightsn	light;
	Gfx	glist[GLIST_LEN];
} Dynamic;

extern Dynamic	dynamic;

/*
 * frame buffer symbols
 */
extern u16	zbuffer[];			/* RAM address */
extern u16	cfb[][SCREEN_WD*SCREEN_HT];	/* RAM address */
extern u16	rsp_cfb[];			/* segment address */

/* RDP's buffer 
 */
extern u64 dram_stack[]; /* used for matrix stack */
extern u64 rdp_output[]; /* buffer for RDP DL */
