/*---------------------------------------------------------------------*
	Copyright (C) 1997, Nintendo.
	
	File		system.h
	Coded    by	Yoshitaka Yasumoto.	Sep 12, 1997.
	Modified by	
	
	$Id: system.h,v 1.3 1997/10/08 10:06:34 yasu Exp $
 *---------------------------------------------------------------------*/
#define	SCREEN_WD	320
#define	SCREEN_HT	240

#define	CLIP_NEAR	64
#define	CLIP_FAR	4096

#define	FOG_IN 		3000
#define	FOG_OUT		4000

#define	RDPFIFO_SIZE	(8*1024/sizeof(u64))
#define	GLIST_LEN	2048

#define	SP_BOOT_UCODE_SIZE	0x00d0	/* Actual Size is 0*00d0 */

#define	NUM_PI_MSGS	8				/* PI Message Queue      */
#define	STACKSIZE	0x2000			/* Stack Size of Thread  */

#define	STATIC_SEGMENT	1
#define	CFB_SEGMENT	2
#define	CFB_ADDRESS	0x80300000
#define	RSPBUF_ADDRESS	0x803a0000	/* If no define‚ right after cfb */

#ifdef _LANGUAGE_C
extern	OSMesgQueue	piMessageQ;
extern	OSMesgQueue	siMessageQ;
extern	OSMesgQueue	dmaMessageQ;
extern	OSIoMesg	dmaIOMessageBuf;
extern	OSMesgQueue	rspMessageQ;
extern	OSMesgQueue	rspBrkMessageQ;
extern	OSMesgQueue	rdpMessageQ;
extern	OSMesgQueue	retraceMessageQ;
extern	OSContStatus	contStatus[MAXCONTROLLERS];
extern	OSContPad	contPad[MAXCONTROLLERS];
extern	u8		contExist;

extern	u16		system_cfb[][SCREEN_WD*SCREEN_HT];
extern	u64		system_rdpfifo[];
extern	u64		system_rspyield[];

/*
 *  Macro to declare extern of WAVE data segment
 */
#define	EXTERN_SEGMENT(name)	\
extern	char	_##name##SegmentStart[], _##name##SegmentEnd[], \
                _##name##SegmentRomStart[], _##name##SegmentRomEnd[]

/*
 *  Macro to trigger segment load
 */
#define	LoadSegment(name,adrs)		\
osPiStartDma(&dmaIOMessageBuf, OS_MESG_PRI_NORMAL, OS_READ, \
	     (u32)_##name##SegmentRomStart, adrs, \
	     (u32)_##name##SegmentRomEnd - (u32)_##name##SegmentRomStart, \
	     &dmaMessageQ)
#define	LoadSegmentAfter(name,after)	LoadSegment(name,_##after##SegmentEnd)

/*
 *  Declare extern of segment
 */
EXTERN_SEGMENT(code);

/*
 *  Print Macro for DEBUG
 */
#ifdef	DEBUG
#  define	osSyncPrintf	osSyncPrintf
#else
#  define	osSyncPrintf	if (0) osSyncPrintf
#endif

#endif	/* _LANGUAGE_C */

/*======== End of system.h ========*/
