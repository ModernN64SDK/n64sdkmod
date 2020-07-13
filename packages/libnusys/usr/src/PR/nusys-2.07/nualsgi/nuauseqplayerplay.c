/*======================================================================*/
/*		NuSYS							*/
/*		nuauseqplayerplay.c					*/
/*									*/
/*		Copyright (C) 1997, NINTENDO Co,Ltd.			*/
/*									*/
/*======================================================================*/
/* $Id: nuauseqplayerplay.c,v 1.2 1998/12/24 09:10:30 ohki Exp $	*/
/*======================================================================*/

#include <nusys.h>
#include <nualsgi.h>

/*----------------------------------------------------------------------*/
/*	nuAuSeqPlayerPlay - Sequence reproduction of sequence player	*/
/*	IN:	player_no	  sequence player						*/
/*				NU_AU_SEQ_PLAYER0 or NU_AU_SEQ_PLAYER1		*/
/*	RET:	None										*/
/*----------------------------------------------------------------------*/
void nuAuSeqPlayerPlay(u32 player_no)
{
  if(nuAuSeqPlayerGetState(player_no) ==  AL_STOPPED)
    {
      alCSeqNew(&nuAuSeqPlayer[player_no].sequence,
		nuAuSeqPlayer[player_no].data_ptr);
      alCSPSetSeq(&nuAuSeqPlayer[player_no].player,
		  &nuAuSeqPlayer[player_no].sequence);
      alCSPPlay(&nuAuSeqPlayer[player_no].player);
    }
  else
    {
      nuAuSeqPlayerStop(player_no);
      /* 
	 Set NU_AU_SEQ_MODE_PLAYPENDING flag to reproduce after the player
	 stopped completely as the tempo could go wrong if reproduced during
	 release.  Reproduce stand by process is performed every retrace using 
	 auSeqPlayerControl(), and when AL_STOPPED, nuSeqPlayerPlay() will be 
	 called again.
	 */
      nuAuSeqPlayer[player_no].mode |= NU_AU_SEQ_MODE_PLAYPENDING;
    }
}
