#!smake -k
# --------------------------------------------------------------------
#        Copyright (C) 1997,1998 Nintendo. (Originated by SGI)
#
#        $RCSfile: Makefile,v $
#        $Revision: 1.11 $
#        $Date: 1999/04/02 10:12:29 $
# --------------------------------------------------------------------
#
# Makefile for overlay demo application
#
#

include $(ROOT)/usr/include/make/PRdefs

#FINAL = YES
ifeq ($(FINAL), YES)
OPTIMIZER       = -O2
LCDEFS          = -DNDEBUG -D_FINALROM -DF3DEX_GBI_2
N64LIB          = -lgultra_rom
else
OPTIMIZER       = -g
LCDEFS          = -DF3DEX_GBI_2 -DDEBUG
N64LIB          = -lgultra_d
endif

APP	=	overlay.out

TARGETS =	overlay.n64

TEXHFILES =	brick.h

HFILES =	$(TEXHFILES) overlay.h

KERNCFILES   =	overlay.c dram_stack.c

KERNOBJECTS =	$(KERNCFILES:.c=.o)

SEGCFILES   =	plain.c texture.c cfb.c rsp_cfb.c

OBJECTS =	$(SEGCFILES:.c=.o) kernel.o

LCINCS =	-I.
LCOPTS =	-G 0
LDFLAGS =	$(MKDEPOPT)  -L$(ROOT)/usr/lib $(N64LIB) -L$(GCCDIR)/mipse/lib -lkmc

LDIRT  =	plain texture

default:	$(TARGETS)

include $(COMMONRULES)

#
# Note the use of the "-u" flag to force inclusion of the given routines
# into the kernel segment, although they are not referenced by any other
# code in the kernel segment.  They are, however, referenced by the
# "plain" and "texture" segments and thus these routines are included in
# the kernel segment.
#
kernel.o:	$(KERNOBJECTS) Makefile
		$(LD) -o $@ -r $(KERNOBJECTS) $(LDFLAGS)

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
ifneq ($(FINAL), YES)
CELFDIR = .
include $(CELFRULES)
$(CODEOBJECTS) $(DATAOBJECTS) :	$(CELFINFOFILES)
endif
endif
