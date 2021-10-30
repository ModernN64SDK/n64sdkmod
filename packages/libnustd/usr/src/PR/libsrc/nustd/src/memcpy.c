/*
=============================================================================
        Copyright (C) 1997-1999 NINTENDO Co.,Ltd.
        
        $RCSfile: memcpy.c,v $
        $Revision: 1.2 $
        $Date: 1999/04/30 10:42:48 $
=============================================================================
Function name: _nmemcpy
-----------------------------------------------------------------------------
Format:  #include <string.h>
        void *_nmemcpy(void *dest, const void *src, size_t count)
Argument:   dest    pointer of destination to be copied 
        src    pointer of source to be copied
        count  the number of bytes to be copied 
Return value:   pointer of destination to be copied,dest 
Explanation:  copies the contents of scr to dest by the amount of count.  
        If the areas of the source and the destination are overlapping, it will not be copied correctly.
-----------------------------------------------------------------------------
*/
#include    "string.h"

void *_nmemcpy(void *dest, const void *src, size_t count)
{
    char    *d = (char *)dest, *s = (char *)src;

    while(count--)  *d++ = *s++;
    return  dest;
}

