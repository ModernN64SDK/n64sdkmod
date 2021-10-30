/*
=============================================================================
        Copyright (C) 1997-1999 NINTENDO Co.,Ltd.
        
        $RCSfile: srand.c,v $
        $Revision: 1.1 $
        $Date: 1999/04/14 07:18:41 $
=============================================================================
function name: srand
-----------------------------------------------------------------------------
format:   #include <stdlib.h>
           void srand(unsigned seed);
argument:      seed    genus of random number   
return value:  none
explanation:   specifies a genus of pseudo-random number that a function 
		   rand()returns. 
=============================================================================
function name: rand
-----------------------------------------------------------------------------
format:   #include <stdlib.h>
           int rand(void);
argument:     none
return value: returns pseudo-random number  
explanation:  returns pseudo-random number of 0~0x7fff 
-----------------------------------------------------------------------------
*/
#include <stdlib.h>

/* #define RAND_MAX    0x7fff move to math.h */
static  unsigned rand_data;

void srand(unsigned seed)
{
    rand_data = seed;
}

int rand(void)
{
    rand_data *= 20077;
    rand_data += 12345;
    return  (rand_data >> 16) & RAND_MAX;
}
