/*
=============================================================================
        Copyright (C) 1997-1999 NINTENDO Co.,Ltd.
        
        $RCSfile: bsearch.c,v $
        $Revision: 1.1 $
        $Date: 1999/04/14 07:18:39 $
=============================================================================
Function name : bsearch
-----------------------------------------------------------------------------
Format:	#include <stdlib.h>
        	void *bsearch(const void *key, const void *base, size_t num,
            size_t width, int (*compare)(const void *, const void *));
Argument:  	key       key to be found
        	base      a pointer of array to be found
        	num       the number of elements of array to be found
        	width     size of one element (byte unit)of array to be found
        	compare   a pointer of a comparison function that a user created
Return value: a pointer of the element that was found. If not found, NULL is
              returned.    
Explanation: Finds an element which matches the argument key among the array
             that is sorted in ascending order using a binary search.  It 
             cannot be found properly unless the array is in ascending order.                                                 
             The argument compare is a pointer of the comparison function a 
             user created. 

            int compare(const void *elem1, const void *elem2);

            if return value is
                less than 0   	elem1 is smaller than elem2
                0             	elem1 is equal to elem2
                greater than 0   	elem1 is larger than elem2 
-----------------------------------------------------------------------------
*/
#include "stdlib.h"

void *bsearch(const void *key, const void *base, size_t num,
                size_t width, int (*compare)(const void *, const void *))
{
  char *p;
  size_t i = 0, j = num -1;
  size_t k;
  int cmp;
  
  do
    {
      k = (j + i) /2;
      p = (char *)base + width*k;
      cmp = compare(key,p);
      if(cmp == 0)
	{
	  while(((void *)p > base) && !compare(key,p-width)) 
	    p -= width;
	  return(p);
	}
      else if(cmp < 0)
	j = k-1;
      else
	i = k+1;
    }
  while(j >= i);
  
  return NULL;
}
