/*---------------------------------------------------------------------*
        Copyright (C) 1998 Nintendo.
        
        $RCSfile: dd_checksum.c,v $
        $Revision: 1.1 $
        $Date: 1999/07/05 02:17:00 $
 *---------------------------------------------------------------------*/

/*  Read/Write check functions */
/*  Please modify the following functions to suit the application . */

#define CHECK_VAGUE 256

s32
dd_checkSum(void)
{
  /*  Checks the final 4 bytes of the designated area. */
  s32 i;
  u32 sum=0,*cs;

  u8  *buf = _rwbuf;

  for(i = 0; i < (s32)dd_stat.buffer_size - CHECK_VAGUE - 4; i += CHECK_VAGUE , buf += CHECK_VAGUE )
    {
        sum += *buf;
    }

  buf = _rwbuf;
  cs = (u32 *)((u32)buf + dd_stat.buffer_size - 4);
  
  return (*cs == sum) ?  LEO_ERROR_GOOD : LEO_ERROR_CHECKSUM;
}

s32
dd_makeSum(void)
{
  /*  Overwrites the final 4 bytes of the designated area with the checksum. */
  /*  Thus the actual size (specified size - 4) of the transferred data is in bytes. */

  s32 i;
  u8  *buf;
  u32 sum=0,*cs;

  buf = _rwbuf;

   for(i = 0; i < (s32)dd_stat.buffer_size - CHECK_VAGUE - 4 ; i += CHECK_VAGUE , buf += CHECK_VAGUE )
    {
        sum += *buf;
    }
 
  buf = _rwbuf;
  cs = (u32 *)((u32)buf + dd_stat.buffer_size - 4);
  *cs = sum;

  return -1;
}

s32
dd_restoreData( void )
{
  /*
   * A routine to restore data at damaged LBAs.
   * Note that lost data content essentially cannot be returned to its original state.
   * This function simply rewrites the contents of the current buffer.
   */

  s32 lba_size,ret;
  TDDStatus _keep;

  bcopy( &dd_stat,&_keep,sizeof(TDDStatus));
  dd_freeze( ERRMES__NOW_IN_RESTORE ); /*  Keeps the external response. */

  LeoByteToLBA(  dd_stat.position , (u32)dd_stat.buffer_size , &lba_size);
  while( ( ret = dd_lbaWrite( dd_stat.position ,lba_size ) ) & DDSTAT_CONTINUE ) 
	dd_wait();
  bcopy( &_keep,&dd_stat,sizeof(TDDStatus));

  dd_closeDialog(0);
  
  return ret ? DDSTAT_DEAD : DDSTAT_GOOD;
}
