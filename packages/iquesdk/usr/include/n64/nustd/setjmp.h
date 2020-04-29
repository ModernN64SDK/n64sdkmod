/*---------------------------------------------------------------------*
        Copyright (C) 1997-1999 NINTENDO Co.,Ltd.
        
        $RCSfile: setjmp.h,v $
        $Revision: 1.1.1.1 $
        $Date: 2002/10/30 02:07:09 $
 *---------------------------------------------------------------------*/
#ifndef _SETJMP_H_
#define _SETJMP_H_

#ifdef _LANGUAGE_C_PLUS_PLUS
extern "C" {
#endif

#define _JBLEN                      11

#ifndef _JMP_BUF_DEFINED
typedef  int  jmp_buf[_JBLEN];
#endif

int setjmp(jmp_buf env);
void longjmp(jmp_buf env, int value);

#ifdef _LANGUAGE_C_PLUS_PLUS
}
#endif
#endif
