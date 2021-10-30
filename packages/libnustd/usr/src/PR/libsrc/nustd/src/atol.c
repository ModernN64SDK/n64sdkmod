/*
=============================================================================
        Copyright (C) 1997-1999 NINTENDO Co.,Ltd.
        
        $RCSfile: atol.c,v $
        $Revision: 1.1 $
        $Date: 1999/04/14 07:18:39 $
=============================================================================
Function name : atol
-----------------------------------------------------------------------------
Format:	#include <stdlib.h>
        	long atol(const char *string)
Argument:   string character string
Return value: long type numeric
Explanation:  Converts string character string to long type numeric     
        
              Specify character string, string as follows:

        [whitespace][{sign}][digits]

            <whitespace>    characters handled as blank characters(see isspace)
            <sign>          + or -
            <digits>        decimal character string which digit is one or more

 				    Conversion is terminated when unrecognizable
                            character appears as a part of number.      
-----------------------------------------------------------------------------
*/
#include    "stdlib.h"
#include    "ctype.h"

long atol(const char *string)
{
    long a = 1, b = 0;

    while(isspace(*string))
      string++;

    if (!*string)   return  0;
    if (*string == '+') string++;
    if (*string == '-') {string++; a = -1;}

    b = 0;
    while(isdigit(*string)) {
        b *= 10;
        b += (*string++) - '0';
    }
    b *= a;
    return  b;
}
