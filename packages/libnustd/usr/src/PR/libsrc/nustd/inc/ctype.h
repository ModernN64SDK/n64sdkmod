/*
=============================================================================
        Copyright (C) 1997-1999 NINTENDO Co.,Ltd.
        
        $RCSfile: ctype.h,v $
        $Revision: 1.1 $
        $Date: 1999/04/14 07:18:39 $
=============================================================================
function name: iscntrl
-----------------------------------------------------------------------------
format:   #include <ctype.h>
          int iscntrl(int c);
argument: c character code 
return value: returns something other than 0 when conditions are met. 
explanation:  checks if argument c is a control character (macro)
=============================================================================
function name: isdigit
-----------------------------------------------------------------------------
format:   #include <ctype.h>
          int isdigit(int c);
argument: c character code
return value: returns something other than 0 when conditions are met 
explanation:  checks if argument c is '0'~'9' (macro)
=============================================================================
function name: islower
-----------------------------------------------------------------------------
format:   #include <ctype.h>
          int islower(int c);
argument: c character code
return value: returns something other than 0 when conditions are met 
explanation:  checks if argument c is lower case 'a'~'z' (macro) 
=============================================================================
function name: isupper
-----------------------------------------------------------------------------
format:   #include <ctype.h>
          int isupper(int c);
argument: c character code 
return value: returns something other than 0 when conditions are met 
explanation:  checks if argument c is upper case 'A'~'Z' (macro) 
=============================================================================
function name: isxdigit
-----------------------------------------------------------------------------
format:   #include <ctype.h>
          int isxdigit(int c);
argument: c character codeb
return value: returns something other than 0 when conditions are met
explanation:  checks if argument c is 'A'~'F','a'~'f','0'~'9' represented by
		  hexadecimal (macro)          
=============================================================================
function name: isalnum
-----------------------------------------------------------------------------
format:   #include <ctype.h>
          int isalnum(int c);
argument: c character code 
return value: returns something other than 0 when conditions are met 
explanation:  checks if argument c is alpha-numeric 'A'~'Z', 'a'~'z', '0'~'9' 
		  (macro)
=============================================================================
function name: isascii
-----------------------------------------------------------------------------
format:   #include <ctype.h>
          int isascii(int c);
argument: c character code 
return value: returns something other than 0 when conditions are met 
explanation: checks if argument c is 0x00~0x7f (macro) 
-----------------------------------------------------------------------------
=============================================================================
function name: isspace
-----------------------------------------------------------------------------
format:   #include <ctype.h>
          int isspace(int c);
argument: c character code 
return value: returns something other than 0 when conditions are met 
explanation:  checks if argument c is 0x09~0x0d, 0x20 (macro)
-----------------------------------------------------------------------------
*/
#ifndef __CTYPE_H__
#define __CTYPE_H__

#ifdef _LANGUAGE_C_PLUS_PLUS
extern "C" {
#endif

extern  unsigned char __MojiStat[0x101];

#define _ISCNTRL     0x01    /*  0x00~0x1f, 0x7f */
#define _ISDIGIT     0x02    /*  '0'~'9'    */
#define _ISLOWER     0x04    /*  'a'~'z'    */
#define _ISUPPER     0x08    /*  'A'~'Z'    */
#define _ISXDIGIT    0x10    /*  'A'~'F', 'a'~'f', '0'~'9' */
#define _ISSPACE     0x20    /*  0x09-0x0d , 0x20 */


#define iscntrl(c)  ((__MojiStat+1)[c] & _ISCNTRL)
#define isdigit(c)  ((__MojiStat+1)[c] & _ISDIGIT)
#define islower(c)  ((__MojiStat+1)[c] & _ISLOWER)
#define isupper(c)  ((__MojiStat+1)[c] & _ISUPPER)
#define isxdigit(c) ((__MojiStat+1)[c] & _ISXDIGIT)
#define isalnum(c)  ((__MojiStat+1)[c] & (_ISDIGIT | _ISLOWER | _ISUPPER))
#define isascii(c)  ((__MojiStat+1)[c] & (_ISLOWER | _ISUPPER))
#define isspace(c)  ((__MojiStat+1)[c] & _ISSPACE)

#ifdef _LANGUAGE_C_PLUS_PLUS
}
#endif
#endif
