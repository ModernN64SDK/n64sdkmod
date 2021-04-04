/*
=============================================================================
        Copyright (C) 1997-1999 NINTENDO Co.,Ltd.
        
        $RCSfile: strstr.c,v $
        $Revision: 1.1 $
        $Date: 1999/04/14 07:18:42 $
=============================================================================
function name: strstr
-----------------------------------------------------------------------------
format:   #include <string.h>
        char *strstr(const char *string1, const char *string2);
argument:   string1     character string to be found
        	string2     character string to find
return value:  pointer of character string, string2 that is included in 
		   character string, string1. If character string, string2 is not
		   included,  NULL is returned.
explanation:   finds if character string, string2 is included in character
		   string, string1.  
        	if included, its pointer is returned.  
        	if not included, NULL is returned
-----------------------------------------------------------------------------
*/
#include    "string.h"

char *strstr(const char *string1, const char *string2)
{
    char    *a, *b;

    while(*string1) {
        a = (char *)string1;
        b = (char *)string2;
        while(1)    {
            if (!*b)    return  (char *)string1;
            if (*b++ != *a++)   break;
        }
        string1++;
    }
    return  NULL;
}
