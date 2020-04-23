
/*---------------------------------------------------------------------*
        Copyright (C) 1998 Nintendo.
        
        $RCSfile: sysassigned.h,v $
        $Revision: 1.1 $
        $Date: 1999/07/05 02:17:01 $
 *---------------------------------------------------------------------*/
#ifndef __const_h__
#define __const_h__

#define CORE_THREAD_PRIORITY        80
#define VIDEO_THREAD_PRIORITY 	    21
#define MAIN_THREAD_PRIORITY  	    16
#define DD_THREAD_PRIORITY			12
#define TRY_THREAD_PRIORITY 	    70
#define SI_THREAD_PRIORITY          48
#define AUDIO_THREAD_PRIORITY       68

#ifndef true
#define false 						0
#define true  						1
#endif

#define STATIC_SEGMENT          	1
#define SEGMENT_ID_CFB             	4
#define SEGMENT_ID_ZBUF             2

#define SCREEN_HT       			240
#define SCREEN_WD       			320

#define STACKSIZE       			0x2000

#define PI_MSGQ_SIZE 				8
#define RDB_SEND_BUF_SIZE   		0x20000
#define DISK_MSG_NUM 				1

#define DD_TOKENKEY_AUDIO                      1
#define DD_TOKENKEY_JSTR                       2
#define DD_TOKENKEY_ROMREAD                    4

#endif
