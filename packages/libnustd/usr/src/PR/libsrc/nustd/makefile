#
# Copyright (C) 1997-1999 NINTENDO Co.,Ltd.
#
# $RCSfile: makefile.gcc,v $
# $Revision: 1.5 $
# $Date: 1999/05/28 09:51:20 $
#
# Makefile for nustd library
#

include /usr/include/n64/make/PRdefs

TOPDIR	= .

TARGETS = libnustd.a
LDIRT = libnustd.a libnustd_d.a


LIB	= /usr/lib/n64
LPR	= $(LIB)/PR
INC	= /usr/include/n64
AR	= mips-n64-ar

CDEFINES = -D_LANGUAGE_C -D_MIPS_SZLONG=32 -D_MIPS_SZINT=32

INCLUDES = $(INCLUDE) -I$(INC) -I$(INC)/PR -I./inc

WFLAGS	= -Wimplicit -Wreturn-type -Wunused -Wcomment

CFLAGS = -c -O -mno-shared -G 0 $(WFLAGS) $(COPTS) $(CDEFINES) $(INCLUDES)

SRCS		= \
		ctype.c \
		abs.c \
		labs.c \
		strchr.c \
		strrchr.c \
		strcpy.c \
		strncat.c \
		strncpy.c \
		strcmp.c \
		strcspn.c \
		strlen.c \
		strncmp.c \
		strpbrk.c \
		strspn.c \
		strcat.c \
		strstr.c \
		strtod.c \
		strtol.c \
		strtoul.c \
		strtok.c \
		atof.c \
		atol.c \
		initheap.c \
		malloc.c \
		mt_malloc.c \
		free.c \
		mt_free.c \
		realloc.c \
		mt_realloc.c \
		calloc.c \
		mt_calloc.c \
		memalign.c \
		mt_memalign.c \
		malloc_memcheck.c \
		mt_malloc_memcheck.c \
		malloc_memdsp.c \
		mt_malloc_memdsp.c \
		_malloc.c \
		_free.c \
		_malloc_memcheck.c \
		_malloc_memdsp.c \
		ldexp.c \
		ldexpf.c \
		frexp.c \
		frexpf.c \
		modf.c \
		modff.c \
		atoi.c \
		pow.c \
		powf.c \
		hypot.c \
		hypotf.c \
		srand.c \
		fmod.c \
		fmodf.c \
		qsort.c \
		mt_qsort.c \
		bsearch.c \
		memchr.c \
		memmove.c \
		memcmp.c \
		memset.c \
		memcpy.c \
		sin.c \
		cos.c \
		tan.c \
		acos.c \
		asin.c \
		atan.c \
		atan2.c \
		sinh.c \
		cosh.c \
		tanh.c \
		log.c \
		log10.c \
		exp.c \
		sinf.c \
		cosf.c \
		tanf.c \
		acosf.c \
		asinf.c \
		atanf.c \
		atan2f.c \
		sinhf.c \
		coshf.c \
		tanhf.c \
		logf.c \
		log10f.c \
		expf.c	\
		div.c	\
		ldiv.c

SRCS.S		= \
		fabs.s fabsf.s 	sqrt.s sqrtf.s		\
		ceil.s ceilf.s floor.s floorf.s		\
		setjmp.s

OBJECTS =	$(SRCS:.c=.o)  $(SRCS.S:.s=.o) 


INCLUDE	= 

default: $(TARGETS)

include $(COMMONRULES)

#.c.o:
#	$(CC) -c -O -G 0 $(WFLAGS) $(COPTS) $(CDEFINES) $(INCLUDES) $<

#.s.o:
#	$(CC) -c -O -G 0 $(WFLAGS) $(COPTS) $(CDEFINES) $(INCLUDES) $<

.s.o:
	$(CC) $(CFLAGS) $<

help:
	@echo make help ...... this HELP
	@echo make deb ....... make _DEBUGis settled and compiled
	@echo mmake clean ..... delete '.o' and $(LDIRT)
	@echo make clobber ... delete '.o', $(LDIRT) and '.n64'

