/*======================================================================*/
/*		NuSYS							*/
/*		nuausndplayerinit.c					*/
/*									*/
/*		Copyright (C) 1997, NINTENDO Co,Ltd.			*/
/*									*/
/*======================================================================*/
/* $Id: nuausndplayerinit.c,v 1.1 1998/12/24 10:10:48 ohki Exp $	*/
/*======================================================================*/


#include <nusys.h>
#include <nualsgi.h>

/*----------------------------------------------------------------------*/
/*	nuAuSndPlayerInit - Initialize sound player				*/
/*	IN:	sndconfig     sound player set structure				*/
/*	RET:	None										*/
/*----------------------------------------------------------------------*/
void nuAuSndPlayerInit(ALSndpConfig* sndconfig)
{
  sndconfig->heap = &nuAuHeap;
  alSndpNew(&nuAuSndPlayer, sndconfig);
}

