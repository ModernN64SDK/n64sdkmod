/*---------------------------------------------------------------------*
        Copyright (C) 1998 Nintendo.
        
        $RCSfile: dd_func.c,v $
        $Revision: 1.5 $
        $Date: 1998/12/17 08:37:16 $
 *---------------------------------------------------------------------*/

/*
 * Standard processing sequence for DD (member functions)
 */ 

/* local func */

static s32 dd_leoRecv(s32 *err_code,s32 func_no );
static s32 dd_catchError(s32 *err_code,s32 func_no);
static void    	dd_pushError(s32 err);
static s32     	dd_errSeqDiskSeek( s32 err_code );
static s32     	dd_errSeqDiskChange(void);
static s32     	dd_confirmDisk( void );
static s32     	dd_defaultErrors(void);
static s32      dd_freeze( s32 errmes);
#ifdef START_FROM_CASSETTE
static void     dd_setErrMes(s32 errmes);
#endif

static s32      _bootstep=0;

/* implementation */

#define LeoRecv(err_code,func_no) {\
							 ret = dd_leoRecv(&err_code,func_no);\
							 if(ret) return ret;\
							 }

#define LeoRecv0(err_code,func_no) {\
							ret = dd_catchError(&err_code,func_no);\
							if(ret) return ret;\
							}

s32
dd_leoRecv(s32 *err_code,s32 func_no)
{
  osRecvMesg(&diskQ, (OSMesg *)&error, OS_MESG_BLOCK);
  *err_code = error;
  return dd_catchError(err_code,func_no);
}

s32
dd_catchError(s32 *ecode,s32 func_no )
{
  s32 ret,err_code;

  if( _hook.cnt > 0  && (func_no == _hook.target) )
	{
	  _hook.cnt--;
	  *ecode = _hook.errno;
	}

  err_code = *ecode;
  dd_pushError(err_code);
  if( err_code == LEO_ERROR_COMMAND_TERMINATED || err_code == LEO_ERROR_WAITING_NMI )
	{
	  dd_closeDialog(0);
	  ret = DDSTAT_KILLED;
	} 
  else if( err_code == LEO_ERROR_QUEUE_FULL )
	{
	  ret = DDSTAT_CONTINUE; 
	}
  else
	ret = 0;
  
  return ret;
}

#ifdef START_FROM_CASSETTE
void
dd_setErrMes( s32 errmes )
{
  dd_stat.errmes = errmes;			
}
#endif

void
dd_pushError( s32 err )
{
  s16           i;

  if(dd_errbuf[0] != err )
  {	
    for (i = ERRBUF_SIZE-1 ; i > 0 ; i -- ) 
	dd_errbuf[i] = dd_errbuf[i-1];
    dd_errbuf[0] = err;
  }

  dd_stat.errno = err;
}

s32
dd_defaultErrors(void)
{
  dd_stat.errmes = ERRMES__TORISETSU_YOME;	  
  return DDSTAT_DEAD;
}


s32
dd_setMotorTime( u32 standby , u32 sleep )
{
  s32    ret=-1,err_code = -1;
  
  if( standby <= 10 && sleep <= 10 )
	{
	  LeoModeSelectAsync(&_cmdBlk, (u32)standby, (u32)sleep, &diskQ);
	  LeoRecv(err_code,TLeoModeSelectAsync);
	}
  else
	{
	  err_code = LEO_ERROR_ILLEGAL_MOTORTIME;
	  LeoRecv0(err_code,TLeoModeSelectAsync);
	}

  switch( err_code )
    {
    case LEO_ERROR_GOOD:
      ret = DDSTAT_GOOD;
      break;
    default :
      ret = dd_defaultErrors();
      break;
     }

  return ret;
}

s32
dd_calmDown( s16 flag ) {
  s32  ret=-1 , err_code = -1;

  LeoSpdlMotor(&_cmdBlk, flag , &diskQ);
  LeoRecv(err_code,TLeoSpdlMotor);

  switch( err_code )
    {
    case 0:
    case LEO_ERROR_MEDIUM_NOT_PRESENT:
      ret = DDSTAT_GOOD;
      break;
    default:
      ret = dd_defaultErrors();
      break;
    }
  return ret;
}

s32
dd_activate( s16 flag ) {
  s32    ret = -1, err_code = -1;

  LeoSpdlMotor(&_cmdBlk, flag , &diskQ);
  LeoRecv(err_code,TLeoSpdlMotor);
  
  switch( err_code )
    {
    case LEO_ERROR_MEDIUM_MAY_HAVE_CHANGED:
	case LEO_ERROR_UNRECOVERED_READ_ERROR:
      return dd_defaultErrors();
    default:
      break;
    }

  return dd_errSeqDiskSeek( err_code );
}

