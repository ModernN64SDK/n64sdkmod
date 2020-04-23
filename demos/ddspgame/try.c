
/*---------------------------------------------------------------------*
        Copyright (C) 1998 Nintendo.
        
        $RCSfile: try.c,v $
        $Revision: 1.2 $
        $Date: 1998/12/02 00:44:40 $
 *---------------------------------------------------------------------*/

/*
   try.c   - Exception handling class
 */

/* pre-process */
#include "main.h"
#include "htry.h"
#include "hdd.h"

/* implmentations */

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

/* Explanation */
