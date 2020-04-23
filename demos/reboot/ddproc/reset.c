
/*---------------------------------------------------------------------*
        Copyright (C) 1998 Nintendo.
        
        $RCSfile: reset.c,v $
        $Revision: 1.1 $
        $Date: 1999/07/05 02:17:01 $
 *---------------------------------------------------------------------*/

/* pre-process */

#include "hglobals.h"
#include "hreset.h"
#include "hdd.h"


/* declarerations */

OSMesgQueue     rstMessageQ;
OSMesg          rstMessageBuf;

static OSThread rstThread;
static u64      rstThreadStack[STACKSIZE/sizeof(u64)];
static void     coreObjectOfrst(void *);
s16             resetflag = 0;

/* implmentations */

void
rst_create(void)
{
  osCreateMesgQueue(&rstMessageQ, &rstMessageBuf, 1);
  osSetEventMesg(OS_EVENT_PRENMI, &rstMessageQ, NULL);

  osCreateThread(&rstThread, ++g_thread_id,coreObjectOfrst, 0
                                 ,rstThreadStack + STACKSIZE/sizeof(u64), RESET_THREAD_PRIORITY);
  osStartThread(&rstThread);
}

static void
coreObjectOfrst(void * arg)
{
  arg = arg; /* no use */

	osRecvMesg(&rstMessageQ, NULL, OS_MESG_BLOCK);
	resetflag = 1;
	dd_reset();
}


