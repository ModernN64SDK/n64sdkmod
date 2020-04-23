#!smake -k
# --------------------------------------------------------------------
#        Copyright (C) 1997,1998 Nintendo.
#
#        $RCSfile: Makefile,v $
#        $Revision: 1.10 $
#        $Date: 1999/07/13 08:33:38 $
# --------------------------------------------------------------------
#
# Makefile for performance demo application
#
#

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

APP		= main.out
TARGETS		= main.n64

HFILES		= motortest.h nu64sys.h thread.h

CODEFILES	= motortest.c nu64sys.c asci.c
CODEOBJECTS	= $(CODEFILES:.c=.o)

DATAFILES	= cfb1.c
DATAOBJECTS	= $(DATAFILES:.c=.o)

CODESEGMENT	= codesegment.o

OBJECTS		= $(CODESEGMENT) $(DATAOBJECTS)

LCINCS		= -I. -I$(ROOT)/usr/include/PR -I$(ROOT)/usr/include

LCOPTS =	-G 0

LDIRT		= $(APP) cont *.so

LDFLAGS		= $(MKDEPOPT)  -L$(ROOT)/usr/lib \
		-L$(ROOT)/usr/lib/PR $(N64LIB) -L$(GCCDIR)/mipse/lib -lkmc

SOURCES =	Makefile spec $(HFILES) $(CODEFILES) \
		$(DATAFILES)

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

nu64sys.o:	nu64sys.c nu64sys.h

asci.o:		asci.c

motortest.o:	motortest.c motortest.h


# for exeGCC CELF
ifeq ($(GCC_CELF), ON)
ifneq ($(FINAL), YES)
CELFDIR = .
include $(CELFRULES)
$(CODEOBJECTS) $(DATAOBJECTS) :	$(CELFINFOFILES)
endif
endif
