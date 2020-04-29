#ifndef _CTYPE_H
#define _CTYPE_H
/*
	ctype.h
*/

#define _ISSP	1	/* 0x9-0xd , 0x20 */
#define _ISUP	2	/* 'A'-'Z' */
#define _ISLO	4	/* 'a'-'z' */
#define _ISDI	8	/* '0'-'9' */
#define _ISHE	16	/* 'a'-'f' , 'A'-'F' */
#define _ISCT	32	/* 0x0-0x1f , 0x7f */
#define _ISPU	64	/* punctuation */
#define _ISKAN	128	/* SJIS 1st BYTE */

extern char _ctype[];

#define isalnum(c)	(_ctype[(unsigned char)c] & (_ISDI | _ISUP | _ISLO))
#define isalpha(c)	(_ctype[(unsigned char)c] & (_ISUP | _ISLO))
#define isascii(c)	((unsigned)(c) < 128)
#define iscntrl(c)	(_ctype[(unsigned char)c] & _ISCT)
#define isdigit(c)	(_ctype[(unsigned char)c] & _ISDI)
#define isgraph(c)	((c) >= 0x21 && (c) <= 0x7e)
#define islower(c)	(_ctype[(unsigned char)c] & _ISLO)
#define isprint(c)	((c) >= 0x20 && (c) <= 0x7e)
#define ispunct(c)	(_ctype[(unsigned char)c] & _ISPU)
#define isspace(c)	(_ctype[(unsigned char)c] & _ISSP)
#define isupper(c)	(_ctype[(unsigned char)c] & _ISUP)
#define isxdigit(c)	(_ctype[(unsigned char)c] & (_ISDI | _ISHE))
#define iskanji(c)	(_ctype[(unsigned char)c] & _ISKAN)

#define _toupper(c)	((c) + 'A' - 'a')
#define _tolower(c)	((c) + 'a' - 'A')
#define toascii(c)	((c) & 0x7f)

int tolower(int c);
int toupper(int c);

#endif
