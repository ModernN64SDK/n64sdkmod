/*======================================================================*/
/*		NuSYS							*/
/*		nusicallbackremove.c					*/
/*									*/
/*		Copyright (C) 1997, NINTENDO Co,Ltd.			*/
/*									*/
/*----------------------------------------------------------------------*/
/* Ver 1.1	97/12/3		Created by Kensaku Ohki(SLANP)		*/
/*======================================================================*/
/* $Id: nusicallbackremove.c,v 1.4 1999/01/21 07:21:53 ohki Exp $	*/
/*======================================================================*/
#include <nusys.h>
/*----------------------------------------------------------------------*/
/* nuSiCallBackRemove - Delete call back function from SI manager   	*/
/* Deletes call back function allocated to call back function list from */
/* list.  If not allocated, it does nothing.  		    		*/
/*	IN:	*list    Pointer of call back function list structure	*/
/*	RTN:	none							*/
/*----------------------------------------------------------------------*/
void nuSiCallBackRemove(NUCallBackList* list)
{
    OSIntMask		mask;
    NUCallBackList**	siCallBackListPtr;


    /* Look for the function in the list*/
    siCallBackListPtr = &nuSiCallBackList;

    while(*siCallBackListPtr){

	if((*siCallBackListPtr)->next == list){
	    mask = osSetIntMask(OS_IM_NONE);
	    (*siCallBackListPtr)->next = list->next;
	    list->next = NULL;
	    osSetIntMask(mask);
	    break;
	}
	siCallBackListPtr = &(*siCallBackListPtr)->next;
    }
}

    