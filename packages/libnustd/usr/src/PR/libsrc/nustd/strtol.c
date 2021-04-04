/*
=============================================================================
        Copyright (C) 1997-1999 NINTENDO Co.,Ltd.
        
        $RCSfile: strtol.c,v $
        $Revision: 1.2 $
        $Date: 1999/04/30 10:45:39 $
=============================================================================
Function name: strtol
-----------------------------------------------------------------------------
Format:   #include <stdlib.h>
          long strtol(const char *string, char **endptr, int base)
Argument:   string     character string to be converted
        	endptr     destination of storage of pointer to unrecognizable
			     character string
	      base       if 2~36, then radix.  acquire radix from string format
			     using 0  
Return value: converted value of character string, string to long type 
Explanation:  converts character string,  string to long type.   

        character string, string is specified as folloing format.    

        [whitespace][{sign}][{0}][{x | X}][digits]

            <whitespace>    characters handled as blank characters(see isspace)
            <sign>          + or -
            <digits>        numeric character of more than 1 digit  

if 0 is specified as radix base, radix will be handled as follows:     
        unless the first of string is '0'and the second character is 'x' or
	  `X`, octal number.    
        if the first of string is '0' and the second character is  'x' or 'X',
	  hexadecimal number.   
        if the first of string is '1' ~ '9', decimal number     

        conversion is terminated when an unrecognizable character appears as
	  a part of number.

        if endptr is not NULL, set a pointer to character string which
	  conversion was terminated to *endptr  
-----------------------------------------------------------------------------
*/
#include    "stdlib.h"
#include    "math.h"
#include    "ctype.h"
#include    "string.h"

static  char hexstring[] = {"0123456789abcdefghijklmnopqrstuvwxyz"};

long strtol(const char *string, char **endptr, int base)
{
    long a = 1, b = 0;
    char    c, *d;

    while(isspace(*string))
      string++;

    if (!*string)   return  0;
    if (*string == '+') string++;
    if (*string == '-') {string++; a = -1;}

    b = 0;
    if (!base)  {
        if (*string == '0') {
            string++;
            if (*string == 'x' || *string == 'X')   {base = 16; string++;}
            else                                    base = 8;
        }   else    base = 10;
    }
    while(*string)  {
        if (!isalnum(*string)) break;
        c = *string++;
        if (c>='A' && c<='Z')   c += 'a' - 'A';
        d = _nstrchr(hexstring, c);
        if (d == NULL)  {string --; break;}
        c = d - &hexstring[0];
        if (c >= base)  {string --; break;}
        if (b > (LNG_MAX-c)/base)   {
            if (a==1)   return  LNG_MAX;
            else        return  -LNG_MAX-1;
        }
        b *= base;
        b += c;
    }
    b *= a;
    if (endptr != NULL) *endptr = (char *)string;
    return  b;
}
