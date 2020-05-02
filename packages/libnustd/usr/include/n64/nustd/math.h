/*---------------------------------------------------------------------*
        Copyright (C) 1997-1999 NINTENDO Co.,Ltd.
        
        $RCSfile: math.h,v $
        $Revision: 1.5 $
        $Date: 1999/07/16 03:38:36 $
 *---------------------------------------------------------------------*/
#ifndef __MATH_H__
#define __MATH_H__

#ifdef _LANGUAGE_C_PLUS_PLUS
extern "C" {
#endif

#include "mathdef.h"

double  sqrt(double n);
#ifdef __sgi
#pragma intrinsic(sqrt);
#endif
float   _nsqrtf(float n);
double  fabs(double n);
#ifdef __sgi
#pragma intrinsic(fabs);
#endif
float   fabsf(float n);
#ifdef __sgi
#pragma intrinsic(fabsf);
#endif
double  floor(double n);
float   floorf(float n);
double  ceil(double n);
float   ceilf(float n);

double  frexp(double x, int *expptr);
float   frexpf(float x, int *expptr);
double  ldexp(double x, int exp);
float   ldexpf(float x, int exp);
double modf(double x, double *intptr);
float modff(float x, float *intptr);

double pow(double x, double y);
float powf(float x, float y);

double fmod(double x, double y);
float fmodf(float x, float y);

double  hypot(double x, double y);
float   hypotf(float x, float y);

double sin(double t);
double cos(double t);
double tan(double t);
double acos(double x);
double asin(double x);
double atan(double x);
double atan2(double y, double x);
double sinh(double t);
double cosh(double t);
double tanh(double t);
double exp(double x);
double log(double x);
double log10(double x);

float _nsinf(float t);
float _ncosf(float t);
float tanf(float t);
float acosf(float x);
float asinf(float x);
float atanf(float x);
float atan2f(float y, float x);
float sinhf(float t);
float coshf(float t);
float tanhf(float t);
float expf(float x);
float logf(float x);
float log10f(float x);

#ifdef _LANGUAGE_C_PLUS_PLUS
}
#endif
#endif
