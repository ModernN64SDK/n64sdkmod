/*
=============================================================================
        Copyright (C) 1997-1999 NINTENDO Co.,Ltd.
        
        $RCSfile: strcspn.c,v $
        $Revision: 1.1 $
        $Date: 1999/04/14 07:18:41 $
=============================================================================
function name: strcspn
-----------------------------------------------------------------------------
format:   #include <string.h>
        size_t  strcspn(const char *string1, const char *string2);
argument:   string1     character string to be found
        	string2     character string to find
return value: length of the head
explanation:  calculates the length of the head in charcter string, string1
		  which consists of characters that are not included in character
		  string, string2.   
-----------------------------------------------------------------------------
*/
#include    "string.h"

size_t  strcspn(const char *string1, const char *string2)
{
    int     a = 0;
    char    *b;

    while(*string1) {
        b = (char *)string2;
        while(*b)
            if (*b++ == *string1)   return  a;
        string1++;
        a++;
    }
    return  a;
}
