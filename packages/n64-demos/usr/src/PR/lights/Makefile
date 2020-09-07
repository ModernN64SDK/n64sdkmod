#!smake -k
# --------------------------------------------------------------------
#        Copyright (C) 1997,1998 Nintendo. (Originated by SGI)
#
#        $RCSfile: Makefile,v $
#        $Revision: 1.10 $
#        $Date: 1999/04/02 10:11:20 $
# --------------------------------------------------------------------
#
# Makefile for teapot demo application
#
#
#

include $(ROOT)/usr/include/make/PRdefs

#FINAL = YES
ifeq ($(FINAL), YES)
OPTIMIZER       = -O2
LCDEFS          = -DNDEBUG -D_FINALROM -DF3DEX_GBI_2
N64LIB          = -lultra_rom
else
OPTIMIZER       = -g
LCDEFS          = -DDEBUG -DF3DEX_GBI_2
N64LIB          = -lultra_d
endif

APP =		teapot.out

TARGETS =	teapot.n64

TEXHFILES =	teapot_shad64.h

HFILES =	I4spec.h controller.h teapot_tri.h \
		IA8phong.h teapot.h teapot_vtx.h

CODEFILES   =	teapot.c controller.c dram_stack.c

CODEOBJECTS =	$(CODEFILES:.c=.o)

DATAFILES   =	static.c	\
		zbuffer.c	\
		cfb.c

DATAOBJECTS =	$(DATAFILES:.c=.o)

CODESEGMENT =	codesegment.o

OBJECTS =	$(CODESEGMENT) $(DATAOBJECTS)

LCDEFS +=	-DUSE_HIGHLIGHT -DUSE_MIPMAP
LCINCS =	-I. -I$(ROOT)/usr/include/PR -I $(ROOT)/usr/include
LCOPTS =	-G 0
LDIRT  =	$(APP) teapot *.so
	
LDFLAGS =	$(MKDEPOPT)  -L$(ROOT)/usr/lib -L$(ROOT)/usr/lib/PR $(N64LIB)  -L$(N64_LIBGCCDIR) -lgcc 

default:	$(TARGETS)

include $(COMMONRULES)

$(CODESEGMENT):	$(CODEOBJECTS)
		$(LD) -o $(CODESEGMENT) -r $(CODEOBJECTS) $(LDFLAGS)

$(OBJECTS): $(HFILES) $(TEXHFILES)

ifeq ($(FINAL), YES)
$(TARGETS) $(APP):      spec $(OBJECTS)
	$(MAKEROM) -s 9 -r $(TARGETS) -e $(APP) spec
	makemask $(TARGETS)
else
$(TARGETS) $(APP):      spec $(OBJECTS)
	$(MAKEROM) -r $(TARGETS) -e $(APP) spec
endif


