/*======================================================================*/
/*		NuSYS							*/
/*		nuauinit.c						*/
/*									*/
/*		Copyright (C) 1997, NINTENDO Co,Ltd.			*/
/*									*/
/*======================================================================*/
/* $Id: nuauinit.c,v 1.5 1998/12/24 08:29:58 ohki Exp $		*/
/*======================================================================*/

#include <nusys.h>
#include <nualsgi.h>

/*----------------------------------------------------------------------*/
/*	nuAuInit - Initializes the audio					*/
/*	IN:	start of buffer						*/
/*	RET:	size of used heap area used					*/
/*----------------------------------------------------------------------*/
s32 nuAuInit(void)
{
    /* Initialize the Audio Manager.	*/
    nuAuMgrInit((void*)NU_AU_HEAP_ADDR, NU_AU_HEAP_SIZE, &nuAuSynConfig);

    /* Initialize the Sequence Player.	*/
    nuAuSeqPlayerInit(&nuAuSeqpConfig, 0x8000, NU_AU_SEQ_PLAYER0);

    /* Initialize the Sequence Player.	*/
    nuAuSeqPlayerInit(&nuAuSeqpConfig, 0x8000, NU_AU_SEQ_PLAYER1);

    /* Initialize the Sound Player. */
    nuAuSndPlayerInit(&nuAuSndpConfig);

    /* Initialize the audio control callback function. */
    nuAuMgrFuncSet(nuAuSeqPlayerControl);

    /* Register the PRE NMI processing function.	*/
    nuAuPreNMIFuncSet(nuAuPreNMIProc);

    /* Return the size of the heap area used.	*/
    return nuAuHeapGetUsed();
}

