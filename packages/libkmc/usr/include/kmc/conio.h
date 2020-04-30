#ifndef _CONIO_H
#define _CONIO_H
/*
	conio.h
*/

void _pt_dsp(int c);
int _pt_key(void);
void _pt_puts(char *);

int kbhit(void);
int getch(void);
int getche(void);
int putch(int c);
int ungetch(int c);
char *cgets(char *s);
int cputs(char *s);
int cprintf(char *f,...);
int cscanf(char *,...);

#endif
