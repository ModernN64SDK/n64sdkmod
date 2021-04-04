/*======================================================================*/
/*		NuSYS							*/
/*		nuaustlheap.c						*/
/*									*/
/*		Copyright (C) 1999, NINTENDO Co,Ltd.			*/
/*									*/
/*======================================================================*/
/* $Id: nuaustlheap.c,v 1.1 1999/01/07 12:29:23 ohki Exp $		*/
/*======================================================================*/
#include <nusys.h>
#include <nualstl.h>
#include "lib_memory.h"		/* Please copy from libmus source. */

/*----------------------------------------------------------------------*/
/*      nuAuStlHeapAlloc - Allocates memory                                	*/
/*                                                                      */
/*      Reserves the audio heap area.                                    */
/*                                                                      */
/*      IN:     length                                                  */
/*      RET:    pointer to the reserved heap                                */
/*----------------------------------------------------------------------*/
void* nuAuStlHeapAlloc(s32 length)
{
    return __MusIntMemMalloc(length);
}

/*----------------------------------------------------------------------*/
/*      nuAuStlHeapGetFree - Obtains the size of the available heap                  	*/
/*                                                                      */
/*      Obtains the size of the available audio heap.            */
/*                                                                      */
/*      IN:     nothing                                                    */
/*      RET:    size of heap available	                                */
/*----------------------------------------------------------------------*/
s32 nuAuStlHeapGetFree(void)
{
    ALHeap* heap;
    heap = __MusIntMemGetHeapAddr();

    return(heap->len - nuAuStlHeapGetUsed());
}

/*----------------------------------------------------------------------*/
/*      nuAuStlHeapGetUsed - Obtains the size of heap used			*/
/*                                                                      */
/*      Obtains the size of the audio heap area used..                  */
/*                                                                      */
/*      IN:     length                                                  */
/*      RET:    size of the audio heap area used					*/
/*----------------------------------------------------------------------*/
s32 nuAuStlHeapGetUsed(void)
{
    return(__MusIntMemRemaining());
}