s32
dd_checkEject( void ) {
  s32           ret=-1,err_code=-1;

  LeoSpdlMotor(&_cmdBlk, LEO_MOTOR_BRAKE, &diskQ);
  LeoRecv(err_code,TLeoSpdlMotor);

  switch( err_code ) 
	{
	case 0:
	  ret = DDSTAT_CONTINUE | dd_openDialog( ERRMES__DISK_WO_NUKE );
	  break;
	case LEO_ERROR_MEDIUM_NOT_PRESENT:
	  dd_closeDialog(0x00FF);  /* Clear area 3 */
	  ret = DDSTAT_GOOD;
	  break;
	default:
	  ret = dd_defaultErrors();
	}
  return ret;
}


s32
dd_rezero(void)
{
  s32 ret = -1, err_code=-1;
  LeoRezero(&_cmdBlk , &diskQ );
  LeoRecv(err_code,TLeoRezero);

  return dd_errSeqDiskSeek( err_code );
}

s32
dd_lbaWrite(s32 start_adr ,s32 size)
{
  s32 	start_lba,lba_size , byte_size;
  s32   ret=-1,err_code = -1;

  start_lba = start_adr;
  lba_size  = size;
  dd_stat.read_offset = 0;
  LeoLBAToByte( start_lba , lba_size , &byte_size );
  dd_stat.buffer_size = byte_size;
    
  if( byte_size > _rwbuf_size )
	{
	  err_code = LEO_ERROR_BUFFER_OVERFLOW; 
	  LeoRecv0(err_code,TLeoReadWrite);
	}
  else
	{
	  dd_makeSum();
	  LeoReadWrite(&_cmdBlk, OS_WRITE , (u32)start_lba ,_rwbuf, lba_size , &diskQ );	
	  LeoRecv(err_code,TLeoReadWrite);
	}

  return dd_errSeqDiskSeek( err_code );
}

s32
dd_init(void)
{
  s32  err_code,ret;
  
  switch(_bootstep)
	{
	case 0:
#ifndef START_FROM_CASSETTE
	  err_code = LeoCreateLeoManager((OSPri)OS_PRIORITY_LEOMGR - 1,
								(OSPri)OS_PRIORITY_LEOMGR,
								LeoMessages, NUM_LEO_MESGS);
#else
	  if( !_CA )
		err_code = LeoCJCreateLeoManager((OSPri)OS_PRIORITY_LEOMGR - 1,
									(OSPri)OS_PRIORITY_LEOMGR,LeoMessages, NUM_LEO_MESGS);
	  else
		err_code = LeoCACreateLeoManager((OSPri)OS_PRIORITY_LEOMGR - 1,
									(OSPri)OS_PRIORITY_LEOMGR,LeoMessages, NUM_LEO_MESGS);
#endif
	  LeoRecv0(err_code,TLeoCreateLeoManager);
	  
	  if( err_code )
		{
#ifdef START_FROM_CASSETTE
		  if ( err_code == LEO_ERROR_DEVICE_COMMUNICATION_FAILURE )
			{
			  dd_openDialog( ERRMES__DRIVE_NOT_FOUND );
			  ret = DDSTAT_KILLED;
			}
		  else
			ret = dd_defaultErrors();
#endif
		  break;
		}
	  else
		{
#ifdef START_FROM_CASSETTE
		  if( osGetMemSize() < 0x00800000L )
			{
			  dd_pushError( LEO_ERROR_RAMPACK_NOT_CONNECTED ) ;
			  dd_setErrMes( ERRMES__RAMPACK_NOT_CONNECTED );
			  ret = DDSTAT_DEAD;
			  break;
			}
		  else
#endif			
			  ++_bootstep;
		}

#ifdef START_FROM_CASSETTE /*  With disk startup, the following processing is unnecessary because it is performed by the IPL. */
	case 1:
	  while(1)
		{
		  LeoResetClear();
		  ret = dd_checkInsert();
		  if( !( ret & DDSTAT_CONTINUE ) ) break;
		  dd_wait();
		}
	  if( ret ) break;
	  ++_bootstep;

	case 2:
#ifndef NO_USE_RTC
	  if( ret = dd_readRTC() ) break;
#endif
	  ++_bootstep;
#endif

	default:	
	  break;
	  
	}
  return ret;	
}

s32
dd_readRTC(void)
{
  s32 ret = -1;
  static s32 err_code = 0;

  LeoReadRTC(&_cmdBlk, &diskQ);
  LeoRecv(err_code,TLeoReadRTC);  

  switch( err_code )
	{
	case LEO_ERROR_GOOD:
	  bcopy( &_cmdBlk.data.time, &_rtc, sizeof(LEODiskTime));
	  ret = 0;
	  break;
	case LEO_ERROR_REAL_TIME_CLOCK_FAILURE:
#ifdef START_FROM_CASSETTE
	  if( not_initialized )
		{
		  ret = DDSTAT_KILLED | dd_openDialog( ERRMES__SET_CLOCK );
		  break;
		}
#endif
	  ret = dd_defaultErrors();
	  break;
	default:
	  ret = dd_defaultErrors();
  }

  return ret;
}

