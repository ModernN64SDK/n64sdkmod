#!smake -k
# --------------------------------------------------------------------
#        Copyright (C) 1997,1998 Nintendo. (Originated by SGI)
#        
#        $RCSfile: Makefile.pc,v $
#        $Revision: 1.3 $
#        $Date: 1999/01/08 07:35:01 $
# --------------------------------------------------------------------
#
# Makefile for nosPak test application 
#
#

include $(ROOT)/usr/include/make/PRdefs

LANG_JAPANESE = 

# for LANG_JAPANESE
#PAKLIB =	-lgnos_jpn
# for LANG_ENGLISH
PAKLIB =	-lgnos

#FINAL = YES

ifeq ($(FINAL), YES)
OPTIMIZER       = -O2
LCDEFS          = -DNDEBUG -D_FINALROM -DF3DEX_GBI_2
N64LIB          = -lgultra_rom
else
OPTIMIZER       = -g
LCDEFS          = -DDEBUG -DF3DEX_GBI_2
N64LIB          = -lgultra_d
endif

APP =		onetri.out

TARGETS =	onetri.n64

TEXHFILES =	brick.h

HFILES =	$(TEXHFILES) onetri.h

CODEFILES   =	onetri.c dram_stack.c rdp_output.c

CODEOBJECTS =	$(CODEFILES:.c=.o)

DATAFILES   =	static.c cfb.c rsp_cfb.c

DATAOBJECTS =	$(DATAFILES:.c=.o)

CODESEGMENT =	codesegment.o

OBJECTS =	$(CODESEGMENT) $(DATAOBJECTS)

LCINCS =	-I. -I../lib
LCOPTS =	-G 0
LDFLAGS =	$(MKDEPOPT) -L../lib $(PAKLIB) -L$(ROOT)/usr/lib $(N64LIB) -L$(GCCDIR)/mipse/lib -lkmc

LDIRT  =	$(APP)

default:	$(TARGETS)

include $(COMMONRULES)

$(CODESEGMENT):	$(CODEOBJECTS)
		$(LD) -o $(CODESEGMENT) -r $(CODEOBJECTS) $(LDFLAGS)

ifeq ($(FINAL), YES)
$(TARGETS) $(APP):      spec $(OBJECTS)
	$(MAKEROM) -s 9 -r $(TARGETS) -e $(APP) spec
	makemask $(TARGETS)
else
$(TARGETS) $(APP):      spec $(OBJECTS)
	$(MAKEROM) -r $(TARGETS) -e $(APP) spec
endif

# for exeGCC CELF
ifeq ($(GCC_CELF), ON)
CELFDIR = .
include $(CELFRULES)
$(CODEOBJECTS) $(DATAOBJECTS) :	$(CELFINFOFILES)
endif
