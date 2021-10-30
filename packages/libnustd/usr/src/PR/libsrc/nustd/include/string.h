/*---------------------------------------------------------------------*
        Copyright (C) 1997-1999 NINTENDO Co.,Ltd.
        
        $RCSfile: string.h,v $
        $Revision: 1.3 $
        $Date: 1999/05/28 08:14:06 $
 *---------------------------------------------------------------------*/
#ifndef __STRING_H__
#define __STRING_H__

#ifdef _LANGUAGE_C_PLUS_PLUS
extern "C" {
#endif

#ifndef NULL
#define NULL            0
#endif

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

void *memchr(const void *buf, int c, size_t count);
void *memmove(void *dest, const void *src, size_t count);
int memcmp(const void *buf1, const void *buf2, size_t count);
void    *memset(void* dst, int c, size_t count);
void *_nmemcpy(void *dest, const void *src, size_t count);

char *_nstrchr(const char *string, int c);
char *strcpy(char *str1, const char *str2);
char *strncat(char *str1, const char *str2, size_t n);
char *strncpy(char *str1, const char *str2, size_t  n);
char *strrchr(const char *str, int c);
int strcmp(const char *str1, const char *str2);
size_t  strcspn(const char *string1, const char *string2);
size_t  _nstrlen(const char *string);
int strncmp(const char *str1, const char *str2, int n);
char    *strpbrk(const char *string1, const char *string2);
size_t  strspn(const char *string1, const char *string2);
char  *strcat(char *str1, const char *str2);
char *strstr(const char *string1, const char *string2);
char *strtok(char *string1, const char *string2);

/* Included in ultra, but there are no prototypes in ultra, so the declarations are left here.  */
void *memcpy(void *dest, const void *src, size_t count);
size_t strlen(const char *string);
char *strchr(const char *string, int c);

#ifdef _LANGUAGE_C_PLUS_PLUS
}
#endif
#endif
