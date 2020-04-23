
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
 * File:	overlay.h
 * Create Date:	Mon Apr 17 11:45:47 PDT 1995
 *
 */

#ifndef _LANGUAGE_MAKEROM
#include <ultra64.h>
#endif

#define	STATIC_SEGMENT		1
#define	CFB_SEGMENT		2

#define	SCREEN_HT	240
#define	SCREEN_WD	320

/* this stack size is in bytes, and is a lot larger
 * than this program needs.
 */
#define	STACKSIZE	0x2000

#define	GLIST_LEN	2048
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
	Mtx	modeling;
	Mtx	viewing;
        Mtx     identity;
	Gfx	glist[GLIST_LEN];
} Dynamic;

/* exports from plain.c and texture.c (overlays) */

#ifdef __MWERKS__
extern void	drawSquare_Plain(OSTask *, int);
extern void	drawSquare_Texture(OSTask *, int);
#else
extern void	drawSquare(OSTask *, int);
#endif
/* exports from dram_stack.c */

extern u64 dram_stack[];	/* used for matrix stack */

/* exports from cfb.c */

extern u16	cfb[][SCREEN_WD*SCREEN_HT];	/* CPU address for frame bufs */

/* exports from rsp_cfb.c */

extern u16	rsp_cfb[];	/* RSP address for frame buffer */

#endif	/* _LANGUAGE_C */
