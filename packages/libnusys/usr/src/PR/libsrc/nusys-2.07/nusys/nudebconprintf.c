/*======================================================================*/
/*		NuSYS							*/
/*		nudebconprintf.c					*/
/*									*/
/*		Copyright (C) 1997, NINTENDO Co,Ltd.			*/
/*									*/
/*----------------------------------------------------------------------*/
/* Ver 1.2	98/06/15	Created by Kensaku Ohki(SLANP)		*/
/*======================================================================*/
#include <nusys.h>
#include <stdarg.h>


#define FLAG_PLUS	0x0001
#define FLAG_LEFT	0x0002
#define FLAG_SPACE	0x0004
#define	FLAG_SPECIAL	0x0008
#define FLAG_ZEROPAD	0x0010
#define FLAG_LARGE	0x0020
#define FLAG_SIGN	0x0040
#define FLAG_FLOAT	0x0100
#define FLAG_FLOAT_F	0x0200
#define FLAG_FLOAT_E	0x0400
#define FLAG_FLOAT_G	0x0800

#define	QUALIFIER_INT		0x00
#define	QUALIFIER_SHORT		0x01
#define	QUALIFIER_LONG		0x02
#define	QUALIFIER_LONGLONG	0x03
#define	QUALIFIER_DOUBLE	0x04


#define DBL_EXP_BIAS		1023
#define DBL_EXP_MAX		1023
#define DBL_EXP_MIN		-1022
#define	DBL_ZERO_EXP		(DBL_EXP_MIN - 1)
#define	DBL_ZERO_MANTISSA	0
#define	DBL_NaN_EXP		(DBL_EXP_MAX + 1)


#define IS_DIGIT(c)	('0' <= (c) && (c) <= '9')

char* digitTxt[] = {
    "0123456789ABCDEFX",
    "0123456789abcdefx"
};

u8 NaNTxt[] = "<NaN>";
u8 infinityTxt[] = "<Inf>";

/*----------------------------------------------------------------------*/
/*	getatoi								*/
/*----------------------------------------------------------------------*/
static int getatoi(const char** str )
{
    int num = 0;

    while(IS_DIGIT(**str)){
	num = num * 10 + **str - '0';
	(*str)++;
    }
    return num;
}



/*----------------------------------------------------------------------*/
/*	outputNumber							*/
/*----------------------------------------------------------------------*/
void outitoasc(s32 wndNo, long long num, int base, int size, int precision, int type)
{
    char*	digit;
    char	pad;
    char	sign;
    s32 	cnt;
    char	buf[66];
    
    if(type & FLAG_LARGE){
	digit = digitTxt[0];
//	digit = "0123456789ABCDEFX";
    } else {
//	digit = "0123456789abcdefx";
	digit = digitTxt[1];
    }
    
    if(type & FLAG_LEFT){
	type &= ~FLAG_ZEROPAD;
    }
    pad =  (type & FLAG_ZEROPAD) ? '0' : ' ';
    sign = 0;
    if(type & FLAG_SIGN){
	if(num < 0){
	    sign = '-';
	    num = -num;
	    size--;
	} else if(type & FLAG_PLUS){
	    sign = '+';
	    size--;
	} else if(type & FLAG_SPACE){
	    sign = ' ';
	    size--;
	}
	
    }
    

    

    cnt = 0;
    if((unsigned long long)num == 0L){
        buf[cnt++] = 0;
    } else {
        while(num !=0){
            buf[cnt++] = (unsigned long long)num % (unsigned)base;
            num = (unsigned long long)num / (unsigned)base;
        }
    }

    
    if( cnt > precision){
	precision = cnt;
    }
    size -= precision;
    if(!(type & (FLAG_ZEROPAD | FLAG_LEFT))){
	while(size-- > 0){
	    nuDebConPutc(wndNo, ' ');
	}
    }
    if(sign){
	nuDebConPutc(wndNo, sign);
    }
    if(type & FLAG_SPECIAL){
	if(base == 8){
	    nuDebConPutc(wndNo, '0');
	} else if(base == 16){
	    nuDebConPutc(wndNo, '0');
	    nuDebConPutc(wndNo, digit[0x10]);
	}
    }
    if(!(type & FLAG_LEFT)){
	while(size-- > 0){
	    nuDebConPutc(wndNo, pad);
	}
    }
    while(cnt < precision--){
	nuDebConPutc(wndNo, '0');
    }
    while(cnt-- > 0){
	nuDebConPutc(wndNo, digit[buf[cnt]]);
    }
    while(size-- >0){
	nuDebConPutc(wndNo, ' ');
    }
    return;
}

