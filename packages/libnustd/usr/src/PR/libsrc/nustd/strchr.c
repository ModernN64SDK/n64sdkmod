/*
=============================================================================
        Copyright (C) 1997-1999 NINTENDO Co.,Ltd.
        
        $RCSfile: strchr.c,v $
        $Revision: 1.2 $
        $Date: 1999/04/30 10:43:30 $
=============================================================================
Function name: _nstrchr
-----------------------------------------------------------------------------
Format:  #include <string.h>
        char *_nstrchr(const char *string, int c)
Argument:  string  character string to be found
       	    c  character code to be found
return value:  a pointer of the first character code, c that was found.  If
		   not found, NULL is returned.         
explanation:   finds a character code, c from character string, string, and
		returns the first pointer that was found.  If not found, NULL
		is returned
-----------------------------------------------------------------------------
*/
#include    "string.h"

char *_nstrchr(const char *string, int c)
{
    while(*string)  {
        if (*string == c)   return  (char *)string;
        string++;
    }
    return  NULL;
}