s32
dd_setRTC(LEODiskTime *rtcp)
{
  s32 ret = -1,err_code = -1;
  
  LeoSetRTC( &_cmdBlk ,rtcp ,&diskQ );
  LeoRecv(err_code,TLeoSetRTC);  

  switch( err_code )
	{
	case LEO_ERROR_GOOD:
	  ret = DDSTAT_GOOD;
	  break;
	default:
	  ret = dd_defaultErrors();
	}

  return ret;
}

s32
dd_testUnit(LEOStatus *status)
{
  s32 ret = -1 , err_code = -1;
 
  err_code = LeoTestUnitReady(status);
  LeoRecv0(err_code,TLeoTestUnitReady);

  switch( err_code )
	{
	case LEO_ERROR_GOOD:
	case LEO_ERROR_MEDIUM_NOT_PRESENT:
	case LEO_ERROR_MEDIUM_MAY_HAVE_CHANGED:
	  ret = 0;
	  break;
 	case LEO_ERROR_BUSY:
	  ret = DDSTAT_CONTINUE;
	  break;
	default:
	  ret = dd_defaultErrors();
	}
  return ret;
}
  
s32
dd_read(s32 start_adr , s32 size ,s16 flag_convert )
{
  s32  start_lba,lba_size , actual_byte , byte_size;
  s32  err_code = -1,ret=-1;
  LEOCapacity cap;

  if (flag_convert)
	{
	  start_adr &= 0x7FFFFFFF;
	  size  &= 0x7FFFFFFF;
	  LeoByteToLBA(0 , start_adr , &start_lba);
	  LeoLBAToByte(0 , (u32)start_lba , &actual_byte);
	  if ( actual_byte > start_adr )
		{
		  --start_lba;
		  LeoLBAToByte(0 , (u32)start_lba , &actual_byte);
		}
	  dd_stat.read_offset = start_adr - actual_byte;
	  LeoByteToLBA(start_lba , (u32)size + dd_stat.read_offset , &lba_size);
	}
  else
	{
	  start_lba = start_adr;
	  lba_size  = size;
	  dd_stat.read_offset = 0;
	}

  LeoLBAToByte( start_lba , lba_size , &byte_size );
  dd_stat.position = start_lba;
  dd_stat.buffer_size = byte_size; /* used by error 23 */

  if( byte_size > _rwbuf_size )
	{
	  err_code = LEO_ERROR_BUFFER_OVERFLOW; 
	  LeoRecv0(err_code,TLeoReadWrite);
	}
  else
	{
	  LeoReadWrite(&_cmdBlk, OS_READ , (u32)start_lba ,
			   _rwbuf, lba_size , &diskQ );	
	  LeoRecv(err_code,TLeoReadWrite);
	}

  /*  check sum */

  if( !err_code )
	{
	  LeoReadCapacity( &cap , OS_WRITE );
	  err_code = ((u32)start_lba < cap.startLBA ) ? 0 : dd_checkSum();
	  dd_stat.position = start_lba + lba_size;
	}
  
  return dd_errSeqDiskSeek( err_code );
}


s32
dd_seek(s32 start_adr , s32 flag_convert )
{
  s32 	ret = -1,start_lba,actual_byte,err_code = -1;

  if (flag_convert)
	{
	  start_adr &= 0x7FFFFFFF;
	  LeoByteToLBA(0 , start_adr , &start_lba);
	  LeoLBAToByte(0 , (u32)start_lba , &actual_byte);
	  if ( actual_byte > start_adr ) --start_lba;
	}
  else
	start_lba = start_adr;

  dd_stat.position = start_lba;    /*  for error 23 */
 
  LeoSeek(&_cmdBlk , (u32)start_lba , &diskQ );
  LeoRecv(err_code,TLeoSeek);

  return dd_errSeqDiskSeek( err_code );
}


