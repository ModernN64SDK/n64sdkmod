/*======================================================================*/
/*      Mirro House                                                     */
/*      om_func.c                                                       */
/*                                                                      */
/*      Copyright (C) 1998 MONEGI CORPORATION                           */
/*                                                                      */
/*======================================================================*/
#include    <nusys.h>
#include    "math.h"
#include    <stdlib.h>

f32 SinTable[] = {
    0.000000,   
    0.017452,   0.034899,   0.052336,   
    0.069756,   0.087156,   0.104528,   
    0.121869,   0.139173,   0.156434,   
    0.173648,   0.190809,   0.207912,   
    0.224951,   0.241922,   0.258819,   
    0.275637,   0.292372,   0.309017,   
    0.325568,   0.342020,   0.358368,   
    0.374607,   0.390731,   0.406737,   
    0.422618,   0.438371,   0.453990,   
    0.469472,   0.484810,   0.500000,   
    0.515038,   0.529919,   0.544639,   
    0.559193,   0.573576,   0.587785,   
    0.601815,   0.615661,   0.629320,   
    0.642788,   0.656059,   0.669131,   
    0.681998,   0.694658,   0.707107,   
    0.719340,   0.731354,   0.743145,   
    0.754710,   0.766044,   0.777146,   
    0.788011,   0.798636,   0.809017,   
    0.819152,   0.829038,   0.838671,   
    0.848048,   0.857167,   0.866025,   
    0.874620,   0.882948,   0.891007,   
    0.898794,   0.906308,   0.913545,   
    0.920505,   0.927184,   0.933580,   
    0.939693,   0.945519,   0.951057,   
    0.956305,   0.961262,   0.965926,   
    0.970296,   0.974370,   0.978148,   
    0.981627,   0.984808,   0.987688,   
    0.990268,   0.992546,   0.994522,   
    0.996195,   0.997564,   0.998630,   
    0.999391,   0.999848,   1.000000,   
};

/* ----------------------------------------------------------------------------
name:
    lookAtXYDeg
synoopsis:
    s32 lookAtXYDeg(s32 x, s32 y, f32 *dist);
description:
    Obtains the angle and distance of the line described by 2 points (0, 0 and  <x>, <y>).
parameter:
    s32 x;
        x-coordinate 
    s32 y;
        y-coordinate 
    f32 *dist;
        First pointer: returns the distance
return:
    Angle formed by the 2 points (degrees)
notes:
    When used for an analog stick, gives the stick deflection angle and degree.
---------------------------------------------------------------------------- */
s32 om_expad_lookat_xydeg(s32 x, s32 y, f32 *dist)
{
    u16 i;
    s16 a, b, c;
    f32 xy;
#if 1
    *dist = hypotf(x, y);
#else
    *dist = sqrtf(x*x + y*y);
#endif
    if (!x && !y)   return  -1;
    xy = abs(y) / (*dist);          /* calculate sin(theta) = y / (sqrt(x^2 + y^2)) */


    a = 0;
    b = 90;
    for (i=0; i<7; i++) {
        c = a + ((b-a) / 2);
        if (SinTable[c] < xy)   a = c;
        else                    b = c;
    }

    if (x<0 && y>=0)    c = 180 - c;
    if (x<0 && y<0)     c += 180;
    if (x>=0 && y<0)    c = 360 - c;    /*  Correct the angle depending on the quadrant */

    return  c;
}

/* ----------------------------------------------------------------------------
name:
    lookAtXYDegf
synopsis:
    s32 lookAtXYDegf(float x, float y, float *dist);
description:
    Obtains the angle and distance of the line described by 2 points (0, 0 and  <x>, <y>). 
parameter:
    float   x;
        x-coordinate
    float   y;
        y-coordinate 
    float   *dist;
        First pointer: returns the distance
 
return:
        Angle formed by the 2 points (degrees)
 
notes:
    	A type float version of the function lookAtXYDeg()
---------------------------------------------------------------------------- */
float om_expad_lookat_xydegf(float x, float y, float *dist)
{
    u16 i;
    float   a, b, c;
    f32 xy;

    *dist = hypotf(x, y);
    if (!x && !y)   return  -1;
    xy = fabsf(y) / (*dist);            /*  calculate sin(theta) = y / (sqrt(x^2 + y^2)) */

    a = 0;
    b = 90;
    for (i=0; i<7; i++) {
        c = a + ((b-a) / 2);
        if (SinTable[(int)c] < xy)  a = c;
        else                    b = c;
    }

    if (x<0 && y>=0)    c = 180 - c;
    if (x<0 && y<0)     c += 180;
    if (x>=0 && y<0)    c = 360 - c;    /* Correct the angle depending on the quadrant */

    return  c;
}


