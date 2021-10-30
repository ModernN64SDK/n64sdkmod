/*
=============================================================================
        Copyright (C) 1997-1999 NINTENDO Co.,Ltd.
        
        $RCSfile: strncat.c,v $
        $Revision: 1.1 $
        $Date: 1999/04/14 07:18:42 $
=============================================================================
function name: strncat
-----------------------------------------------------------------------------
format:   #include <string.h>
        char *strncat(char *str1, const char *str2, size_t n);
argument:   str1      character string buffer to be added
        	str2      character string buffer to add
        	n	    character number to be added
return value:  pointer of str1
explanation:   adds character string str2 to the end of character string str1
		   by n character string.   
-----------------------------------------------------------------------------
*/
#include    "string.h"

char *strncat(char *str1, const char *str2, size_t n)
{
    char *p = str1;
    while(*p) p++;
    while((*str2) && n--) *p++ = *str2++; 
    *p = '\0';
    return  str1;
}