/*----------------------------------------------------------------------*/
/*	getintval							*/
/*----------------------------------------------------------------------*/
long long getintval(va_list* ap, s32 qualifier, u32 flags)
{
    long long 	llval;
    
    switch(qualifier){
    case QUALIFIER_SHORT:
	if(flags & FLAG_SIGN){
	    llval = va_arg(*ap, short);
	} else {
	    llval = va_arg(*ap, unsigned short);
	}
	break;
    case QUALIFIER_LONG:
	if(flags & FLAG_SIGN){
	    llval = va_arg(*ap, long);
	} else {
	    llval = va_arg(*ap, unsigned long);

	}
	break;
    case QUALIFIER_LONGLONG:
	if(flags & FLAG_SIGN){
	    llval = va_arg(*ap, long long);
	} else {
	    llval = va_arg(*ap, unsigned long long);
	}
	break;
    default:
	if(flags & FLAG_SIGN){
	    llval = va_arg(*ap, int);
	} else {
	    llval = va_arg(*ap, unsigned int);	    
	}
	break;
    }

    return llval;
    
}
#if 0 
void outfloattoasc(s32 wndNo, double dval, s32 size, s32 precision, u32 type)
{
    char*	digit;    
    char	buf[70];
    u8		dbuf[0x10];
    u64*	dvimg;
    double*	dptr;
    char	sign;
    char	pad;
    s32		exp;
    u64		mantissa;
    u64		tmp64;
    u8		fflag;
    s32		cnt;
    s32		cntint;
    s32		cntfloat;
    

    dvimg = (u64*)(((u32)dbuf + 7) & (~0x07));
    dptr = (double*)dvimg;
    *dptr = dval;

    exp = ((*dvimg >> 52) &  0x7FF) - DBL_EXP_BIAS;
    mantissa = *dvimg & 0x000FFFFFFFFFFFFFLL;

    if(precision < 0){
	precision = 6;
    }

    if(type & FLAG_LARGE){
	digit = digitTxt[0];
    } else {
	digit = digitTxt[1];
    }
    if(type & FLAG_LEFT){
	type &= ~FLAG_ZEROPAD;
    }
    pad =  (type & FLAG_ZEROPAD) ? '0' : ' ';
    sign = *((u8*)dvimg) >> 7;


    if(*((u8*)dvimg) & 0x7f){
	sign = '-';
	size--;
	*((u8*)dvimg) &= 0x7f;
    } else if(type & FLAG_PLUS){
	sign = '+';
	size--;
    } else if(type & FLAG_SPACE){
	sign = ' ';
	size--;
    }

    
    cntint = 0;
    cntfloat = 0;
    if(exp == DBL_ZERO_EXP){
	if(mantissa == (u64)DBL_ZERO_MANTISSA){
	    buf[cntint++] = '0';	   	/* Zero */
	} else {
	    /* Unnormalized number.*/
	    double dtmp;
	    double dexp;
	    int exp10;
	    
	    dtmp = *dptr;
	    dexp = 1.0;
	    for(exp10 = 0; dtmp > 10.0; exp10++){
		dtmp /= 10.0;
		dexp *= 10.0;
	    }
	    dtmp = *dptr;
	    while(exp10--){
		buf[cntint++] = (u8)dtmp/dexp;
		dexp /=10.0;
	    }
	}
    } else if(exp == DBL_NaN_EXP){
	u8* txt;
	if(mantissa){
	    txt = NaNTxt;		/* Nonnumeric character (NaN)	*/
	} else {
	    txt = infinityTxt;		/* Infinite number */
	}
	while(*txt != '\0'){
	    buf[cnt++] = *(txt++);
	}
    } else {
	double dtmp;
	double dexp;
	int exp10;

	dtmp = *dptr;
	dexp = 1.0;
	for(exp10 = 1; dtmp > 10.0; exp10++){
	    dtmp /= 10.0;
	    dexp *= 10.0;
	}
	dtmp = *dptr;
	while(exp10-- > 0){

	    buf[cntint] = dtmp/dexp;
	    dtmp -= buf[cntint] * dexp;
	    dexp /=10.0;
	    cntint++;
	    if(dexp == 0.0)break;
	}
	dtmp = *dptr;
    }
}
#endif    
/*----------------------------------------------------------------------*/
/*	nuDebConPrintf - Output formatted character string.               */
/* 	Outputs a formatted character string to the console.             */
/*	IN:	windowNo	Window number for display. 		*/
/*		fmt		Formatted character string. 		*/
/*		[argument]	Argument corresponding to format specification. */
/*	RET:	None							*/
/*----------------------------------------------------------------------*/
void nuDebConPrintf(u32 wndNo, const char* fmt, ...)
{

    va_list	ap;
    char*	sval;
    s32		ival;
//    s32		base;
    s32		precision;
    s32		qualifier;
    u32		flags;
    s32		width;
    s32		len;
    long long	num;
//    double	dval;
    
    va_start(ap, fmt);
    
    for(; *fmt; fmt++){
	if(*fmt != '%'){
	    nuDebConPutc(wndNo, *fmt);
	    continue;
	}

	/* Get the assignment inhibit characters.	*/
	flags = 0;
	while(1){
	    switch(*++fmt){
	    case '-': flags |= FLAG_LEFT;    continue;
	    case '+': flags |= FLAG_PLUS;    continue;
	    case ' ': flags |= FLAG_SPACE;   continue;
	    case '#': flags |= FLAG_SPECIAL; continue;
	    case '0': flags |= FLAG_ZEROPAD; continue;
	    }
	    break;
	}


	/* Get the maximum field width.	*/
	width = -1;
	if(IS_DIGIT(*fmt)){
	    width = getatoi(&fmt);
	} else if(*fmt == '*'){
	    width = va_arg(ap, int);
	    if(width < 0){
		width = -width;
		flags |= FLAG_LEFT;
	    }
	}

	/* Get the precision specification	*/
	precision = -1;
	if(*fmt == '.'){
	    fmt++;
	    if(IS_DIGIT(*fmt)){
		precision = getatoi(&fmt);
	    } else if(*fmt == '*'){
		++fmt;
		precision = va_arg(ap, int);
	    }
	    if(precision < 0){
		precision = 0;
	    }
	    
	}

	/* Get the conversion qualifier */
	qualifier = QUALIFIER_INT;
	
	switch(*fmt){
	case 'h':
	    qualifier = QUALIFIER_SHORT;
	    fmt++;
	    break;
	case 'l':
	    qualifier = QUALIFIER_LONG;
	    fmt++;
	    if(*fmt == 'l'){
		qualifier = QUALIFIER_LONGLONG;
		fmt++;
	    }
	    break;
	case 'L':	/* Long double not supported. double as assumed.  */
	    qualifier = QUALIFIER_DOUBLE;
	    fmt++;
	    break;
	}


	
	/* Get the conversion specifier	*/
	switch(*fmt){
	case 'd':	/* singed decimal */
	case 'i':	/* singed decimal */
	    flags |= FLAG_SIGN;
	case 'u':	/* unsigned decima */
//	    base = 10;
	    num = getintval(&ap, qualifier, flags);
	    outitoasc(wndNo, num, 10, width, precision, flags);
	    break;
	    
	case 'o':	/* unsigned octal */
	    num = getintval(&ap, qualifier, flags);
	    outitoasc(wndNo, num, 8, width, precision, flags);
	    break;
	    
	case 'X':	/* unsigned hex	& large */
	    flags |= FLAG_LARGE;
	case 'x':	/* unsigned hex	& small */
	    num = getintval(&ap, qualifier, flags);
	    outitoasc(wndNo, num, 16, width, precision, flags);
	    break;
#if 0	    
	case 'f':	/* [-]mmm.dddd... 		*/
	    flags |= FLAG_FLOAT_F;
	    dval = va_arg(ap, double);
	    outfloattoasc(wndNo, dval, width, precision, flags);
	    break;
	case 'E':	/* [-]d.ddd...E[+/-]dddd 	*/
	    flags |= FLAG_LARGE;
	case 'e':	/* [-]d.ddd...e[+/-]dddd 	*/
	    flags |= FLAG_FLOAT_E;
	    dval = va_arg(ap, double);
	    break;
	    
	case 'G':	/* %E or %f			*/
	    flags |= FLAG_LARGE;
	case 'g':	/* %e or %f			*/
	    dval = va_arg(ap, double);
	    flags |= FLAG_FLOAT_G;
	    break;
#endif	    
	case 'c':	/* string */
	    if(!(flags & FLAG_LEFT)){
		while(--width > 0){
		    nuDebConPutc(wndNo, ' ');
		}
	    }
	    ival = va_arg(ap, int);
	    nuDebConPutc(wndNo, ival);
	    while(--width >0){
		nuDebConPutc(wndNo, ' ');
	    }
	    break;
	case 's':	/* strings */
	    sval = va_arg(ap, char *);
	    if(!sval){
		sval = "<NULL>";
	    }
	    
	    for(len = 0; (sval[len] != '\0') && (precision - len) ; len++)
		;
	    if(!(flags & FLAG_LEFT)){
		while(len < width--){
		    nuDebConPutc(wndNo, ' ');
		}
	    }
	    nuDebConCPuts(wndNo, sval);
	    while(len < width--){
		nuDebConPutc(wndNo, ' ');
	    }
	    break;
	    
	case 'p':	/* pointer */
	    if(width == -1){
		width = sizeof(void *);
		flags |= FLAG_ZEROPAD;
	    }
	    outitoasc(wndNo, (unsigned long long)(unsigned int) va_arg(ap, void *), 16,
		      width, precision, flags);
	    break;
	    
	case 'n':	/* invalid  */
	    break;
	case '%':	/* output '%' */
	    nuDebConPutc(wndNo, '%');
	    break;
	default:
	    if(!*fmt){
		--fmt;
	    }
	    break;
	}

    }
}
