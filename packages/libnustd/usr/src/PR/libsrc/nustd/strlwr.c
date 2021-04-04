/*---------------------------------------------------------------------*
        Copyright (C) 1997-1999 NINTENDO Co.,Ltd.
        
        $RCSfile: strlwr.c,v $
        $Revision: 1.1 $
        $Date: 1999/04/14 07:18:42 $
 *---------------------------------------------------------------------*/
#include    "string.h"

char *strlwr(char *str)
{
    char    *p = str;
    while(*p)   {
        if (*p>='A' && *p<='Z') *p += 'a' - 'A';
        p++;
    }
    return str;
}
