/*
=============================================================================
        Copyright (C) 1997-1999 NINTENDO Co.,Ltd.
        
        $RCSfile: atoi.c,v $
        $Revision: 1.1 $
        $Date: 1999/04/14 07:18:39 $
=============================================================================
Function name : atoi
-----------------------------------------------------------------------------
Format:  	#include <stdlib.h>
        	int atoi(const char *s)
Argument:   string character string
Return value: int type numeric
Explanation:  Converts string character string to int type numeric. 

         Specify character string, string as follows: 

        [whitespace][{sign}][digits]

            <whitespace>    characters handled as blank characters(see isspace)
            <sign>          + or -
            <digits>        decimal value which digit is one or more

       Conversion is terminated when unrecognizable characger appears as a 
       part of number.
-----------------------------------------------------------------------------
*/
#include    "stdlib.h"
#include    "ctype.h"

int atoi(const char *s)
{
    int sign = 1,digits = 0;

    while(isspace(*s))
      s++;

    if (*s =='-')           {sign = -1; s++;}
    else    if (*s =='+')   s++;

    while(isdigit(*s))  {
        digits *= 10;
        digits += (*s++) - '0';
    }
    digits *= sign;
    return digits;
}
