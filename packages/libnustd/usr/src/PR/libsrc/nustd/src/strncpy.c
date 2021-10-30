/*
=============================================================================
        Copyright (C) 1997-1999 NINTENDO Co.,Ltd.
        
        $RCSfile: strncpy.c,v $
        $Revision: 1.1 $
        $Date: 1999/04/14 07:18:42 $
=============================================================================
function name: strncpy
-----------------------------------------------------------------------------
format:   #include <string.h>
        char *strncpy(char *str1, const char *str2, size_t  n);
argument:   str1   character string buffer of destination to be copied
        	str2   character string buffer of source to be copied
        	m  	 character number to be copied
return value: pointer of str1 
explanation:  copies character string, str2 to character string, str1 by the
		  number of n character.   
-----------------------------------------------------------------------------
*/
#include    "string.h"

char *strncpy(char *str1, const char *str2, size_t  n)
{
    char *p = str1;
    while((*str2) && n--)   *p++ = *str2++;
    *p = '\0';
    return  str1;
}
