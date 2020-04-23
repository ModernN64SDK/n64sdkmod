/*---------------------------------------------------------------------*
        Copyright (C) 1998 Nintendo.
        
        $RCSfile: dd_core.c,v $
        $Revision: 1.4 $
        $Date: 1998/12/26 00:40:40 $
 *---------------------------------------------------------------------*/

/*
 * DD thread generation
 */ 

/* implementation */
extern u8 heap_rwbuf[];

s32 
dd_create(void) /* constructor */
{
  static OSThread 		diskThread;
  static u64      		diskStack[STACKSIZE/sizeof(u64)];
  TMessage4dd           *msg = &dd_firstMsg;

  if( !not_initialized ) return 0;

  osCreateMesgQueue(&diskMsgQ, diskMsgBuf, DISK_MSG_NUM);
  osCreateMesgQueue(&diskQ, diskQBuf, 1);				   
  osCreateMesgQueue(&ddflushQ, ddflushQBuf, 1);				   
  osCreateMesgQueue(&frozenQ, frozenQBuf, 1);				   
  osCreateMesgQueue(&ddfirstQ, ddfirstQBuf, 1);				   
  osCreateMesgQueue(&recDiskMsgQ, recDiskMsgBuf, DISK_MSG_NUM);

/* default heap */
  bzero( heap_rwbuf, READWRITE_BYTESIZE );

  HANDLE_DD = osDriveRomInit();

  osCreateThread(&diskThread, 8, (void *)coreObjectOfdd, 0
				 ,diskStack+STACKSIZE/sizeof(u64), DD_THREAD_PRIORITY);

  bzero( &dd_stat , sizeof(TDDStatus) );

  osStartThread(&diskThread);
  dd_stat.mode = DDSTAT_BUSY;
  msg->retQ    = &ddfirstQ;
  msg->command = LEOCMD_INITIALIZE;
  dd_exec(msg);

  return -1;
}

/*
 *  Thread for the DD process.
 * 
 *  Commands are issued by dd_exec. 
 *  dd_exec must confirm that the return value of dd_checkGate is true before issuing    *   a command.  If it is not true, there is no guarantee that dd_exec will issue the 
*   command.
 *  However, when recQ for TMessage4dd is Null, dd_exec becomes
 *  an asynchronous function, and dd_checkGate is unnecessary. 
*/


