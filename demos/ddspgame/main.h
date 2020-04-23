
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
 * File:	main.h
 * Create Date:	Mon Apr 17 11:45:47 PDT 1995
 *
 */
#ifndef __MAIN_H__
#define __MAIN_H__

#include <ultra64.h>
#include "sysassigned.h"
#include "htry.h"
#include "cont.h"

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

extern Dynamic	dynamic;

/* these are the static display lists */

extern Gfx 	rspinit_dl[];
extern Gfx 	rdpinit_dl[];
extern Gfx	clearcfb_dl[];
extern Gfx 	shadetri_dl[];
extern Gfx 	textri_dl[];

/* RSP address for the color frame buffer */

extern u16	cfb[][SCREEN_WD*SCREEN_HT];
extern u16	rsp_cfb[];
extern u64      dram_stack[];
extern u64      rdp_output[];

extern u8	*message[];
extern s32	kaddr[];
extern       s32             initkanchr(u8 *, s32);

#endif	/* _LANGUAGE_C */


#endif /* __MAIN_H__  */
