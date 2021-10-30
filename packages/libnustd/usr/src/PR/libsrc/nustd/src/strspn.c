/*
=============================================================================
        Copyright (C) 1997-1999 NINTENDO Co.,Ltd.
        
        $RCSfile: strspn.c,v $
        $Revision: 1.1 $
        $Date: 1999/04/14 07:18:42 $
=============================================================================
function name: strspn
-----------------------------------------------------------------------------
format:   #include <string.h>
        size_t  strspn(const char *string1, const char *string2);
argument:   string1     character string to be found
        	string2     character string to find
return value:  length of the head
explanation:   calculates the length of the head in character string, string1
		   that consists of only characters that are included in character
		   string, string2.
-----------------------------------------------------------------------------
*/
#include    "string.h"

size_t  strspn(const char *string1, const char *string2)
{
    int     a = 0;
    char    *b;

    while(*string1) {
        b = (char *)string2;
        while(1)    {
            if (!*b)                return  a;
            if (*b++ == *string1)   break;
        }
        string1++;
        a++;
    }
    return  -1;
}
