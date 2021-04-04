/*======================================================================*/
/*		NuSystem						*/
/*		nuauconfig.c						*/
/*									*/
/*		Copyright (C) 1998, NINTENDO Co,Ltd.			*/
/*									*/
/*	98/12/10	Created	by K.Ohki(SLANP)			*/
/*======================================================================*/
/* $Id: nuauconfig.c,v 1.1 1998/12/24 08:28:22 ohki Exp $									*/
/*======================================================================*/

#include <nusys.h>
#include <nualsgi.h>

ALSynConfig	nuAuSynConfig = {
    NU_AU_SYN_VVOICE_MAX,	/* Number of virtual voices (not used)	*/
    NU_AU_SYN_PVOICE_MAX,	/* Number of physical voices			*/
    NU_AU_SYN_UPDATE_MAX,	/* Maximum number of updates			*/
    0,
    NULL,			/*DMA initialization callback function 	*/
    NULL,			/* Heap structure			*/
    NU_AU_OUTPUT_RATE,	/* Output frequency reset by the program */
    AL_FX_NONE,		/* Sound effects 		*/
    0,				/* Custom effects		*/
};

ALSeqpConfig	nuAuSeqpConfig = {
	NU_AU_SEQ_VOICE_MAX,
	NU_AU_SEQ_EVENT_MAX,
	NU_AU_SEQ_CHANNEL_MAX,
	0,
	NULL,
	NULL,
	NULL,
	NULL
};
ALSndpConfig	nuAuSndpConfig = {
    NU_AU_SND_SOUND_MAX,
    NU_AU_SND_EVENT_MAX,
    NULL
};
