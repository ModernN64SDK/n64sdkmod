
/*---------------------------------------------------------------------*
        Copyright (C) 1998 Nintendo.
        
        $RCSfile: try.c,v $
        $Revision: 1.1 $
        $Date: 1999/07/05 02:17:01 $
 *---------------------------------------------------------------------*/

/*
   try.c   - exception process class
 */

/* pre-process */
#include "hglobals.h"
#include "htry.h"
#include "hdd.h"

/* declarerations */

OSMesgQueue     tryMsgQ;
OSMesg          tryMsgBuf[TRY_MSGQ_SIZE];

static OSThread tryThread;
static u64      tryThreadStack[STACKSIZE/sizeof(u64)];
static void     objectOftry(void *);

/* implmentations */

void
try_create(void)
{
  osCreateMesgQueue(&tryMsgQ, tryMsgBuf,TRY_MSGQ_SIZE);

  osCreateThread(&tryThread, ++g_thread_id,(void *)objectOftry, 0
                                 ,tryThreadStack + STACKSIZE/sizeof(u64), TRY_THREAD_PRIORITY);
  osStartThread(&tryThread);
}

static void
objectOftry(void * arg)
{
  TException *e=NULL;
  arg=arg; /* no use */

  while(1)
	{
	  osRecvMesg(&tryMsgQ, (OSMesg *)e, OS_MESG_BLOCK);

	  switch(e->id)
		{

		}
	} 
}

void
try_assert(u8 * str , s32 i)
{
#ifdef DEBUG
  if(!i) 
	{
	  osSyncPrintf("catch exception %s \n",str );
	  osStopThread(0);
	  while(1);
	}
#else
  str = str;
  i = i;
#endif
  return;
}

s32
try_warning(u8 * str , s32 i)
{
#ifdef DEBUG
  if(!i) 
	{
	  osSyncPrintf("warning ! catch exception %s \n",str );
	}
#else
  str = str;
#endif
  return !i;
}

/* explanation */

