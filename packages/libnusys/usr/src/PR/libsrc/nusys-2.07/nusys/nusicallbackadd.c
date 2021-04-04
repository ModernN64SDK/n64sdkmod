/*======================================================================*/
/*		NuSYS							*/
/*		nusicallbackadd.c					*/
/*									*/
/*		Copyright (C) 1997, NINTENDO Co,Ltd.			*/
/*									*/
/*----------------------------------------------------------------------*/
/* Ver 1.1	97/12/1		Created by Kensaku Ohki(SLANP)		*/
/*======================================================================*/
/* $Id: nusicallbackadd.c,v 1.7 1999/01/23 05:50:36 ohki Exp $	*/
/*======================================================================*/
#include <nusys.h>

/*----------------------------------------------------------------------*/
/* nuSiCallBackAdd - Registers a callback function with the SI Manager	*/
/*									*/
/*	Registers an SI device manager with the SI Manager.	*/
/*	The SI device manager itself is a list of pointers to functions.	 */
/*								*/
/*	IN:	*list	Pointer to the callback function list structure  */
/*	RTN:	nothing							*/
/*----------------------------------------------------------------------*/
void nuSiCallBackAdd(NUCallBackList* list)
{
    OSIntMask		mask;
    NUCallBackList**	siCallBackListPtr;
    s32			cnt;

    /* Search for end of list */
    siCallBackListPtr = &nuSiCallBackList;
    while(*siCallBackListPtr){

	/* Do not register a function if it is already registered. */
	if((*siCallBackListPtr)->majorNo == list->majorNo){
#ifdef NU_DEBUG
	    osSyncPrintf("nuSiCallBackAdd: CallBack is already added(major no = %#X!!\n", list->majorNo);
#endif /* NU_DEBUG */
	    return;
	}
	siCallBackListPtr = &(*siCallBackListPtr)->next;
    }

    /* Check the number of message processing functions.	*/
    /* func[0] is for retrace processing. When not need, it is NULL  */
    /*	 and not used, and the subsequent terminal NULL is searched for.*/
    
    for(cnt = 1; list->func[cnt] != NULL; cnt++);
    
    mask = osSetIntMask(OS_IM_NONE);
    *siCallBackListPtr = list;
    list->next = NULL;
    list->funcNum = cnt;
    osSetIntMask(mask);
}
