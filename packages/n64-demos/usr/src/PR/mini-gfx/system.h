/*---------------------------------------------------------------------
  $Id: system.h,v 1.1.1.1 2002/05/02 03:27:18 blythe Exp $
  
  File : system.h

  Coded     by Yoshitaka Yasumoto.   Apr 11, 1997.
  Copyright by Nintendo, Co., Ltd.           1997.
  ---------------------------------------------------------------------*/
#define	SCREEN_WD	320
#define	SCREEN_HT	240

#define RDPFIFO_SIZE    (8*1024/sizeof(u64))
#define GLIST_LEN       2048

#define SP_BOOT_UCODE_SIZE      0x00d0  /* 本当のサイズは 0x00d0 */

#define	NUM_PI_MSGS	8		/* PI メッセージキュー      */
#define	STACKSIZE	0x2000		/* スレッドのスタックサイズ */

#define	STATIC_SEGMENT	1
#define CFB_SEGMENT     2
#define CFB_ADDRESS     0x80300000
#define RSPBUF_ADDRESS  0x803a0000      /* define がなければ cfb の直後に */

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
extern  u64             system_rdpfifo[];
extern  u64             system_rspyield[];

/*
 *  DEBUG 用プリントマクロ
 */
#ifdef	DEBUG
#  define	osSyncPrintf	osSyncPrintf
#else
#  define	osSyncPrintf	if (0) osSyncPrintf
#endif

#endif	/* _LANGUAGE_C */

/*======== End of system.h ========*/