static void
coreObjectOfdd( void * arg )
{
  s32	next_mode,_cmd,_retval;
  u8    unit_status=0;
  TMessage4dd *msg=NULL;

  arg = arg; /* no use */

  while (1){

      /* modified */
    osRecvMesg( &diskMsgQ ,(OSMesg *)&msg , OS_MESG_BLOCK);

	_cmd = msg->command;
    next_mode = DDSTAT_BUSY;

	if( dd_stat.mode & DDSTAT_KILLED ) /* KILLED status restored condition*/
	  {
		switch( dd_stat.errno )
		  {
#ifdef START_FROM_CASSETTE
		  case LEO_ERROR_REAL_TIME_CLOCK_FAILURE:
			if( not_initialized && ( _cmd == LEOCMD_SETRTC ) )
			  {
			    dd_closeDialog(0);
				do{
				  next_mode = dd_setRTC((LEODiskTime *)&msg->param);
				}while(	next_mode == DDSTAT_CONTINUE );

				if( next_mode == DDSTAT_GOOD )
				  {
					dd_stat.mode = DDSTAT_CONTINUE;
					_cmd = LEOCMD_INITIALIZE;
					break;
				  }
				else
				  dd_stat.mode = DDSTAT_DEAD;
			  }
#endif
		  default:
			osSendMesg(msg->retQ, (OSMesg)0 , OS_MESG_BLOCK);			
			continue;
		  }
	  }
	
	_retval = -1; /* default */
	dd_stat.errmes = 0;

	do{
	  dd_stat.mode = next_mode;
	  switch ( _cmd )
		{
		case LEOCMD_INITIALIZE:
		  next_mode = dd_init();
		  if( next_mode == DDSTAT_GOOD )  not_initialized = 0;
		  break;
		case LEOCMD_IDLE:
		  next_mode = DDSTAT_IDLE;
		  break;
		case LEOCMD_REZERO:
		  next_mode = dd_rezero();
		  break;
		case LEOCMD_SEEK:
		  next_mode = dd_seek( msg->param.hi , UNITMODE_LBA );
		  if( next_mode == DDSTAT_GOOD )
			dd_stat.position = msg->param.hi;
		  break;
		case LEOCMD_LBAREAD:
		  _setBuffer((u8 *)msg->param.rw);  
		  next_mode = dd_read(msg->param.hi , msg->param.lo , UNITMODE_LBA);
		  if( next_mode == DDSTAT_GOOD )
			{
			  msg->param.rw = (u32)_rwbuf;
			  _retval = dd_stat.buffer_size;
			}
		  break;
	    case LEOCMD_BYTEREAD:
          _setBuffer((u8 *)msg->param.rw);  
          next_mode = dd_read(msg->param.hi , msg->param.lo , UNITMODE_BYTE);
          if( next_mode == DDSTAT_GOOD )
            {
              msg->param.rw = (u32)_rwbuf;
			  msg->param.offs     = dd_stat.read_offset;
              _retval = dd_stat.buffer_size;		/* >= param.lo */
            }
          break;
		case LEOCMD_LBAWRITE:
		  _setBuffer((u8 *)msg->param.rw);  
		  next_mode = dd_lbaWrite(msg->param.hi ,msg->param.lo );
		  if( next_mode == DDSTAT_GOOD )
			{
              msg->param.rw = (u32)_rwbuf;
			  _retval = dd_stat.buffer_size;
			  dd_stat.position = msg->param.hi + msg->param.lo;
			}
		  break;
		case LEOCMD_ACTIVE:
		  next_mode = dd_activate( LEO_MOTOR_ACTIVE );
		  break;
		case LEOCMD_STANDBY:
		  next_mode = dd_activate( LEO_MOTOR_STANDBY);
		  break;
		case LEOCMD_SLEEP:
		  next_mode = dd_calmDown( LEO_MOTOR_SLEEP  );
		  break;
		case LEOCMD_BRAKE:
		  next_mode = dd_calmDown( LEO_MOTOR_BRAKE  );
		  break;
		case LEOCMD_GETID:
		  next_mode = msg->param.lo ? dd_checkInsert() : dd_errSeqDiskChange();
		  if( next_mode == DDSTAT_GOOD )
			{
			  if( (msg->param.hi) & 0x80000000 )
				bcopy( &_diskID , (void *)msg->param.hi ,sizeof(LEODiskID) );
			}
		  break;	
		case LEOCMD_EJECT:
		  next_mode = dd_checkEject();
		  break;	
		case LEOCMD_SETMOTORTIME:
		  next_mode = dd_setMotorTime(msg->param.hi,msg->param.lo);
		  break;
		case LEOCMD_GETRTC:
		  next_mode = dd_readRTC();
		  if( next_mode == DDSTAT_GOOD ) bcopy(&_rtc,&msg->param,sizeof(LEODiskTime));
		  break;
		case LEOCMD_SETRTC:
		  next_mode = dd_setRTC((LEODiskTime *)&msg->param);
		  break;
		  
		case LEOCMD_TESTUNIT:
		  next_mode = dd_testUnit( (LEOStatus *)&unit_status );
		  _retval = 0x100 | unit_status;
		  break;
		  
		default:
		  dd_pushError(LEO_ERROR_USER_DEFINED);
		  next_mode = dd_defaultErrors();
		}
    }while( next_mode & DDSTAT_CONTINUE );
	
	dd_stat.mode = next_mode; 
	if ( next_mode == DDSTAT_GOOD )
	  dd_closeDialog(0);
	else
	  _retval = 0;
 
	osSendMesg(msg->retQ, (OSMesg)_retval , OS_MESG_BLOCK); /* Notify that processing has terminated */
  }
}

