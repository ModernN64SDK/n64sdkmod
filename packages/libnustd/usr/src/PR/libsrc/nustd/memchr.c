/*
=============================================================================
        Copyright (C) 1997-1999 NINTENDO Co.,Ltd.
        
        $RCSfile: memchr.c,v $
        $Revision: 1.1 $
        $Date: 1999/04/14 07:18:40 $
=============================================================================
function name: memchr
-----------------------------------------------------------------------------
format:   #include <string.h>
        void *memchr(const void *buf, int c, size_t count);
argument:  buf     pointer to be found
        c     byte data to be found
        count    the number of characters to be found
return value: first pointer that found byte data c  
        if not found, NULL is returned. 
explanation:  finds byte data c of the number of count characters from a pointer  
-----------------------------------------------------------------------------
*/
#include    "string.h"

void *memchr(const void *buf, int c, size_t count)
{
    unsigned char   *p = (char *)buf;
    while(count--)  {
        if (*p == c)    return  p;
        p++;
    }
    return  (void *)NULL;
}
