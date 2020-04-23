#!smake
# --------------------------------------------------------------------
#        Copyright (C) 1997,1998 Nintendo. (Originated by SGI)
#
#        $RCSfile: Makefile,v $
#        $Revision: 1.17 $
#        $Date: 1999/07/13 08:34:36 $
# --------------------------------------------------------------------

include $(ROOT)/usr/include/make/PRdefs

#FINAL = YES
#ERROR =
ifeq ($(FINAL), YES)
OPTIMIZER       = -O2
LCDEFS          = -DNDEBUG -D_FINALROM
N64LIB          = -lgultra_rom
else
OPTIMIZER       = -g
ifeq ($(ERROR), YES)
LCDEFS          = -DDEBUG -DULTRA_DEBUG
N64LIB          = -lgultra_d
else
LCDEFS          = -DDEBUG
N64LIB          = -lgultra
endif
endif

APP =		soundmonkey.out

TARGETS =	soundmonkey.n64

HFILES =	soundmonkey.h

CODEFILES =	soundmonkey.c
CODEOBJECTS =	$(CODEFILES:.c=.o)

CODESEGMENT =	codesegment.o

OBJECTS =	$(CODESEGMENT)

LCINCS =	-I. -I$(ROOT)/usr/include/PR
LCOPTS =	-G 0
LDIRT  =	$(APP) $(APP_EM) monkey.aud load.map load_em.map

LDFLAGS =	$(MKDEPOPT)  -L$(ROOT)/usr/lib -L$(ROOT)/usr/lib/PR \
		$(N64LIB) -L$(GCCDIR)/mipse/lib -lkmc

default:	$(TARGETS)

include $(COMMONRULES)

$(CODESEGMENT):	$(CODEOBJECTS)
		$(LD) -o $(CODESEGMENT) -r $(CODEOBJECTS) $(LDFLAGS)

ifeq ($(FINAL), YES)
$(TARGETS) $(APP):      spec $(OBJECTS) 
	$(MAKEROM) -r $(TARGETS) -e $(APP) spec
	makemask $(TARGETS)
else
$(TARGETS) $(APP):      spec $(OBJECTS) 
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
