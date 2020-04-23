
/*---------------------------------------------------------------------*
        Copyright (C) 1998 Nintendo.
        
        $RCSfile: globals.c,v $
        $Revision: 1.1 $
        $Date: 1999/07/05 02:17:00 $
 *---------------------------------------------------------------------*/

#include "hglobals.h"
#include "hdd.h"

OSId 			g_thread_id;

#ifdef _RDB_
u8  			rdbSendBuf[RDB_SEND_BUF_SIZE];
#endif

OSPiHandle *HANDLE_ROM;

OSMesgQueue     rspMsgQ, rdpMsgQ, dmaMsgQ;

OSMesgQueue 	piMsgQ;

/* implementations */

/*-----------------------------------------------------------------------------
  DMA Transfer
-----------------------------------------------------------------------------*/

s32
g_dmaRead(OSPiHandle *handle, void *src_addr, void *dest_addr, u32 size)
{
  OSIoMesg    iomsg;
  TDDToken    _token;

  if (size & 0x1) size--;

#ifdef DEBUG
  try_assert("dmaread : no handle",(s32)handle);
  try_assert("dmaread : size 0",(s32)size);
#endif

  iomsg.hdr.pri       = OS_MESG_PRI_NORMAL;
  iomsg.hdr.retQueue  = &dmaMsgQ;
  iomsg.dramAddr      = dest_addr;
  iomsg.devAddr       = (u32)src_addr;
  iomsg.size          = size;
 
  /* void the CPU cache */
  osInvalDCache((void*)dest_addr, (s32)size);

  /* start DMA read */
  if( _token = dd_getToken(DD_TOKENKEY_ROMREAD,1)) // block_mode
	{
	  osEPiStartDma(handle, &iomsg, OS_READ);
	  
	  /* wait DMA read end */
	  (void)osRecvMesg(&dmaMsgQ, NULL, OS_MESG_BLOCK);
	  
	  dd_freeToken(_token);
	  
	  return (s32)size;
	}         
  
  return 0;
}

void
g_dmaNode_insert( TDmaNode *_new, TDmaNode *_dest )
{
	  _new->node.next  = (ALLink *)_dest;
	  _new->node.prev  = (ALLink *)0;
	  _dest->node.prev = (ALLink *)_new;
}

