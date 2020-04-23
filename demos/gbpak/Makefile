#!smake -k
# --------------------------------------------------------------------
#        Copyright (C) 1998 Nintendo.
#
#        $RCSfile: Makefile,v $
#        $Revision: 1.11 $
#        $Date: 1999/07/13 08:33:27 $
# --------------------------------------------------------------------
#
# Makefile for gbpak test application
#
#

include $(ROOT)/usr/include/make/PRdefs

#LANG_ENGLISH =
#FINAL = YES
ifeq ($(FINAL), YES)
OPTIMIZER	= -O2
N64LIB		= -lgultra_rom
LCDEFS          = -DNDEBUG -D_FINALROM
else
OPTIMIZER	= -g
N64LIB		= -lgultra_d
LCDEFS          =
endif

APP		= main.out
TARGETS		= main.n64

HFILES		= main.h nu64sys.h siproc.h graph.h thread.h mes_e.h

ifeq ($(LANG_ENGLISH), YES)
CODEFILES	= main.c siproc.c gbpaksramreadwrite.c \
		  nu64sys.c asci.c graph.c
LCDEFS		+= -DLANG_ENGLISH
else
CODEFILES	= main.c siproc.c gbpaksramreadwrite.c \
		  nu64sys.c asci.c graph.c \
		  putkanji.c printkanji.c

KCODEFILES 	= kfont.c mes.c
endif

CODEOBJECTS	= $(CODEFILES:.c=.o) $(KCODEFILES:.c=.o)

DATAFILES	= cfb.c
DATAOBJECTS	= $(DATAFILES:.c=.o)

CODESEGMENT	= codesegment.o

OBJECTS		= $(CODESEGMENT) $(DATAOBJECTS)

LCINCS		= -I. -I$(ROOT)/usr/include/PR -I$(ROOT)/usr/include

LCOPTS =	-G 0

LDIRT		= $(APP)

LDFLAGS		= $(MKDEPOPT)  -L$(ROOT)/usr/lib \
		-L$(ROOT)/usr/lib/PR $(N64LIB) -L$(GCCDIR)/mipse/lib -lkmc

default:	$(TARGETS)

include $(COMMONRULES)

$(CODESEGMENT):	$(CODEOBJECTS) $(GBPAKLIB)
	$(LD) -o $(CODESEGMENT) -r $(CODEOBJECTS) $(LDFLAGS)

ifeq ($(FINAL), YES)
$(TARGETS) $(APP):	spec $(OBJECTS)
	$(MAKEROM) -s 9 -r $(TARGETS) -e $(APP) spec
	makemask $(TARGETS)
else
$(TARGETS) $(APP):	spec $(OBJECTS)
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
