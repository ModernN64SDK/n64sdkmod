#ifndef _SETJMP_H
#define _SETJMP_H

typedef unsigned long jmp_buf[16];

void longjmp(jmp_buf,int);
int setjmp(jmp_buf);

#endif
