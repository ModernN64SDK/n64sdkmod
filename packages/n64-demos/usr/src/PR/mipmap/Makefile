#!smake -k
# --------------------------------------------------------------------
#        Copyright (C) 1997,1998 Nintendo. (Originated by SGI)
#
#        $RCSfile: Makefile,v $
#        $Revision: 1.14 $
#        $Date: 1999/04/02 10:11:30 $
# --------------------------------------------------------------------
#
# Makefile for game template
#
#

include $(ROOT)/usr/include/make/PRdefs

ifeq ($(FINAL), YES)
OPTIMIZER       = -O2 -std=gnu90
LCDEFS          = -D_FINALROM -DNDEBUG -DF3DEX_GBI_2
N64LIB          = -lultra_rom
else
OPTIMIZER       = -g -std=gnu90
LCDEFS          = -DDEBUG -DF3DEX_GBI_2
N64LIB          = -lultra_d
endif

APP =		game.out

TARGETS =	game.n64

TEXHFILES =	./RGBA16molecule.h ./RGBA16moleculeMM.h 	\
		./RGBA16moleculeHMM.h 				\
		./RGBA16brick.h ./RGBA16brickMM.h 		\
		./RGBA16brickHMM.h				\
		./RGBA16chainlink.h ./RGBA16chainlinkMM.h 	\
		./RGBA16chainlinkHMM.h				\
		./RGBA16mymip.h ./RGBA16mymipMM.h 		\
		./RGBA16mymipHMM.h				\
		./RGBA16checker.h ./RGBA16checkerMM.h 		\
		./RGBA16checkerHMM.h

HFILES =	$(TEXHFILES) boot.h game.h controller.h font.h font_ext.h \
		static.h texture.h

CODEFILES   =	boot.c game.c controller.c font.c dram_stack.c

CODEOBJECTS =	$(CODEFILES:.c=.o)

DATAFILES   =	gfxinit.c gfxstatic.c texture.c cfb.c rsp_cfb.c zbuffer.c

DATAOBJECTS =	$(DATAFILES:.c=.o)

CODESEGMENT =	codesegment.o

OBJECTS =	$(CODESEGMENT) $(DATAOBJECTS)

LCDEFS +=	$(HW_FLAGS)
LCINCS =	-I.
LCOPTS =	-G 0
LDFLAGS =	$(MKDEPOPT)  -L$(ROOT)/usr/lib $(N64LIB) -L$(N64_LIBGCCDIR) -lgcc  -lnustd

LDIRT  =	$(APP)

default:	$(TARGETS)

include $(COMMONRULES)

gfxstatic.o:	$(TEXHFILES)

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

