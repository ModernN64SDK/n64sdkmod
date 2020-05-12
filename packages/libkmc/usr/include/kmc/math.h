#ifndef _MATH_H
#define _MATH_H
/*
	math.h
*/

extern int errno;

#define EDOM	33		/* Math argument error */
#define ERANGE	34		/* Too large */

double	sin(double);
double	cos(double);
double	tan(double);
double	atan(double);
double	atan2(double,double);
double  sqrt(double);
double	asin(double);
double	acos(double);
double	exp(double);
double	sinh(double);
double	cosh(double);
double	tanh(double);
double  log(double);
double  log10(double);
double  frexp(double,int *);
double  pow(double,double);
double  fabs(double);
double  fmod(double,double);
double  ceil(double);
double  floor(double);
double  ldexp(double,int);
double	modf(double,double *);

/*   matherr    */
struct  _mexception
{
    int type;
    char  *name;
    double  arg1, arg2, retval;
};

int _matherr(struct _mexception *);
double __matherr(int type,char *name,double arg1,double arg2,double retval);

/* _mexception.type define */
#define _DOMAIN 1
#define _SING   2
#define _OVERFLOW 3
#define _UNDERFLOW 4

#endif /* _MATH_H */
