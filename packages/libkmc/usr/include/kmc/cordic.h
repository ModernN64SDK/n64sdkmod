#ifndef _CORDEC_H
#define _CORDEC_H
/*
	cordec.h
	sin,cos,tan,atan CORDEC
*/

#if 0
/*** for Turboc ***/
#define TURBOC	1
#define MBIT	0x40000000
#define CBIT	30
#define XLONG	long
#define XDOUBLE	double
#else
/**** for GCC ****/
#define GCC		1
#define MBIT	0x1000000000000000LL
#define CBIT	52
#define XLONG	long long
#define XDOUBLE	long double
#endif



#define PI	3.141592653589793238
#define K	0.607252935008881278

double __matherr(int type , char *name,double arg1,double arg2,double retval);

#endif
