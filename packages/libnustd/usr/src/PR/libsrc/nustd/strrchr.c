/*
=============================================================================
        Copyright (C) 1997-1999 NINTENDO Co.,Ltd.
        
        $RCSfile: strrchr.c,v $
        $Revision: 1.1 $
        $Date: 1999/04/14 07:18:42 $
=============================================================================
function name: strrchr
-----------------------------------------------------------------------------
format:   #include <string.h>
        char *strrchr(const char *str, int c)
argument:   string     character string to be found
        	c  	     character code to be found 
return value:  pointer of the last character code c that was found.  If not
		   found, NULL is returned.  
explanation:   finds a character code c from character string, string, and 
		   return the pointer that was last found.   If not found, NULL
		   is returned.         
------------------------------------------------------------------------------
*/
#include    "string.h"

char *strrchr(const char *str, int c)
{
    char    *p = NULL;
    while(1)    {
        if (*str == c)  p = (char *)str;
        if (!(*str++))  break;
    }
    return  p;
}
