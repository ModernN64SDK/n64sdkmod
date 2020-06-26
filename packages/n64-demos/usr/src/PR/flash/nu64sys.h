/*---------------------------------------------------------------------
  File   : nu64sys.h

  Created  by  Yoshitaka Yasumoto.   Jul,11 1995.
  Modified by  
  
  Copyright by Nintendo, Co., Ltd. 1995.
  ---------------------------------------------------------------------*/
#ifndef	_nu64sys_h_
#define	_nu64sys_h_

#define	public
#define	private	static

#define SCREEN_LOW	0
#define SCREEN_HIGH	!SCREEN_LOW

#if	SCREEN_LOW
#define SCREEN_WD	320
#define SCREEN_HT	240
#else
#define SCREEN_WD	640
#define SCREEN_HT	480
#endif

#define STACKSIZE	0x2000

#define	n_WaitMesg(a)	osRecvMesg(&n_##a##MessageQ, NULL, OS_MESG_BLOCK)
#define	n_TestMesg(a)	osRecvMesg(&n_##a##MessageQ, NULL, OS_MESG_NOBLOCK)

#ifdef _LANGUAGE_C
extern	OSMesgQueue	n_dmaMessageQ,	n_rdpMessageQ;
extern	OSMesgQueue	n_rspMessageQ,	n_retraceMessageQ;
extern	OSMesgQueue	n_siMessageQ;
#endif

#endif
