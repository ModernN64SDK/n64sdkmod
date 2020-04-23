# --------------------------------------------------------------------
#        Copyright (C) 1997,1998 Nintendo.
#
#        $RCSfile: Makefile,v $
#        $Revision: 1.7 $
#        $Date: 1998/09/29 08:48:38 $
# --------------------------------------------------------------------
include $(ROOT)/usr/include/make/PRdefs

APP		= main.out

OPTIMIZER	= -g

TARGETS		= main.n64

HFILES		= nu64sys.h sram.h

CODEFILES	= main.c sraminit.c

CODEOBJECTS	= $(CODEFILES:.c=.o)

DATAFILES	= readbuf.c writebuf.c

DATAOBJECTS	= $(DATAFILES:.c=.o)

CODESEGMENT	= codesegment.o

OBJECTS		= $(CODESEGMENT) $(DATAOBJECTS)

LCDEFS		= -DDEBUG
LCINCS		= -I. -I$(ROOT)/usr/include/PR -I $(ROOT)/usr/include

LCOPTS =	-G 0
LDIRT		= $(APP) main

LDFLAGS		= $(MKDEPOPT)  -L$(ROOT)/usr/lib -lgultra_d -L$(GCCDIR)/mipse/lib -lkmc

default: $(TARGETS)

include $(COMMONRULES)

$(CODESEGMENT): main.o sraminit.o
	$(LD) -o $(CODESEGMENT) -r main.o sraminit.o $(LDFLAGS)

$(TARGETS) $(APP): spec $(OBJECTS)
	$(MAKEROM) -r $(TARGETS) -e $(APP) spec

# for exeGCC CELF
ifeq ($(GCC_CELF), ON)
ifneq ($(FINAL), YES)
CELFDIR = .
include $(CELFRULES)
$(CODEOBJECTS) $(DATAOBJECTS) :	$(CELFINFOFILES)
endif
endif
