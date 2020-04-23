/*---------------------------------------------------------------------*
        Copyright (C) 1998 Nintendo.
        
        $RCSfile: hdd_internal.h,v $
        $Revision: 1.1 $
        $Date: 1999/07/05 02:17:00 $
 *---------------------------------------------------------------------*/

/*
 * Standard processing sequence for DD
 */ 

/* pre-process */

#include <ultra64.h>
#include <PR/leo.h>
#include "sysassigned.h"
#include "hdd.h"

#define  USE_TOKEN

/* declarerations */

typedef struct{
  s32 target;
  s32 cnt;
  s32 errno;
} THookError;

/*ALHeap dd_heap;                          /* heap structur */
/*u8     dd_heap_buf[READWRITE_BYTESIZE];     /* heap buffer  */

THookError _hook={0};

OSPiHandle             *_dev_handler = 0;
s32                     dd_errbuf[ERRBUF_SIZE+1]={0};

static u8              *_rwbuf;
static s32              _rwbuf_size; 
static OSMesg           LeoMessages[NUM_LEO_MESGS];
static TDDStatus  		dd_stat;

static OSMesgQueue     	diskMsgQ,diskQ,frozenQ,ddfirstQ;
#ifdef USE_TOKEN
OSMesgQueue dd_tokenQ;
static OSMesg dd_tokenQBuf[1];
#endif
OSMesgQueue     		ddflushQ; 
static OSMesg   		diskMsgBuf[DISK_MSG_NUM],diskQBuf[1],ddflushQBuf[1]
					,frozenQBuf[1],ddfirstQBuf[1];

static s32              _CA = 0; 

static OSMesgQueue     	recDiskMsgQ;
static OSMesg   		recDiskMsgBuf[1];

LEODiskID		_diskID ,_savedID;
static LEODiskTime      _rtc;
static LEOCmd			_cmdBlk;
static s32				error;
static s32 				_save_id = 1; 
static TMessage4dd 		dd_firstMsg = {0};
static s32              _created = 0;
static s32              _not_initialized = 1;

/* local functions */

static void     dd_closeDialog(s32 mask);
static s32      dd_openDialog(s32 mes);

static s32     	dd_init( void );

static s32     	dd_read(s32 start_adr , s32 size ,s16 flag_convert );
static s32      dd_lbaWrite(s32 start_adr ,s32 size);
static s32     	dd_seek( s32 adr , s32 flag_convert );
static s32     	dd_rezero( void );

static s32     	dd_setMotorTime( u32 standby , u32 sleep );
static s32     	dd_calmDown( s16 flag );
static s32     	dd_activate( s16 flag );

static s32      dd_readRTC(void);
static s32      dd_setRTC(LEODiskTime *rtcp);
static s32      dd_testUnit(LEOStatus *status);

static s32     	dd_checkEject( void );
static s32     	dd_checkInsert( void);

static void    	coreObjectOfdd( void * );
void     dd_wait(void); 
static void     _setBuffer(u8 *buf);

static  s32     dd_checkSum(void);
static  s32     dd_makeSum(void);
static  s32     dd_restoreData(void);

static void     _lockToken(void);
static void     _unlockToken(void);

enum leofunc_t
{
  TLeoCreateLeoManager,
  TLeoResetClear,
  TLeoReadDiskID,
  TLeoReadWrite,
  TLeoSeek,
  TLeoRezero,
  TLeoTestUnitReady,
  TLeoModeSelectAsync,
  TLeoSpdlMotor,
  TLeoReadRTC,
  TLeoSetRTC
};

#if 0 
/*
   Discussion:

   Description of dd_stat.mode

   DDSTAT_GOOD  : Normal termination.
   DDSTAT_IDLE  : Awaiting command.
   DDSTAT_BUSY  : Performing disk processing. 
   DDSTAT_KILLED : Cases such as forced termination by LeoClearQueue 	(CMD_TERMINATED).  Whether data are restored depends on the application. 
   DDSTAT_DEAD  : Abnormal termination.  Restoration not performed. 


   Option macros:

   DEBUG : Valid only when debugging.
   START_FROM_CASSETTE : For cassette startup.
   NO_USE_RTC          : RTC not used.
   NEED_DISK           : Do not complete initialization if no disk present (Only with cassette startup).
   			 

*/
#endif
