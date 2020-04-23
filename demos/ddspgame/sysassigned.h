/*---------------------------------------------------------------------*
        Copyright (C) 1998 Nintendo.
        
        $RCSfile: sysassigned.h,v $
        $Revision: 1.2 $
        $Date: 1999/01/06 10:43:51 $
 *---------------------------------------------------------------------*/

#define	STATIC_SEGMENT		1
#define	CFB_SEGMENT		2

#define	SCREEN_HT	240
#define	SCREEN_WD	320

/* this stack size is in bytes, and is a lot larger
 * than this program needs.
 */
#define	STACKSIZE	0x2000

#define	GLIST_LEN	2048
#define RDP_OUTPUT_LEN  (4096*16)

#define	ERROR_MES_SIZE	26
#define DD_THREAD_PRIORITY			12

#ifndef true
#define false 						0
#define true  						1
#endif
