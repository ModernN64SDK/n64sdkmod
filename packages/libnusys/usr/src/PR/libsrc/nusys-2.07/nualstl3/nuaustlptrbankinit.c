/*======================================================================*/
/*		NuSYS							*/
/*		nuaustlptrbankinit.c					*/
/*									*/
/*		Copyright (C) 1999, NINTENDO Co,Ltd.			*/
/*									*/
/*======================================================================*/
/* $Id: nuaustlptrbankinit.c,v 1.1 1999/01/07 12:29:49 ohki Exp $		*/
/*======================================================================*/
#include <nusys.h>
#include <nualstl.h>

void*	nuAuPtrBank_ptr = NULL;

/*----------------------------------------------------------------------*/
/*	nuAuStlPtrBankInit - Initializes the sample pointer bank		*/
/*									*/
/*	Allocates storage area for the sample pointer bank (.ptr).		*/
/*									*/
/*	IN:	pbk_size	size of the wave table reference pointer	*/
/*	RET:	nothing							*/
/*----------------------------------------------------------------------*/
void nuAuStlPtrBankInit(u32 pbk_size)
{
    nuAuPtrBank_ptr =  nuAuStlHeapAlloc(pbk_size);
}
