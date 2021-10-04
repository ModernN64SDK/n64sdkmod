
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

#define	STATIC_SEGMENT		1

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

  Mtx	modeling1;
  Mtx   modeling2;
  Mtx   modeling3;
  Mtx   modeling4;

  Mtx	viewing;
  Mtx   identity;
  Gfx	glist[GLIST_LEN];
} Dynamic;

extern Dynamic	dynamic;

extern unsigned short cfb_16_a[];
extern unsigned short cfb_16_b[];
extern unsigned short zbuffer[];

extern void     *cfb_ptrs[2];

/* RSP address for the color frame buffer */

extern float  theta;
extern float  TranslateHorizontal;
extern float  TranslateVertical;

extern u64 dram_stack[];

#endif	/* _LANGUAGE_C */
