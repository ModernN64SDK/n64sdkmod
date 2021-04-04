/*======================================================================*/
/*		NuSystem						*/
/*		nuaumgr.c						*/
/*									*/
/*		Copyright (C) 1998, NINTENDO Co,Ltd.			*/
/*	98/12/21	Created  by K.Ohki(SLANP)			*/
/*======================================================================*/
/* $Id: nuauheap.c,v 1.3 1999/01/05 12:27:08 ohki Exp $		*/
/*======================================================================*/
#include <nusys.h>
#include <nualsgi.h>

ALHeap		nuAuHeap;			/* Heap structure */

/*----------------------------------------------------------------------*/
/*      nuAuHeapInit - Memory initialization                                 	*/
/*                                                                      */
/*      IN:     *heap	Heap structure					*/
/*		*baffer	Heap buffer location				*/
/*		length	Heap size					*/
/*      RET:    Pointer to the allocated heap 				*/
/*----------------------------------------------------------------------*/
void nuAuHeapInit(ALHeap* hp, u8* base, s32 len)
{
    alHeapInit(hp, base, len);
}

/*----------------------------------------------------------------------*/
/*      nuAuHeapAlloc - Allocates memory	*/
/*                                                                      */
/*      Allocates an area for the audio heap.                                    */
/*                                                                      */
/*      IN:     length                                                  */
/*      RET:   pointer to the allocated heap                                */
/*----------------------------------------------------------------------*/
void* nuAuHeapAlloc(s32 length)
{
    return alHeapAlloc(&nuAuHeap, 1, length);
}

/*----------------------------------------------------------------------*/
/*      nuAuHeapGetFree - Obtains the size of the available heap 	*/
/*                                                                      */
/*      Obtains the size of the available area of the heap. */
/*                                                                      */
/*      IN:     nothing                                                    */
/*      RET:   size of available heap area	                                */
/*----------------------------------------------------------------------*/
s32 nuAuHeapGetFree(void)
{
    return (nuAuHeap.len - nuAuHeapGetUsed());
}

/*----------------------------------------------------------------------*/
/*      nuAuHeapGetUsed - Obtains the size of the used heap			*/
/*                                                                      */
/*      Obains the size of the used audio heap area. */
/*                                                                      */
/*      IN:     length                                                  */
/*      RET:    size of the used heap area					*/
/*----------------------------------------------------------------------*/
s32 nuAuHeapGetUsed(void)
{
    return ((s32)nuAuHeap.cur - (s32)nuAuHeap.base);
}

