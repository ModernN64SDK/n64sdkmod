/*
=============================================================================
        Copyright (C) 1997-1999 NINTENDO Co.,Ltd.
        
        $RCSfile: strlen.c,v $
        $Revision: 1.2 $
        $Date: 1999/04/30 10:43:39 $
=============================================================================
Function name: _nstrlen
-----------------------------------------------------------------------------
Format:   #include <string.h>
        size_t  _nstrlen(const char *string);
Argument:   string    character string buffer 
Return value: string   length of character string
Explanation:   returns the length of character string of string 
-----------------------------------------------------------------------------
*/

#include    "string.h"

size_t  _nstrlen(const char *string)
{
    size_t  len = 0;
    while(*string++)    len++;
    return  len;
}
