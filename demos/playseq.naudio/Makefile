#!smake
# --------------------------------------------------------------------
#        Copyright (C) 1997,1998 Nintendo. (Originated by SGI)
#
#        $RCSfile: Makefile,v $
#        $Revision: 1.14 $
#        $Date: 1999/07/13 05:38:45 $
# --------------------------------------------------------------------

#DBG_FLAG  =  -D_NON_DEBUG
include $(ROOT)/usr/include/make/PRdefs

#FINAL = YES
ifeq ($(FINAL), YES)
OPTIMIZER       = -O2
LCDEFS          = -DNDEBUG -D_FINALROM
N64LIB          = -lgultra_rom
NAUDIOLIB	= -lgn_audio
else
OPTIMIZER       = -g
LCDEFS          = -DDEBUG
N64LIB          = -lgultra_d
NAUDIOLIB	= -lgn_audio_d
endif

APP =		playseq.out
TARGETS =	playseq.n64

INSTFILES =	sampleBank.inst

BANKFILES =	$(INSTFILES:.inst=.ctl)
TABLEFILES =	$(INSTFILES:.inst=.tbl)
SYMFILES =	$(INSTFILES:.inst=.sym)

HFILES =	playseq.h

CODEFILES =	playseq.c
CODEOBJECTS =	$(CODEFILES:.c=.o)

CODESEGMENT =	codesegment.o

OBJECTS =	$(CODESEGMENT)

LCINCS =	-I.\
		-I$(ROOT)/usr/include\
		-I$(ROOT)/usr/include/PR
LCOPTS =	-G 0
LDFLAGS =	$(MKDEPOPT) \
		-L$(ROOT)/usr/lib $(NAUDIOLIB) $(N64LIB) -L$(GCCDIR)/mipse/lib -lkmc

LDIRT = $(APP) $(TARGETS)

default:	$(TARGETS)

include $(COMMONRULES)

.SUFFIXES: .inst .ctl .tbl

.inst.ctl:
	$(IC) -o $* $<

.inst.tbl:	# Dummy target for tables
	@echo ''

$(CODESEGMENT):	$(CODEOBJECTS)
		$(LD) -o $(CODESEGMENT) -r $(CODEOBJECTS) $(LDFLAGS)

ifeq ($(FINAL), YES)
$(TARGETS) $(APP):      spec $(OBJECTS)  $(BANKFILES) $(TABLEFILES)
	$(MAKEROM) -s 9 -r $(TARGETS) -e $(APP) spec
	makemask $(TARGETS)
else
$(TARGETS) $(APP):      spec $(OBJECTS)  $(BANKFILES) $(TABLEFILES)
	$(MAKEROM) -r $(TARGETS) -e $(APP) spec
endif

# for exeGCC CELF
ifeq ($(GCC_CELF), ON)
ifneq ($(FINAL), YES)
CELFDIR = .
include $(CELFRULES)
$(CODEOBJECTS) $(DATAOBJECTS) :	$(CELFINFOFILES)
endif
endif
