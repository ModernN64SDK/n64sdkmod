#!smake -k
# --------------------------------------------------------------------
#        Copyright (C) 1997,1998 Nintendo. (Originated by SGI)
#
#        $RCSfile: Makefile,v $
#        $Revision: 1.30 $
#        $Date: 1999/04/02 10:14:23 $
# --------------------------------------------------------------------
#
# Makefile for topgun demo application
#
#
#
include /usr/include/n64/make/PRdefs

FINAL = YES
ifeq ($(FINAL), YES)
OPTIMIZER       = -O2 -std=gnu90
LCDEFS          = -D_FINALROM -DNDEBUG -DF3DEX_GBI_2
N64LIB          = -lultra_rom
else
OPTIMIZER       = -g -std=gnu90
LCDEFS          = -DDEBUG  -DF3DEX_GBI_2
N64LIB          = -lultra_d
endif

APP =		topgun.out

TARGETS =	topgun.n64

TEXHFILES =	ground.h sky.h tree.h
TEXTURES =	Textures/clouds.rgb Textures/harrier_num.rgba \
		Textures/stree1.rgba  Textures/terrain.rgb \
		Textures/camo2.int Textures/decal.inta

MODELS =	Models/av8b.flt

HFILES =	topgun.h

CODEFILES   =	topgun.c dram_stack.c rdp_output.c

CODEOBJECTS =	$(CODEFILES:.c=.o)

DATAFILES   =	av8b.c		\
		cfb.c		\
		ground.c	\
		static.c	\
		zbuffer.c

DATAOBJECTS =	$(DATAFILES:.c=.o)

CODESEGMENT =	codesegment.o

OBJECTS =	$(CODESEGMENT) $(DATAOBJECTS)

LCINCS =	-I. -I/usr/include/n64/PR -I/usr/include/n64
LCOPTS =	-G 0
LDIRT  =	$(APP)

LDFLAGS =	$(MKDEPOPT)  -L/usr/lib/n64 -L/usr/lib/n64/PR $(N64LIB) -L$(N64_LIBGCCDIR) -lgcc

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
ifneq ($(FINAL), YES)
CELFDIR = .
include $(CELFRULES)
$(CODEOBJECTS) $(DATAOBJECTS) :	$(CELFINFOFILES)
endif
endif
