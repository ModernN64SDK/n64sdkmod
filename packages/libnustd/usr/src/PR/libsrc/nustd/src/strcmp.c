/*
=============================================================================
        Copyright (C) 1997-1999 NINTENDO Co.,Ltd.
        
        $RCSfile: strcmp.c,v $
        $Revision: 1.1 $
        $Date: 1999/04/14 07:18:41 $
=============================================================================
function name: strcmp
-----------------------------------------------------------------------------
format:   #include <string.h>
        int strcmp(const char *str1, const char *str2)
argument:   str1   character string buffer to be compared   
        	str2   character string buffer to be compared
return value: if less than 0,   str1 is smaller than str2  
        	  if 0,	        str1 is equal to str2
        	  if more than 0,   str1 is larger than  str2  
explanation:  compares character stirng, str1 with character string, str2,    
-----------------------------------------------------------------------------
*/
#include    "string.h"

int strcmp(const char *str1, const char *str2)
{
    unsigned char   c1 = 1,c2;
    while(c1)   {
        c1=*str1++;
        c2=*str2++;
        if (c1 != c2)   return c1 - c2;
    }
    return 0;
}
