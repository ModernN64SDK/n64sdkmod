/*
=============================================================================
        Copyright (C) 1997-1999 NINTENDO Co.,Ltd.
        
        $RCSfile: qsort.c,v $
        $Revision: 1.1 $
        $Date: 1999/04/14 07:18:41 $
=============================================================================
function name: qsort
-----------------------------------------------------------------------------
format:   #include <stdlib.h>
           void qsort(void *base, size_t num, size_t width,
                    int (*compare)(const void *, const void *));
argument:   base   	pointer that shows an array
        	num    	the number of elements in an array 
        	width   	the size of one element in an array (byte unit)        	compare   	a function to be compared
return value: none
explanation:  performs a quick-sort of arrays
        	  argument compare is a pointer of a comparison function that a
		  user created   

            int compare((type *)elem1, (type *)elem2);

            (type *)is an optional type
            if the value retured is,
                less than 0    elem1 is smaller than elem2
                0       	 elem1 is equal to elem2
                more than 0    elem1 is larger than elem2 
-----------------------------------------------------------------------------
*/
#include    "stdlib.h"

static  int     i;
static  char    *w;

static  void    _qsort_swap(char *p1, char *p2, int width)
{
    int k,c;

    for (k = 0; k<width; k++)   {
        c = *p1;
        *p1++ = *p2;
        *p2++ = c;
    }
}


static  void    _qsort(void *base, size_t num, size_t width,
            int (*compare)(const void *, const void *))
{
    int     a;
    char    *p1, *p2;

    if (num < 1)    return;

    p1 = p2 = base;
    p2 += width;

    for (i=0; i<num-1; i++) {
        a = compare(p1, p2);
        if (a>0)    {
            if (((int)(p2-p1)/width) <2)    w = p2;
            else    {
                w = p1;
                w += width;
                _qsort_swap(p1, w, width);
            }
            _qsort_swap(p1, p2, width);
            p1 = w;
        }
        p2 += width;
    }
    a = ((int)(w - (char *)base)/width)+1;

    w = p1;
    if (a>1)   _qsort(base, a, width, compare);

    w = p1;
    w += width;
    a = num-a;
    if (a>1)    _qsort(w, a, width, compare);
}

void qsort(void *base, size_t num, size_t width,
            int (*compare)(const void *, const void *))
{
  w = base;
  _qsort(base, num, width, compare);
}