deb: clobber
	$(MAKE) TARGETS=libnustd_d.a COPTS='-g -D_DEBUG_LIB -O'

$(TARGETS):	$(OBJECTS) makefile
	$(AR) r $(TARGETS) $(OBJECTS)
	ranlib $(TARGETS)

install: $(TARGETS)
	copy $(TARGETS) ..\lib

install-deb: deb
	$(MAKE) TARGETS=libnustd_d.a install

ctype.o: ctype.h
abs.o: stdlib.h
labs.o: stdlib.h
strchr.o: string.h
strrchr.o: string.h
strcpy.o: string.h
strncat.o: string.h
strncpy.o: string.h
strcmp.o: string.h
strcspn.o: string.h
strlen.o: string.h
strncmp.o: string.h
strpbrk.o: string.h
strspn.o: string.h
strcat.o: string.h
strstr.o: string.h
strtod.o: string.h math.h mathdef.h ctype.h
strtol.o: stdlib.h math.h mathdef.h ctype.h string.h
strtoul.o: math.h mathdef.h stdlib.h ctype.h string.h
strtok.o: string.h
atof.o: stdlib.h math.h mathdef.h ctype.h
atol.o: stdlib.h ctype.h
initheap.o: string.h malloc.h _malloc.h
malloc.o: string.h malloc.h _malloc.h
mt_malloc.o: string.h
mt_malloc.o: malloc.h _malloc.h
free.o: malloc.h _malloc.h string.h
mt_free.o: string.h malloc.h _malloc.h
realloc.o: string.h malloc.h _malloc.h
mt_realloc.o: string.h malloc.h _malloc.h
calloc.o: string.h malloc.h _malloc.h
mt_calloc.o: malloc.h _malloc.h string.h
memalign.o: string.h malloc.h _malloc.h
mt_memalign.o: string.h malloc.h _malloc.h
malloc_memcheck.o: string.h malloc.h _malloc.h
mt_malloc_memcheck.o: string.h malloc.h _malloc.h
malloc_memdsp.o: string.h malloc.h _malloc.h
mt_malloc_memdsp.o: string.h malloc.h _malloc.h
_malloc.o: string.h _malloc.h malloc.h
_free.o: string.h _malloc.h malloc.h
_malloc_memcheck.o: string.h malloc.h _malloc.h
_malloc_memdsp.o: string.h malloc.h _malloc.h
ldexp.o: math.h mathdef.h
ldexpf.o: math.h mathdef.h
frexp.o: math.h mathdef.h
frexpf.o: math.h mathdef.h
atoi.o: stdlib.h ctype.h
pow.o: math.h mathdef.h
powf.o: math.h mathdef.h
hypot.o: math.h mathdef.h
hypotf.o: math.h mathdef.h
srand.o: stdlib.h
qsort.o: stdlib.h
mt_qsort.o: stdlib.h
bsearch.o: stdlib.h
memchr.o: string.h
memmove.o: string.h
memcmp.o: string.h
memset.o: string.h
memcpy.o: string.h
sin.o: math.h mathdef.h
cos.o: math.h mathdef.h
tan.o: math.h mathdef.h
acos.o: math.h mathdef.h
asin.o: math.h mathdef.h
atan.o: math.h mathdef.h
atan2.o: math.h mathdef.h
sinh.o: math.h mathdef.h
cosh.o: math.h mathdef.h
tanh.o: math.h mathdef.h
log.o: math.h mathdef.h
log10.o: math.h mathdef.h
exp.o: math.h mathdef.h
sinf.o: math.h mathdef.h
cosf.o: math.h mathdef.h
tanf.o: math.h mathdef.h
acosf.o: math.h mathdef.h
asinf.o: math.h mathdef.h
atanf.o: math.h mathdef.h
atan2f.o: math.h mathdef.h
sinhf.o: math.h mathdef.h
coshf.o: math.h mathdef.h
tanhf.o: math.h mathdef.h
logf.o: math.h mathdef.h
log10f.o: math.h mathdef.h
expf.o: math.h mathdef.h
div.o: stdlib.h
ldiv.o: stdlib.h
