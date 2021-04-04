/*
=============================================================================
        Copyright (C) 1997-1999 NINTENDO Co.,Ltd.
        
        $RCSfile: strcpy.c,v $
        $Revision: 1.1 $
        $Date: 1999/04/14 07:18:41 $
=============================================================================
function name: strcpy
-----------------------------------------------------------------------------
format:   #include <string.h>
        char *strcpy(char *str1, const char *str2);
argument:   str1    character string buffer of destination to be copied
        	str2    character string buffer of source to be copied
return value:  a pointer of str1  
explanation:   copies character string, str2 to character string, str1  
-----------------------------------------------------------------------------
*/
#include    "string.h"

char *strcpy(char *str1, const char *str2)
{
    char *p;
    p = str1;
    while(*str2) *p++ = *str2++;
    *p = '\0';
    return  str1;
}