/*  Function for issuing commands to the DD thread */
/*  It is an asynchronous function if retQ has been specified in the */
/*  message structure parameter and synchronous if it is 0. */

s32
dd_exec( TMessage4dd * msg )
{

  if(!(dd_stat.mode & DDSTAT_DEAD ) && msg->locked == FALSE )
	{
	  msg->locked = TRUE;
	  if(!msg->retQ)
		{
		  msg->retQ = &recDiskMsgQ;  /*  Temporary message queue */
		  osSendMesg(&diskMsgQ, (OSMesg) msg , OS_MESG_BLOCK);
		  osRecvMesg( msg->retQ , (OSMesg *)&(msg->retVal), OS_MESG_BLOCK);
		  msg->locked = false;
		  msg->retQ = NULL;
		}
	  else
		osSendMesg(&diskMsgQ, (OSMesg) msg , OS_MESG_BLOCK);
	}
  else
	{
	  msg->retVal = 0;
	  return false;
	}
  return true;
}

s32 
dd_checkGate( TMessage4dd *ddmsg ) /*  Filter for issuing commands to DD asynchronously */
{
  if( dd_stat.mode & ( DDSTAT_DEAD | DDSTAT_BUSY | DDSTAT_CONTINUE ) )
	return 0;

  if ( ddmsg->locked == true && ddmsg->retQ )
	{
	  if(osRecvMesg( ddmsg->retQ,(OSMesg *)&(ddmsg->retVal) ,OS_MESG_NOBLOCK))
		return 0;
	  ddmsg->locked = false; 
	}
  return -1;
}

void           
dd_change_mode_USA(void)
{
  _CA = 1;
}

u32
dd_getVersion(void)
{
  LEOVersion ver;
  u32 ret=0;
 
  if( !LeoInquiry(&ver) )
	{
	   ret  = (ver.drive<<24)    & 0xff000000;
	   ret |= (ver.driver<<16)   & 0x00ff0000;
	   ret |= (ver.deviceType<<8)& 0x0000ff00;
	   ret |= (ver.ndevices<<0)  & 0x000000ff;
	}
		
  return ret;
}

s32
dd_getBlockSize(s32 lba , s32 size)
{
  s32 ret;
  LeoLBAToByte( lba ,size ,&ret);
  return ret;
}

s32
dd_saveID(LEODiskID * det)
{
  bcopy( det, &_savedID ,sizeof(LEODiskID));
  return -1;
}

void
dd_wait(void) /*  Wait till the next retrace */
{
  while (!osRecvMesg(&ddflushQ, NULL, OS_MESG_NOBLOCK)); /* <- Semicolon IS required ! */
  osRecvMesg(&ddflushQ,(OSMesg *)NULL, OS_MESG_BLOCK);
}

void
dd_errHook(s32 func_no , s32 err_no , s32 count )
{
  _hook.target = func_no;
  _hook.errno  = err_no;
  _hook.cnt    = count;
}

s32
dd_thaw(s32 target_err) /*  Resume a suspended thread  */
{
  if( _already_frozen && ( dd_stat.errmes == target_err ) )
	{
	  _already_frozen = 0;
	  osSendMesg(&frozenQ,(OSMesg *)NULL, OS_MESG_BLOCK);
	  return -1;
	}
  return 0;
}

void
dd_flush(void)
{
/*  from core */
  if(HANDLE_DD) osSendMesg(&ddflushQ, (OSMesg)NULL, OS_MESG_NOBLOCK);
}

TDDStatus *
dd_getStatus(void)
{
  return &dd_stat;
}

void
_setBuffer( u8 *buf )
{
  u32 chopper = ~0xF;

  if( buf == NULL )
	{
	  _rwbuf = heap_rwbuf;
	  _rwbuf_size = READWRITE_BYTESIZE;
	}
  else
	{
	  _rwbuf = (u8 *)(((u32)buf + 7 ) & chopper);
	  _rwbuf_size = DD_MAX_BUFSIZE;
	}
}

s32
dd_driveExist( void )
{
  static s32 _flag = 0;
  return _flag ? _flag : LeoDriveExist();
}


