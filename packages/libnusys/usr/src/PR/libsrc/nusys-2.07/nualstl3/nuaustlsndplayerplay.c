/*======================================================================*/
/*		NuSYS							*/
/*		nuaustlsndplayerplay.c					*/
/*									*/
/*		Copyright (C) 1999, NINTENDO Co,Ltd.			*/
/*									*/
/*======================================================================*/
/* $Id: nuaustlsndplayerplay.c,v 1.1 1999/01/07 12:34:00 ohki Exp $	*/
/*======================================================================*/
#include <nusys.h>
#include <nualstl.h>

/*----------------------------------------------------------------------*/
/*	nuAuStlSndPlayerPlay - Reproduce sound				*/
/*	IN:	sndNo		number of sounds to reproduce		*/
/*	RET:	sound handler						*/
/*----------------------------------------------------------------------*/
musHandle nuAuStlSndPlayerPlay(u32 sndNo)
{
    return( MusStartEffect(sndNo) );
}

/*----------------------------------------------------------------------*/
/*	nuAuStlSndPlayerPlay2 - Reproduce sound(specify parameter)	*/
/*	IN:	sndNo		number of sounds to reproduce		*/
/*		volume	volume scale (0 - 0x100)			*/
/*		pan		pan range	 (0 - 0xFF)		*/
/*		restartflag	Flag to sound again overwritting or 	*/
/*				not when the same sound number is specified*/
/*				(0:  don't overwrite;  other than 0:  overwrite	*/
/*		priority	priority of sound			*/
/*	RET:	sound handler						*/
/*----------------------------------------------------------------------*/
musHandle nuAuStlSndPlayerPlay2(u32 sndNo, s32 volume, s32 pan, s32 restartflag, s32 priority)
{
    return( MusStartEffect2(sndNo, volume, pan , restartflag, priority) );
}
