/*---------------------------------------------------------------------*
        Copyright (C) 1997-1999 NINTENDO Co.,Ltd.
        
        $RCSfile: stdlib.h,v $
        $Revision: 1.4 $
        $Date: 1999/05/28 08:14:06 $
 *---------------------------------------------------------------------*/
#ifndef __STDLIB_H__
#define __STDLIB_H__

#ifdef _LANGUAGE_C_PLUS_PLUS
extern "C" {
#endif

#ifndef NULL
#define NULL            0
#endif

/* Maximum value returned by rand */
#define RAND_MAX	0x7fff

#if !defined(_SIZE_T) && !defined(_SIZE_T_) && !defined(_SIZE_T_DEF)
#define _SIZE_T
#define _SIZE_T_DEF			/* exeGCC size_t define label */
#if (_MIPS_SZLONG == 32)
typedef unsigned int    size_t;
#endif
#if (_MIPS_SZLONG == 64)
typedef unsigned long   size_t;
#endif
#endif

typedef struct _div_t {
    int quot;
    int rem;
} div_t;

typedef struct _ldiv_t {
    long quot;
    long rem;
} ldiv_t;

double atof(const char *string);
int atoi(const char *s);
long atol(const char *string);

long strtol(const char *string, char **endptr, int base);
double strtod(const char *string, char **endptr);
unsigned long strtoul(const char *string, char **endptr, int base);

void qsort(void *base, size_t num, size_t width, int (*compare)(const void *, const void *));
void mt_qsort(void *base, size_t num, size_t width, int (*compare)(const void *, const void *));
void *bsearch(const void *key, const void *base, size_t num, size_t width, int (*compare)(const void *, const void *));

long    labs(long n);
int abs(int n);

void srand(unsigned seed);
int rand(void);

div_t div(int numer, int denom);
ldiv_t _nldiv(long int numer, long int denom);

/* Included in ultra, but there are no prototypes in ultra, so the declarations are left here. */
ldiv_t ldiv(long int numer, long int denom);

#ifdef _LANGUAGE_C_PLUS_PLUS
}
#endif
#endif
