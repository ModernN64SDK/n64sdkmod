#ifndef _EXPLOG_H
#define _EXPLOG_H
/*
	explog.h
*/

/***** GCC ****/
#define GCC		1
#define XLONG		long long
#define MBIT		0x1000000000000000LL

#define VALID_CT	19
#define FBUF_SIZE	40
#define ABUFSIZE	256

#define EXP_INDEX	0
#define SIG_INDEX	0

#define KL2B		1.44269504088896341	/* 1/log(2) */
#define KL2		0.693147180559945309 	/* log(2) */
#define	KL10E		0.434294481903251827651	/* log10(e) */

typedef union {
    double df;
    unsigned long ui[2];
    unsigned char ub[8];
    unsigned long long ull;
} DFLOAT;

double __matherr(int,char *,double,double,double);

#endif
