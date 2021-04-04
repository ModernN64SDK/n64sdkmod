/* 
 *  N64-HVQ2 library  Sample program
 * 
 *  FILE : system.h
 * 
 *  Copyright (C) 1998,1999 NINTENDO Co.,Ltd.
 * 
 */

/* 1999-02-12 */

#ifndef __SYSTEM_H__
#define __SYSTEM_H__

#define	STACKSIZE  0x2000

/**********************************************************************/

#ifndef USE_RSP
#define USE_RSP	1	/* 0: Use CPU decoder, 1: Use RSP decoder */
#endif

/**********************************************************************/

#ifdef _LANGUAGE_C

#include <ultra64.h>

#define	SCREEN_WD	320	/* Width of image [pixel] */
#define	SCREEN_HT	240	/* Height of image [pixel] */

#ifndef CFB_FORMAT
#define CFB_FORMAT	1	/* Frame buffer format 1:16bit, 2:32bit */
#endif

#if CFB_FORMAT == 2
typedef u32 CFBPix;
#define VIMODE	OS_VI_NTSC_LAN2
#else
typedef u16 CFBPix;
#define VIMODE	OS_VI_NTSC_LAN1
#endif

#define DMA_QUEUE_SIZE  200

/*
 * Size of buffer for compressed image data
 */
#define HVQ_DATASIZE_MAX  40000

/*
 * Size of data area for HVQ2 microcode 
 */
#define HVQ_SPFIFO_SIZE   20000

/*
 * in main.c
 */
void Main(void *);

/*
 * in system.c
 */
void romcpy(void *dest, void *src, int len);

extern OSMesgQueue PiMessageQ;
extern OSMesgQueue dmaMessageQ;
#if USE_RSP
extern OSMesgQueue rspMessageQ;
#endif

/*
 * in cfb.c
 */
extern CFBPix cfb[SCREEN_WD*SCREEN_HT];

/*
 *  Compressed image data segment
 */
extern char _hvqdataSegmentRomStart[], _hvqdataSegmentRomEnd[];

#endif /* _LANGUAGE_C */

#endif /* __SYSTEM_H__ */

/* end */
