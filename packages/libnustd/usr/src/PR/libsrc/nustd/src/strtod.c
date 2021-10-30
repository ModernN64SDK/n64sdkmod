/*
=============================================================================
        Copyright (C) 1997-1999 NINTENDO Co.,Ltd.
        
        $RCSfile: strtod.c,v $
        $Revision: 1.1 $
        $Date: 1999/04/14 07:18:42 $
=============================================================================
function name: strtod
-----------------------------------------------------------------------------
format:   #include <string.h>
          double strtod(const char *string, char **endptr)
argument:   string   character string to be converted
        	endptr   destination of storage of pointer to unrecognizable
			   character string
return value: converted value of character string,string to double type.   
explanation:  converts character string, string to double type.   

        character string, string is specified as following format

        [whitespace][{sign}][digits][.digits][{e | E}[sign]digits]

            <whitespace>    characters handled as blank characters(see isspace)
            <sign>          + or -
            <digits>        decimal character string of more than 1 digit 
        An exponent part folloing e and E can be put after the decimal number.
	  Conversion is terminated when unrecognizable character appears as a 
	  part of number. 

        When endptr is not NULL, set the pointer to character string which 
	  conversion was terminated to *endptr
-----------------------------------------------------------------------------
*/
#include    "string.h"
#include    "math.h"
#include    "ctype.h"

double strtod(const char *string, char **endptr)
{
    double a = 1.0, b = 0.0,  c = 0, d;

    while(isspace(*string))
      string++;

    if (!*string)   return  0.0;
    if (*string == '+') string++;
    if (*string == '-') {string++; a = -1.0;}

    c = 0;
    while(*string)  {
        if (*string == '.') {
            if (b)  break;
            b = 10.0;
            string++;
        }   else    {
            if (!isdigit(*string))  break;
            if (b)  d = (*string++ - '0') / b;
            else    d = (*string++) - '0';

            if (c > (DBL_MAX-d)/10) return  DBL_MAX;

            if (b)  b *= 10.0;
            else    c *= 10.0;
            c += d;
        }
    }
    c *= a;

    if (*string == 'E' || *string == 'e')    {
        string++;
        a = b = 0;
        if (*string == '+') string++;
        if (*string == '-') {string++; a = 1.0;}

        while(*string)  {
            if (!isdigit(*string))  break;
            b *= 10;
            b += (*string++) - '0';
        }
        b = pow(10, b);
        if (a)  c /= b;
        else    c *= b;
    }

    if (endptr != NULL) *endptr = (char *)string;
    return  c;
}
