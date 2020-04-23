# --------------------------------------------------------------------
#        Copyright (C) 1997 Nintendo. (Originated by SGI)
#
#        $RCSfile: Makefile,v $
#        $Revision: 1.5 $
#        $Date: 1998/09/29 07:20:12 $
# --------------------------------------------------------------------
#
# Makefile for app "fault"
#

include $(ROOT)/usr/include/make/PRdefs

APP =		fault.out

OPTIMIZER =	-g

TARGETS =	fault.n64

CODE_HFILES =	fault.h

CODE_CFILES =	fault.c printfault.c

CODE_OBJECTS=	$(CODE_CFILES:.c=.o)

CODESEGMENT =	code.o

LCINCS =	-I. -I$(ROOT)/usr/include/PR
LCOPTS =	-G 0
LASINCS =	$(LCINCS)
LASOPTS =	-non_shared -G 0

LDIRT  =	$(APP)

LDFLAGS =	$(MKDEPOPT)  -L$(ROOT)/usr/lib -lgultra_d -L$(GCCDIR)/mipse/lib -lkmc

default:	$(TARGETS)

include $(COMMONRULES)

$(CODESEGMENT):		$(CODE_OBJECTS)
		$(LD) -o $@ -r $(CODE_OBJECTS) $(LDFLAGS)

$(TARGETS) $(APP):	spec code.o 
		$(MAKEROM) -r $(TARGETS) -e $(APP) spec


# for exeGCC CELF
ifeq ($(GCC_CELF), ON)
ifneq ($(FINAL), YES)
CELFDIR = .
include $(CELFRULES)
$(CODEOBJECTS) $(DATAOBJECTS) :	$(CELFINFOFILES)
endif
endif
