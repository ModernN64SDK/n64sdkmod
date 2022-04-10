/*======================================================================*/
/*		NuSystem						*/
/*		nuauseqplayercontrol.c					*/
/*									*/
/*		Copyright (C) 1998, NINTENDO Co,Ltd.			*
/*									*/
/*	98/11/26	Created  by K.Ohki(SLANP)			*/
/*======================================================================*/
/* $Id: nuauseqplayercontrol.c,v 1.2 1998/12/24 09:05:13 ohki Exp $*/
/*======================================================================*/

#include <nusys.h>
#include <nualsgi.h>

/*----------------------------------------------------------------------*/
/*	nuAuSeqPlayerControl - Controls sequences			*/
/*									*/
/*	Controls the sequences.						*/
/*	This function is registered as a callback function by nuAuInit() and is called from 
/*	the Audio Manager at fixed intervals. 					*/
/*	The function performs sequence fadeouts, stops, and pauses.  Please use it as a	*/
/*	reference when creating custom callbacks. 				*/
/*	IN:	nothing							*/
/*	RET:	nothing							*/
/*----------------------------------------------------------------------*/
void nuAuSeqPlayerControl(void)
{
  int	cnt;
  int vol;

  for(cnt = 0; cnt <2 ;cnt++)
    {
      if(nuAuSeqPlayer[cnt].mode & NU_AU_SEQ_MODE_PLAYPENDING)
	{
	  if(nuAuSeqPlayerGetState(cnt) ==  AL_STOPPED)
	    {
	      /* Play pending sequences. */
	      nuAuSeqPlayerPlay(cnt);
	      nuAuSeqPlayer[cnt].mode &= ~NU_AU_SEQ_MODE_PLAYPENDING;
	    }
	}

      if(nuAuSeqPlayer[cnt].mode & NU_AU_SEQ_MODE_FADEOUT)
	{
	  /* Perform a sequence fadeout. */
	  if(nuAuSeqPlayer[cnt].counter == 0){
	    /*  If the counter value is 0, drop the flag and set the volume to 0. */
	    nuAuSeqPlayerSetVol(cnt, 0);
	    nuAuSeqPlayerStop(cnt);
	    nuAuSeqPlayer[cnt].mode &=~NU_AU_SEQ_MODE_FADEOUT;
	    nuAuSeqPlayerSetVol(cnt, 0x7fff);
	  } else {
	    vol = nuAuSeqPlayerGetVol(cnt);
	    vol -= nuAuSeqPlayer[cnt].speed;
	    nuAuSeqPlayerSetVol(cnt, vol);
	    nuAuSeqPlayer[cnt].counter--;
	  }
	}
    }	    
}
