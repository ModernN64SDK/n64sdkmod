/*======================================================================*/
/*		NuSystem						*/
/*		nuauinitex.c						*/
/*									*/
/*		Copyright (C) 1998, NINTENDO Co,Ltd.			*/
/*									*/
/*	98/12/10	Created	by K.Ohki(SLANP)			*/
/*======================================================================*/
/* $Id: nuauinitex.c,v 1.1 1998/12/24 08:30:33 ohki Exp $									*/
/*======================================================================*/
#include <nusys.h>
#include <nualsgi.h>

/*----------------------------------------------------------------------*/
/*	nuAuInitEx - Initializes the audio				*/
/*									*/
/*	Initializes the audio. The Audio Manager is initialized so that it		*/
/*	uses AI_FIFO_EVET. 		*/
/*	IN:	 start of buffer						*/
/*	RET:	size of heap area used					*/
/*----------------------------------------------------------------------*/
s32 nuAuInitEx(void)
{
    /* Initialize the Audio Manager.	*/
    nuAuMgrInitEx((void*)NU_AU_HEAP_ADDR, NU_AU_HEAP_SIZE, &nuAuSynConfig);

    /* Initialize the Sequence Player. */
    nuAuSeqPlayerInit(&nuAuSeqpConfig, 0x8000, NU_AU_SEQ_PLAYER0);

    /* Initialize the Sequence Player.	*/
    nuAuSeqPlayerInit(&nuAuSeqpConfig, 0x8000, NU_AU_SEQ_PLAYER1);

    /* Initialize the Sound Player. */
    nuAuSndPlayerInit(&nuAuSndpConfig);

    /* Initialize the audio control callback function.	*/
    nuAuMgrFuncSet(nuAuSeqPlayerControl);

    /* Register the PRE NMI processing function.	*/
    nuAuPreNMIFuncSet(nuAuPreNMIProc);

    /* Return the size of the heap area used. */
    return nuAuHeapGetUsed();
}

