#ifndef _STDIO_H
#define _STDIO_H
/*
	stdio.h
*/

#ifndef NULL
#define NULL		0
#endif

#ifndef EOF
#define	EOF			(-1)
#endif

#define BUFSIZ  512                     /* STDIO Buffer size */

int	putchar(int c);
int	getchar(void);
int	puts(char *);
char	*gets(char *s);
int	_putw(int);
int	_getw();
int	printf(const char *, ...);
int	sprintf(char *, const char *, ...);
int	sprintf(char *, const char *, ...);
int	sscanf(const char *,const char *, ...);

#endif
