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
OPTIMIZER       = -O2 -std=gnu90
LCDEFS          = -DNDEBUG -D_FINALROM
N64LIB          = -lultra_rom
else
OPTIMIZER       = -g -std=gnu90
LCDEFS          = -DDEBUG
N64LIB          = -lultra_d
endif

APP =		sample.out

TARGETS =	sample.n64
CODEFILES =	system.c main.c uc_assert.c action.c sprite.c
CODEOBJECTS =	$(CODEFILES:.c=.o)
CODESEGMENT =	codesegment.o

STATICFILES =	static.c texture.c sprite_ball.c
STATICOBJECTS =	$(STATICFILES:.c=.o)
STATICSEGMENT =	staticsegment.o

DATAFILES =	system_cfb.c system_rspbuf.c
DATAOBJECTS =	$(DATAFILES:.c=.o)

BGFILES =	L2_TV.c
BGOBJECTS =	$(BGFILES:.c=.o)

CFILES =	$(CODEFILES) $(STATICFILES) $(DATAFILES) $(BGFILES)
HFILES =	action.h sprite.h system.h texture.h texture_pattern.h \
		uc_assert.h

OBJECTS =	$(CODESEGMENT) $(STATICSEGMENT) $(DATAOBJECTS) $(BGOBJECTS)

LCINCS  =	-I. -I$(ROOT)/usr/include/PR
LCOPTS =	-G 0

LCDEFS  +=	-DF3DEX_GBI

LDIRT   = $(APP) *.dat *.dbg *.lst *.sym 
		    
LDFLAGS =	$(MKDEPOPT)  -L$(ROOT)/usr/lib $(N64LIB) -L$(N64_LIBGCCDIR) -lgcc

default:	$(TARGETS)

include $(COMMONRULES)

$(CODESEGMENT):	$(CODEOBJECTS)
		$(LD) -o $(CODESEGMENT) -r $(CODEOBJECTS) $(LDFLAGS)

$(STATICSEGMENT): $(STATICOBJECTS)
		$(LD) -o $(STATICSEGMENT) -r $(STATICOBJECTS) $(LDFLAGS)

ifeq ($(FINAL), YES)
$(TARGETS):	spec $(OBJECTS) 
	$(MAKEROM) $(LCDEFS) -m -s 9 -r $(TARGETS) -e $(APP) spec 
	makemask $(TARGETS)
else
$(TARGETS):	spec $(OBJECTS) 
	$(MAKEROM) $(LCDEFS) -m -r $(TARGETS) -e $(APP) spec 
endif

