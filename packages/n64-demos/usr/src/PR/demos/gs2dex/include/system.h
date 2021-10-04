/*---------------------------------------------------------------------
	Copyright (C) 1997, Nintendo.
	
	File		system.h
	Coded    by	Yoshitaka Yasumoto.	Feb  3, 1997.
	Modified by	
	Comments	
	
	$Id: system.h,v 1.8 1999/04/16 07:49:14 yoshida Exp $
  ---------------------------------------------------------------------*/
#define	SCREEN_WD	320
#define	SCREEN_HT	240

#define	RDPFIFO_SIZE	(4096/sizeof(u64))
#define	GLIST_LEN	512

#define	SP_BOOT_UCODE_SIZE	0x00d0	/* The real size is 0x00d0 */

#define	NUM_PI_MSGS	8		/* PI message queue     */
#define	STACKSIZE	0x2000		/* Thread stack size */

#define	STATIC_SEGMENT	1
#define	BG_SEGMENT	2
#define	CFB_ADDRESS	0x80300000

#ifndef __MWERKS__
#if 1
#define	RSPBUF_ADDRESS	0x803a0000	/* Just after cfb if no "define" */
#endif
#endif                              /* With Code Warrior, forcedly defined right after cfb. */

#ifdef _LANGUAGE_C
extern	OSMesgQueue	piMessageQ;
extern	OSMesgQueue	siMessageQ;
extern	OSMesgQueue	dmaMessageQ;
extern	OSIoMesg	dmaIOMessageBuf;
extern	OSMesgQueue	rspMessageQ;
extern	OSMesgQueue	rdpMessageQ;
extern	OSMesgQueue	retraceMessageQ;
extern	OSContStatus	contStatus[MAXCONTROLLERS];
extern	OSContPad	contPad[MAXCONTROLLERS];
extern	u8		contExist;

extern	u16		system_cfb[][SCREEN_WD*SCREEN_HT];
extern	u64		system_rdpfifo[];
extern	u64		system_rspyield[];

/*
 *  Macro for WAVE data segment's extern declarations
 */
#define	EXTERN_SEGMENT(name)	\
extern	char	_##name##SegmentStart[], _##name##SegmentEnd[], \
                _##name##SegmentRomStart[], _##name##SegmentRomEnd[]
extern char _codeSegmentBssEnd[];

/*
 *  Segment's extern declarations
 */
EXTERN_SEGMENT(code);
EXTERN_SEGMENT(static);
EXTERN_SEGMENT(bg_rgba);
EXTERN_SEGMENT(bg_ci);

/*
 *  Print macro for DEBUG
 */
#ifdef	DEBUG
#  define	osSyncPrintf	osSyncPrintf
#else
#  define	osSyncPrintf	if (0) osSyncPrintf
#endif

#endif	/* _LANGUAGE_C */

/*======== End of system.h ========*/
