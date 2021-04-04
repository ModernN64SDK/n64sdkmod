/*
=============================================================================
        Copyright (C) 1997-1999 NINTENDO Co.,Ltd.
        
        $RCSfile: memmove.c,v $
        $Revision: 1.1 $
        $Date: 1999/04/14 07:18:40 $
=============================================================================
function name: memmove
-----------------------------------------------------------------------------
format:   #include <string.h>
        void *memmove(void *dest, const void *src, size_t count);
argument:   dest    pointer of destination to be copied
        src    pointer of source to be copied
        count  the number of bytes to be copied
return value:   pointer of destination to be copied, dest
explanation: copies the contents of scr to dest by the amount of count.
        Even if the areas of source and destination are overlapping, it will be copied correctly.
-----------------------------------------------------------------------------
*/
#include    "string.h"

void *memmove(void *dest, const void *src, size_t count)
{
    char    *d = (char *)dest, *s = (char *)src;
    if (d == s) return  dest;
    if (d < s)  {
        while(count--)  *d++ = *s++;
    }   else    {
        d += count-1;
        s += count-1;
        while(count--)  *d-- = *s--;
    }
    return  dest;
}
