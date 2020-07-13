/*======================================================================*/
/*		NuSYS							*/
/*		nuausndplayerplay.c					*/
/*									*/
/*		Copyright (C) 1997, NINTENDO Co,Ltd.			*/
/*									*/
/* 	$Id$								*/
/*======================================================================*/
/* $Id: nuausndplayerplay.c,v 1.1 1998/12/24 10:08:53 ohki Exp $*/
/*======================================================================*/

#include <nusys.h>
#include <nualsgi.h>

/*----------------------------------------------------------------------*/
/*	nuAuSndPlayerPlay - Reproduce sound						*/
/*	IN:	sndNo 	  number of sound to reproduce			*/
/*	RET:	ID for sound object							*/
/*		-1 = could not be reproduced						*/
/*----------------------------------------------------------------------*/
ALSndId nuAuSndPlayerPlay(u32 sndNo)
{

    ALBank*	bank_ptr;
    ALInstrument* inst;
    u32		cnt;

    bank_ptr  = nuAuSndBank_ptr->bankArray[0];
    inst      = bank_ptr->instArray[0];
    
#ifdef NU_DEBUG
    /* Check the number of sound */
    if(inst->soundCount <= sndNo){
	osSyncPrintf("nuAuSndPlayerPlay: sndNo %d is too big.\n", sndNo);
	return -1;
    }
#endif /* NU_DEBUG */
    /* Find nuAuSndId table and look for unused items */
    for(cnt = 0; cnt < nuAuSndPlayer.maxSounds; cnt++){	
      if(nuAuSndId[cnt] == -1)
	break;
      else
	{
	  /* If sound isn't used, delete it and use the number */
	  alSndpSetSound(&nuAuSndPlayer,nuAuSndId[cnt]);
	  if(alSndpGetState(&nuAuSndPlayer) == AL_STOPPED)
	    {
	      alSndpDeallocate(&nuAuSndPlayer,nuAuSndId[cnt]);
	      nuAuSndId[cnt] = -1; /* This line not actually necessary */
	      break;
	    }
	}
    }

    /* If there is no space in the table, error is returned */
    if(cnt == nuAuSndPlayer.maxSounds)
      return(-1);

    /* Register sound to reproduce */
    nuAuSndId[cnt] = alSndpAllocate(&nuAuSndPlayer,inst->soundArray[sndNo]);

    /* Only when it's allocated, play it back
       (allocation should be possible, but confirm it to make sure)
       If not allocated, error is returned */

    if(nuAuSndId[cnt] != -1)
      {
	alSndpSetSound(&nuAuSndPlayer,nuAuSndId[cnt]);
	alSndpPlay(&nuAuSndPlayer);	
      }
    return nuAuSndId[cnt];
}


