#!smake -k
# --------------------------------------------------------------------
#        Copyright (C) 1997,1998 Nintendo. (Originated by SGI)
#
#        $RCSfile: Makefile,v $
#        $Revision: 1.16 $
#        $Date: 1999/04/02 10:09:07 $
# --------------------------------------------------------------------
#
# Makefile for chrome effect demo application
#
#
#

include $(ROOT)/usr/include/make/PRdefs

#FINAL = YES
ifeq ($(FINAL), YES)
OPTIMIZER       = -O2 -std=gnu90
LCDEFS          = -DNDEBUG -D_FINALROM -DF3DEX_GBI_2
N64LIB          = -lultra_rom
else
OPTIMIZER       = -g -std=gnu90
LCDEFS          = -DDEBUG -DF3DEX_GBI_2
N64LIB          = -lultra_d
endif

APP =		letters.out

TARGETS =	letters.n64

TEXTURES =	./Texture/metallic.rgb ./Texture/mahogany.rgb ./Texture/smroad.rgb ./Texture/gameday.rgb

TEXHFILES =	./mahogany.h ./metallic.h ./smroad.h ./sp_gameday.h

HFILES =	letters.h u64_tri.h u64_vtx.h

CODEFILES   =	letters.c dram_stack.c

CODEOBJECTS =	$(CODEFILES:.c=.o)

DATAFILES   =	static.c	\
		zbuffer.c	\
		cfb.c

DATAOBJECTS =	$(DATAFILES:.c=.o)

CODESEGMENT =	codesegment.o

OBJECTS =	$(TEXHFILES) $(CODESEGMENT) $(DATAOBJECTS)

LCINCS =	-I.
LCOPTS =	-G 0
LDIRT  =	$(APP) *.so

LDFLAGS =	$(MKDEPOPT)  -L$(ROOT)/usr/lib -L$(ROOT)/usr/lib/PR $(N64LIB) -L$(N64_LIBGCCDIR) -lgcc 

default:	$(TARGETS)

include $(COMMONRULES)

$(CODESEGMENT):	$(TEXHFILES) $(CODEOBJECTS)
		$(LD) -o $(CODESEGMENT) -r $(CODEOBJECTS) $(LDFLAGS)

ifeq ($(FINAL), YES)
$(TARGETS) $(APP):      spec $(OBJECTS)
	$(MAKEROM) -s 9 -r $(TARGETS) -e $(APP) spec
	makemask $(TARGETS)
else
$(TARGETS) $(APP):      spec $(OBJECTS)
	$(MAKEROM) -r $(TARGETS) -e $(APP) spec
endif


