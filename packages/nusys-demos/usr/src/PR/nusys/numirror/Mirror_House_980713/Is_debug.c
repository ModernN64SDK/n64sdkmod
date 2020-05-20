
#include <ultra64.h>
#include <stdio.h>
#include <ctype.h>
#include <stdarg.h>

/*
   Intelligent Systems Co.,Ltd.

   _IS_MSGBUFFER_AD and _IS_MSGBUFFER_LEN are the communication buffer
   address/buffer for debug print. If position related problems 
   occur, data can be moved in 4K units.
   It is not permitted to extend over the 64K segment.
   (For example, from 0xb1f88000 to 0xb1f98000)
   The settings for the host side need to be set according to the
   corresponding address when the above address is modified. There is
   no guarantee of operation if these conditions are not met.
   Also please do not modify any default data other than 
   _IS_MSGBUFFER_AD and _IS_MSGBUFFER_LEN.
   
   Default Address:  _IS_MSGBUFFER_AD  = 0xb1ff0000
   Default Size   :  _IS_MSGBUFFER_LEN = 0x10000
*/
#define _IS_MSGBUFFER_AD	0xb1ff0000		/* 32Mbyte RAM bottom 	*/
#define _IS_MSGBUFFER_LEN	0x10000			/* bbuffer length 	*/



#define _IS_MSGBUF_HEADLEN	0x20
#define _IS_MSGBUF_CHKAD	(_IS_MSGBUFFER_AD+0x00)		/* V810->N64  "IS64"		*/
#define _IS_MSGBUF_GETPT	(_IS_MSGBUFFER_AD+0x04)		/* V810->N64  get pt		*/
#define _IS_MSGBUF_PUTPT	(_IS_MSGBUFFER_AD+0x14)		/* N64->V810  put pt		*/
#define _IS_MSGBUF_MSGTOP	(_IS_MSGBUFFER_AD+_IS_MSGBUF_HEADLEN)

#define _IS_MSGBUF_CHK_CODE	(('I'<<24)+('S'<<16)+('6'<<8)+'4')

#if 1	/* normal 1 */
#define _is_WriteIo osPiWriteIo
#define _is_ReadIo osPiReadIo
#else	/* isPrintf debug */
#define _IS_MSGBUFFER_AD	0x800f0000
#define _is_WriteIo TestWrite
#define _is_ReadIo TestRead
TestRead(int ad, int *dt)
{ *dt = *(int *)ad; }
TestWrite(int ad, int dt)
{ *(int *)ad	= dt; }
#endif


void isPrintfInit(void)
{
	_is_WriteIo(_IS_MSGBUF_PUTPT, 0);
	_is_WriteIo(_IS_MSGBUF_GETPT, 0);
	_is_WriteIo(_IS_MSGBUF_CHKAD, _IS_MSGBUF_CHK_CODE);
}

static void *_is_printf_s(void *, const char *, int);

extern void _Printf(void (*)(void *), int, const char *, va_list);

void isPrintf(const char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    _Printf((void(*)(void *))_is_printf_s, 0, fmt, ap);
    va_end(ap);
}
void osSyncPrintf(const char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    _Printf((void(*)(void *))_is_printf_s, 0, fmt, ap);
    va_end(ap);
}
void rmonPrintf(const char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    _Printf((void(*)(void *))_is_printf_s, 0, fmt, ap);
    va_end(ap);
}

static void *_is_printf_s(void *str, register const char *buf, register int n)
{
	u32				dt, dt2;
	register int	putad, getad, newad;
	register int	sft, ad;

	_is_ReadIo(_IS_MSGBUF_CHKAD, &dt);
	if (dt != _IS_MSGBUF_CHK_CODE) return((void *)1);

 	_is_ReadIo(_IS_MSGBUF_GETPT, &dt); getad = dt;
	_is_ReadIo(_IS_MSGBUF_PUTPT, &dt); putad = dt;
	newad = putad + n;
	if (newad >= (_IS_MSGBUFFER_LEN - _IS_MSGBUF_HEADLEN)) {
		newad -= (_IS_MSGBUFFER_LEN - _IS_MSGBUF_HEADLEN);
		if ((getad < (newad)) || (getad > putad)) return((void *)1);
	} else {
		if ((getad > putad) && (getad < (newad))) return((void *)1);
	}

    while(n) {
		if (*buf) {
			sft = (3 - (putad & 0x03)) << 3;
			ad = _IS_MSGBUF_MSGTOP+(putad & 0x0ffffffc);
			_is_ReadIo(ad, &dt);
			_is_WriteIo(ad, (dt & (~(0xff << sft)))|(*buf << sft));
			putad++;
			if (putad >= (_IS_MSGBUFFER_LEN - _IS_MSGBUF_HEADLEN)) putad -= (_IS_MSGBUFFER_LEN - _IS_MSGBUF_HEADLEN);
		}
		buf++;
		n--;
    }
	_is_WriteIo(_IS_MSGBUF_PUTPT, putad);
    return((void *)1);
}

