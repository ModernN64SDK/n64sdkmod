/*
=============================================================================
        Copyright (C) 1997-1999 NINTENDO Co.,Ltd.
        
        $RCSfile: malloc_memdsp.c,v $
        $Revision: 1.1 $
        $Date: 1999/04/30 02:13:24 $
=============================================================================
Function name: malloc_memdsp
-----------------------------------------------------------------------------
Syntax:          #include <malloc.h>
                 int malloc_memdsp(unsigned char *s, int size)
Arguments:       s  Character string pointer
                 size Character string size/60
Returned value:  Used size/60
Explanation:     Information about the allocated memory region is returned as character information at 60 columns per line using the specified one-dimensional character string.  Note that the character string must be a multiple of 60 x 2.


        Example of showing character information:
            #define MEMCHECK_STRINGLEN  10
            void dsp_memcheck(void)
            {
                int i, j;
                char    s[MEMCHECK_STRINGLEN*60];

                i = malloc_memdsp(s, MEMCHECK_STRINGLEN);
                for (j=0; j<i;j++)  osSyncPrintf("%s\n",s+j*60);
            }

        Contents of the character information:
        The character information expresses 1 set of data in two lines.
            First line
                "########:$$$$$$$$-&&&&&&&&"
                    ########  -- address where header info is stored
                    $$$$$$$$  -- Start address of memory block that is
                              reserved or released.     
                    &&&&&&&&  -- End address of memory block that is
                              reserved or released.

            Second line
            The 2nd line expresses the memory block's header information
                    "        (########)[$$$$$$$$] FREE"
                    "        (########)[$$$$$$$$] USE"
                    "        (########)[$$$$$$$$] ALIN"

                    ######## -- The memory block's byte size
                    $$$$$$$$ -- The address of the next header 
                             information

                    Character string at end of character string:
                        FREE -- Memory block released, region is usable.
                        USE  -- Memory block is used.
                        ALIN -- Memory block of size =0 and only header
                             for correcting alignment when the memalign 
                             function is used.


            When the contents are incorrect:
                "malloc_memdsp error:######## over area $$$$$$$$-&&&&&&&&"
                    The "address of the next header information" in the
                   header information is outside the bounds of the
                   region reserved by the InitHeap() function.

                "malloc_memdsp error:check InitHeap()"
                    Either malloc_memdsp() has been called without doing 
                    the InitHeap() function, or the header information 
                    in the first memory block is incorrect information.    

-----------------------------------------------------------------------------
*/
#include    <ultra64.h>
#include    "string.h"
#include    "malloc.h"
#include    "_malloc.h"

int malloc_memdsp(unsigned char *s, int size)
{
    if ((int)malloc_ptr == -1)  {
            sprintf(&s[ 0], "malloc_memdsp error:check InitHeap()");
            sprintf(&s[60], "");
            return  2;
    }
    return  _malloc_memdsp(malloc_ptr, s, size);
}
