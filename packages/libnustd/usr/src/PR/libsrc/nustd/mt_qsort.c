/*
=============================================================================
        Copyright (C) 1997-1999 NINTENDO Co.,Ltd.
        
        $RCSfile: mt_qsort.c,v $
        $Revision: 1.1 $
        $Date: 1999/04/30 02:20:09 $
=============================================================================
Function name: mt_qsort
-----------------------------------------------------------------------------
Syntax:         #include <stdlib.h>
                void mt_qsort(void *base, size_t num, size_t width,
                    int (*compare)(const void *, const void *));
Arguments:      base     Pointer indicating array
                num      Number of elements in array
                width    Size (in bytes) of 1 element in array
                compare  Compare function
Returned value: None
Explanation:    Performs a quick sort on an array.
                The "compare" argument is the pointer to a compare function created by the user.

            int compare((type *)elem1, (type *)elem2);

            (type *) can be any type.
            The returned value:
                < 0       elem1 is smaller than elem2
                = 0       elem1 is equal to elem2
                > 0       elem1 is larger than elem2
-----------------------------------------------------------------------------
*/
#include    "stdlib.h"

static  void    _mt_qsort_swap(char *p1, char *p2, int width)
{
    int k,c;

    for (k = 0; k<width; k++)   {
        c = *p1;
        *p1++ = *p2;
        *p2++ = c;
    }
}


static  void    _mt_qsort(void *base, size_t num, size_t width,
            int (*compare)(const void *, const void *), char **w)
{
    int     a, i;
    char    *p1, *p2;

    if (num < 1)    return;

    p1 = p2 = base;
    p2 += width;

    for (i=0; i<num-1; i++) {
        a = compare(p1, p2);
        if (a>0)    {
            if (((int)(p2-p1)/width) <2)    *w = p2;
            else    {
                *w = p1;
                *w += width;
                _mt_qsort_swap(p1, *w, width);
            }
            _mt_qsort_swap(p1, p2, width);
            p1 = *w;
        }
        p2 += width;
    }
    a = ((int)(*w - (char *)base)/width)+1;

    *w = p1;
    if (a>1)   _mt_qsort(base, a, width, compare, w);

    *w = p1;
    *w += width;
    a = num-a;
    if (a>1)    _mt_qsort(*w, a, width, compare, w);
}

void mt_qsort(void *base, size_t num, size_t width,
            int (*compare)(const void *, const void *))
{
  char *w ;

  w = base;
  _mt_qsort(base, num, width, compare, &w);
}
