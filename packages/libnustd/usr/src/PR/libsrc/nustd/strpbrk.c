/*
=============================================================================
        Copyright (C) 1997-1999 NINTENDO Co.,Ltd.
        
        $RCSfile: strpbrk.c,v $
        $Revision: 1.1 $
        $Date: 1999/04/14 07:18:42 $
=============================================================================
function name: strpbrk
-----------------------------------------------------------------------------
format:   #include <string.h>
        char    *strpbrk(const char *string1, const char *string2);
argument:   string1    character string to be found
        	string2    character string to find
return value: pointer that was found
        	  if not found, NULL is returned.  
explanation:  finds the location where either one of characters of character
		  string, string2 is first appeared in character string, string1.   
------------------------------------------------------------------------------
*/

#include    "string.h"

char    *strpbrk(const char *string1, const char *string2)
{
    char    *b;

    while(*string1) {
        b = (char *)string2;
        while(*b)
            if (*b++ == *string1)   return  (char *)string1;
        string1++;
    }
    return  NULL;
}