s32
dd_errSeqDiskSeek( s32 err_code )
{
  s32 			ret,mes=0; 
  LEOCapacity   cap;  

  switch(err_code)
	{
	case LEO_ERROR_GOOD:
	  ret = DDSTAT_GOOD;
	  break;
	case LEO_ERROR_MEDIUM_NOT_PRESENT:
	  mes = ERRMES__DISK_WO_IRERO;
	  goto DCHANGE;
	case LEO_ERROR_EJECTED_ILLEGALLY_RESUME:
	  mes = ERRMES__ACCESSLAMP;
	  goto DCHANGE;
	case LEO_ERROR_MEDIUM_MAY_HAVE_CHANGED:
	case LEO_ERROR_DIAGNOSTIC_FAILURE:
	  goto DCHANGE;
	DCHANGE:
	  dd_openDialog(mes);
	  ret = dd_errSeqDiskChange();
	  ret = ( ret == DDSTAT_GOOD ) ? DDSTAT_CONTINUE : ret;
	  break;
	case LEO_ERROR_UNRECOVERED_READ_ERROR:
	  LeoReadCapacity( &cap , OS_WRITE );
	  ret = ( dd_stat.position < cap.startLBA ) ? 
		dd_defaultErrors() : dd_restoreData();
	  break;
    
	default:
	  ret = dd_defaultErrors();
  }

  return ret;
}

s32
dd_checkInsert( void ) /*  Set _save_id to true when disk is changed. */
{
  s32 err_code = -1;
  s32           ret;

  LeoReadDiskID(&_cmdBlk, &_diskID, &diskQ);
  LeoRecv(err_code,TLeoReadDiskID);

  switch( err_code )
	{
	case LEO_ERROR_GOOD:
	  dd_closeDialog(0);
	  if ( _save_id )
		{
		  dd_saveID( &_diskID );
		  _save_id = 0;
		}
	  ret = DDSTAT_INSERTED;
	  break;

	case LEO_ERROR_EJECTED_ILLEGALLY_RESUME:
	  ret = DDSTAT_CONTINUE | dd_openDialog( ERRMES__ACCESSLAMP );
	  goto NONEED_DISK;

	case LEO_ERROR_MEDIUM_NOT_PRESENT:
	  ret = DDSTAT_CONTINUE | dd_openDialog( ERRMES__DISK_WO_IRERO );
	  goto NONEED_DISK;	  

	NONEED_DISK:
#ifdef START_FROM_CASSETTE
#ifndef NEED_DISK
	  if( not_initialized )
		{
		  dd_closeDialog(0);
		  ret = DDSTAT_GOOD;
		}
#endif
#endif
	  break;

	case LEO_ERROR_DIAGNOSTIC_FAILURE:
	  ret =  DDSTAT_CONTINUE | dd_openDialog( ERRMES__INSERT_AGAIN );
	  break;

#ifdef START_FROM_CASSETTE
	case LEO_ERROR_POWERONRESET_DEVICERESET_OCCURED:
	  if( not_initialized )
		{
		  dd_stat.counter ++;
		  if ( dd_stat.counter < 30 ) 
			{
			  LeoResetClear();
			  ret = DDSTAT_CONTINUE;
			  break;
			}
		}
	  ret = dd_defaultErrors();
      break;
#endif
	default:
	  ret = dd_defaultErrors();
    }
  
  return ret;
}

s32
dd_errSeqDiskChange(void)
{
  s32    ret = -1;

  while(1)
	{
	  while ( ( ret = dd_checkInsert() ) & DDSTAT_CONTINUE ) dd_wait();
	  if( ret != DDSTAT_INSERTED ) break;

	  if( dd_confirmDisk() ) 
		{
		  ret = DDSTAT_GOOD;
		  break;
		}
	  dd_openDialog( ERRMES__WRONG_DISK );

	  while ( ( ret = dd_checkEject() ) & DDSTAT_CONTINUE ) dd_wait();
	  if( ret != DDSTAT_GOOD ) break;
	}
  return ret;
}

s32
dd_openDialog(s32 mes)
{
  dd_stat.mode |= DDSTAT_DIALOGUE;

  if( mes & 0xFF00 ) 
	dd_stat.errmes = ( mes & 0xFF00 ) + ( dd_stat.errmes & 0x00FF ) ;
  else
	dd_stat.errmes = ( mes & 0x00FF ) + ( dd_stat.errmes & 0xFF00 ) ;
  return mes ? DDSTAT_DIALOGUE : 0;
}

void
dd_closeDialog(s32 mask)  /*  Area 1: 0xff, Area 2: 0x00ff */
{
  dd_stat.errmes &= mask;

  if(!dd_stat.errmes)
	dd_stat.mode &= ~DDSTAT_DIALOGUE; 
}

s32 
dd_confirmDisk( void )
{
  return !bcmp((void *)&_savedID, (void *)&_diskID, sizeof(LEODiskID));
}

static s32 _already_frozen=0;

/*  Suspension of DD thread.  Resumes according to dd_thaw */
s32
dd_freeze(s32 errmes)
{
  if( _already_frozen ) return 0;

  _already_frozen = 1;
  dd_stat.mode = DDSTAT_KILLED;
  dd_openDialog(errmes);
  osRecvMesg(&frozenQ,(OSMesg *)NULL, OS_MESG_BLOCK);  

  dd_stat.mode = DDSTAT_BUSY;  
  dd_closeDialog(0);
  
  return -1;
}
