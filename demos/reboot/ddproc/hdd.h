
/*---------------------------------------------------------------------*
        Copyright (C) 1998 Nintendo.
        
        $RCSfile: hdd.h,v $
        $Revision: 1.1 $
        $Date: 1999/07/05 02:17:00 $
 *---------------------------------------------------------------------*/

#ifndef _hdd_h_
#define _hdd_h_

#include <ultra64.h>
#include <PR/leo.h>

#define READWRITE_BYTESIZE   		0x10000 /* 65k about 3 block */

#define ERRBUF_SIZE     			7
#define NUM_LEO_MESGS   			8
#define DISK_MSG_NUM 				1

#define DDSTAT_GOOD				0
#define DDSTAT_IDLE				0
#define DDSTAT_INSERTED			0
#define DDSTAT_CONTINUE    		0x10
#define DDSTAT_KILLED			0x20
#define DDSTAT_DEAD				0x40
#define DDSTAT_BUSY       		0x80
#define DDSTAT_DIALOGUE			0x100

#define ERRMES__DRIVE_NOT_FOUND		0x01
#define ERRMES__ACCESSLAMP			0x02
#define ERRMES__WRONG_DISK	 		0x04
#define ERRMES__NOW_IN_RESTORE      0x08 
#define ERRMES__ACCESS_TO_NOFORMAT_AREA 0x10
#define ERRMES__TORISETSU_YOME      0x11 
#define ERRMES__SET_CLOCK           0x12
#define ERRMES__RAMPACK_NOT_CONNECTED	0x14
#define ERRMES__MANUAL_YOME 		0x18

#define ERRMES__INSERT_AGAIN        0x100
#define ERRMES__DISK_WO_IRERO		0x200
#define ERRMES__DISK_WO_NUKE        0x400

#define ERRMES__UNEXPECTED			0x20
#define ERRMES__UNDEFINED	   		0x21

#define LEO_ERROR_DDROMCART_NOT_CONNECTED    105
#define ERRMES__DDROMCART_NOT_CONNECTED    0x22

#define LEOCMD_INITIALIZE 			61
#define LEOCMD_GETID 				13

#define LEOCMD_ACTIVE   			5
#define LEOCMD_STANDBY  			6
#define LEOCMD_SLEEP    			7
#define LEOCMD_BRAKE    			8
#define LEOCMD_EJECT    			32

#define LEOCMD_LBAREAD  			23
#define LEOCMD_LBAWRITE 			24
#define LEOCMD_BYTEREAD				25
#define LEOCMD_BYTEWRITE			26

#define LEOCMD_GETRTC			    30
#define LEOCMD_SETRTC			    31

#define LEOCMD_IDLE					0
#define LEOCMD_SEEK					2
#define LEOCMD_REZERO				1

#define LEOCMD_TESTUNIT    			28
#define LEOCMD_SETMOTORTIME			27

#define LEOCMD_TEST  				99

#define UNITMODE_LBA 				0
#define UNITMODE_BYTE   			1

#define LEO_ERROR_USER_DEFINED     	101
#define LEO_ERROR_MODE_INCOMPLETE	100
#define LEO_ERROR_PROGRAM_FAULT		102
#define LEO_ERROR_UNEXPECTED_ACCIDENT	103
#define LEO_ERROR_BUFFER_OVERFLOW	104
#define LEO_ERROR_ILLEGAL_MOTORTIME	105

#define LEO_ERROR_CHECKSUM	        LEO_ERROR_UNRECOVERED_READ_ERROR

#define TRANSMIT_LOWRATE  			1   /* Transfer efficiency under 1 MB by convention */

#define SEC_SIZE_ZONE0        232    /* bytes/sector in zone-0 */

#define DD_MAX_LBA					4291
#define DD_MAX_BYTE					64458560
#define DD_MAX_BUFSIZE				8000000

typedef struct {
  s32   mode;			        /* GOOD,BUSY,CONTINUE,KILLED,DEAD,DIALOG */
  s32   errno;                  /* Error value of the last LEO function last executed. */   
  s32 	errmes;			        /* For error message display. */
  s32   counter;				/* The reset counter */
  s32   reserved;				
  u32   buffer_size;              /*Size of the data read. */
  u32   position;               /* Head position (LBA). */
  u32   read_offset;            /* Offset for reading from buffer. */
} TDDStatus;

typedef struct{
  s32   command;  
  s32 	locked;		/* 0 means available , non 0 means this hasnt reach yet */
  struct
  {
	s32   hi;
	s32   lo;
	s32   rw;
	s32   offs;
  }param;
  OSMesgQueue 	*retQ;
  s32   		retVal;
} TMessage4dd;

/* public members of "diskproc" class */

#define dd_reset        LeoReset

extern  OSMesgQueue     ddflushQ;

extern  s32 			dd_create(void);
extern  s32 			dd_exec( TMessage4dd * msg );
extern  s32 			dd_saveID(LEODiskID * det);

extern  u32				dd_getVersion(void);
extern  s32 			dd_getBlockSize(s32 lba,s32 size);

extern  void            dd_change_mode_USA(void);
extern  s32      		dd_checkGate( TMessage4dd * msg );
extern  void            dd_errHook(s32 func_no , s32 err_no , s32 count );
extern  s32             dd_thaw(s32 target_err);
extern  void            dd_flush(void);

extern  void 			dd_killCommand(void);

/* gets information */
extern  TDDStatus      *dd_getStatus(void);
extern  OSPiHandle     *dd_getHandle(void);
extern  u8             *dd_getDefaultBuffer( void );
extern  s32             dd_driveExist(void);

/* execlusive control */
typedef s32 TDDToken;
extern  TDDToken 		dd_getToken(TDDToken token, s32 _block);
extern  void            dd_freeToken(TDDToken token);

#endif



