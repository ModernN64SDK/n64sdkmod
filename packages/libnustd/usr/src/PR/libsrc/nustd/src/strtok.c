/*
=============================================================================
        Copyright (C) 1997-1999 NINTENDO Co.,Ltd.
        
        $RCSfile: strtok.c,v $
        $Revision: 1.1 $
        $Date: 1999/04/14 07:18:42 $
=============================================================================
function name: strtok
-----------------------------------------------------------------------------
format:   #include <string.h>
        char *strtok(char *string1, const char *string2);
argument:   string1     when the first token is taken out, a pointer of 
				character string is specified, 
                        when next token is taken out, NULL is specififed. 
        	string2     delimiting character string to take out a token
return value: pointer of token   
              if no more token is available, NULL is returned.            
explanation:  Take out a token from character string, string1 using the 
		  characters in character string, string2 as token separator. 
		  When taking out the next token, NULL is specifiefd in character
		  string, string1.
-----------------------------------------------------------------------------
*/
#include    "string.h"

char *strtok(char *string1, const char *string2)
{
    static  char    *ptr = NULL;
    char    *a;

    if (string1 != NULL)    ptr = string1;
    if (ptr == NULL)        return  NULL;
    a = ptr;
    ptr = strpbrk(ptr, string2);
    if (ptr != NULL)    *ptr++ = NULL;                      /*  End Of String   */
    return  a;
}
