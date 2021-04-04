/*======================================================================*/
/*		NuSYS							*/
/*		nuaustlsndplayerinit.c					*/
/*									*/
/*		Copyright (C) 1998, NINTENDO Co,Ltd.			*/
/*									*/
/*----------------------------------------------------------------------*/
/* Ver 1.3	98/10/19	Created by Kensaku Ohki(SLANP)		*/
/*======================================================================*/
/* $Id: nuaustlsndplayerinit.c,v 1.1 1999/01/07 12:33:37 ohki Exp $		*/
/*======================================================================*/
#include <nusys.h>
#include <nualstl.h>

void*	nuAuEffect_ptr = NULL;

/*----------------------------------------------------------------------*/
/*	nuAuStlSndPlayerInit - Initializes the Sound Player 		*/
/*									*/
/*	IN:	size		Size of the buffer for sound effect bank (.bfx) storage*/
/*								*/
/*	RET:	nothing							*/
/*----------------------------------------------------------------------*/
void nuAuStlSndPlayerInit(u32 size)
{
    nuAuEffect_ptr = nuAuStlHeapAlloc(size);
}
