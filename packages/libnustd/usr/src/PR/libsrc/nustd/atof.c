/*
=============================================================================
        Copyright (C) 1997-1999 NINTENDO Co.,Ltd.
        
        $RCSfile: atof.c,v $
        $Revision: 1.1 $
        $Date: 1999/04/14 07:18:39 $
=============================================================================
Function name : atof
-----------------------------------------------------------------------------
Format:  	#include <stdlib.h>
        	double atof(const char *string);
Argument:   string  character string
Return value: double type numeric
Explanation:  Converts string character string to double type numeric  

        Specify character string, string as follows:

        [whitespace][{sign}][digits][.digits][{e | E}[sign]digits]

            <whitespace>    characters handled as blank characters(see isspace)
            <sign>          + or -
            <digits>        decimal character string which digit is one or more  

        An exponent part following e and E can be put after the decimal
        number. Conversion is terminated when unrecognizable character appears
        as a part of number. 
-----------------------------------------------------------------------------
*/
#include    "stdlib.h"
#include    "math.h"
#include    "ctype.h"

double atof(const char *string)
{
    double a = 1.0, b = 0.0,  c = 0;

    while(isspace(*string))
      string++;
    
    if (!*string)   return  0.0;
    if (*string == '+') string++;
    if (*string == '-') {string++; a = -1.0;}
    while(*string)  {
        if (*string == '.') {
            if (b)  break;
            b = 10.0;
            string++;
        }   else    {
            if (!isdigit(*string))  break;
            if (b)  {
                c += ((*string++ - '0') / b);
                b *= 10.0;
            }   else    {
                c *= 10.0;
                c += (*string++) - '0';
            }
        }
    }
    c *= a;

    if (*string == 'E' || *string == 'e')   {
        string++;
        a = b = 0;
        if (*string == '+') string++;
        if (*string == '-') {string++; a = 1.0;}

        while(isdigit(*string)) {
            b *= 10;
            b += (*string++) - '0';
        }
        b = pow(10, b);
        if (a)  c /= b;
        else    c *= b;
    }
    return  c;
}
