/*
=============================================================================
        Copyright (C) 1997-1999 NINTENDO Co.,Ltd.
        
        $RCSfile: strtoul.c,v $
        $Revision: 1.2 $
        $Date: 1999/04/30 10:45:43 $
=============================================================================
Function name: strtoul
-----------------------------------------------------------------------------
Format:   #include <stdlib.h>
          unsigned long strtoul(const char *string, char **endptr, int base)
Argument:   string   character string to be converted
        	endptr   destination of storage of pointer to unrecognizable 
			   character string
       	base     if 2~36, then radix.  acquire radix from string format
			   using 0. 
Return value: converted value of character string, string to unsigned long type.   
Explanation:  converts character string, string to unsigned long type   
  
      character string, string is specified as following format.   

        [whitespace][{sign}][{0}][{x | X}][digits]

            <whitespace>    characters handled as blank characters(see isspace)
            <sign>          +(- is considered as an unrecognizable character)
            <digits>        numeric character of more than 1 digit

if 0 is specified as radix base, radix will be handles as follows:
        unless the first of string is '0' and the second character is  'x'
	  or `X`, octal number    
        if the first of string is '0' and the second character is  'x' or
	 'X', hexadecimal number   
        if the first of string is '1' ~ '9', decimal number     

        conversion is terminated when unrecognizable character appears as a
	  part of number

        if endptr is not NULL, set a pointer to character string which 
	  conversion was terminated to *endptr.   
-----------------------------------------------------------------------------
*/
#include    "math.h"
#include    "stdlib.h"
#include    "ctype.h"
#include    "string.h"

static  char hexstring[] = {"0123456789abcdefghijklmnopqrstuvwxyz"};

unsigned long strtoul(const char *string, char **endptr, int base)
{
    unsigned long b = 0;
    char    c, *d;

    while(isspace(*string))
      string++;

    if (!*string)   return  0;
    if (*string == '+') string++;
    if (*string == '-') return  0;

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
        if (b > (ULNG_MAX-c)/base)  return  ULNG_MAX;
        b *= base;
        b += c;
    }
    if (endptr != NULL) *endptr = (char *)string;
    return  b;
}
