#! smake
#---------------------------------------------------------------------
#	Copyright (C) 1997, Nintendo.
#
#	File		Makefile
#	Coded    by	Yoshitaka Yasumoto.	Mar 19, 1997.
#	Modified by
#	Comments
#
#	$Id: Makefile,v 1.17 1999/04/02 10:10:55 yoshida Exp $
#---------------------------------------------------------------------

include $(ROOT)/usr/include/make/PRdefs

#FINAL = YES
ifeq ($(FINAL), YES)
OPTIMIZER       = -O2
LCDEFS          = -DNDEBUG -D_FINALROM
N64LIB          = -lgultra_rom
else
OPTIMIZER       = -g
LCDEFS          = -DDEBUG
N64LIB          = -lgultra_d
endif

APP =		sample.out

TARGETS =	sample.n64
CODEFILES =	system.c main.c uc_assert.c action.c sprite.c
CODEOBJECTS =	$(CODEFILES:.c=.o)
CODESEGMENT =	codesegment.o

STATICFILES =	static.c texture.c sprite_ball.c
STATICOBJECTS =	$(STATICFILES:.c=.o)
STATICSEGMENT =	staticsegment.o

BGFILES =	l2_tv.c
BGOBJECTS =	$(BGFILES:.c=.o)

DATAFILES =	system_cfb.c system_rspbuf.c
DATAOBJECTS =	$(DATAFILES:.c=.o)

CFILES =	$(CODEFILES) $(STATICFILES) $(DATAFILES)
HFILES =	action.h sprite.h system.h texture.h texture_pattern.h \
		uc_assert.h

OBJECTS =	$(CODESEGMENT) $(STATICSEGMENT) $(DATAOBJECTS) $(BGOBJECTS)

MAPFILE =	load.map

LCINCS  =	-I. -I$(ROOT)/usr/include/PR
LCOPTS =	-G 0

LCDEFS  +=	-DF3DEX_GBI

LDIRT   = $(APP) *.dat *.dbg *.lst *.sym  $(MAPFILE)
		    
LDFLAGS =	$(MKDEPOPT)  -L$(ROOT)/usr/lib $(N64LIB) -L$(GCCDIR)/mipse/lib -lkmc

default:	$(TARGETS)

include $(COMMONRULES)

$(CODESEGMENT):	$(CODEOBJECTS)
		$(LD) -o $(CODESEGMENT) -r $(CODEOBJECTS) $(LDFLAGS)

$(STATICSEGMENT): $(STATICOBJECTS)
		$(LD) -o $(STATICSEGMENT) -r $(STATICOBJECTS) $(LDFLAGS)

ifeq ($(FINAL), YES)
$(TARGETS):	spec $(OBJECTS) 
	$(MAKEROM) $(LCDEFS) -m -s 9 -r $(TARGETS) -e $(APP) spec > $(MAPFILE)
	makemask $(TARGETS)
else
$(TARGETS):	spec $(OBJECTS) 
	$(MAKEROM) $(LCDEFS) -m -r $(TARGETS) -e $(APP) spec > $(MAPFILE)
endif


# for exeGCC CELF
ifeq ($(GCC_CELF), ON)
ifneq ($(FINAL), YES)
CELFDIR = .
include $(CELFRULES)
$(CODEOBJECTS) $(DATAOBJECTS) :	$(CELFINFOFILES)
endif
endif
