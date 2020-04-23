#!smake -k
# --------------------------------------------------------------------
#        Copyright (C) 1997,1998 Nintendo. (Originated by SGI)
#
#        $RCSfile: Makefile,v $
#        $Revision: 1.28 $
#        $Date: 1999/07/15 10:20:56 $
# --------------------------------------------------------------------
#
# Makefile for ddspgame demo application
#
#

#START_FROM_CASSETTE = YES
#FINAL = YES
include $(ROOT)/usr/include/make/PRdefs
PRDEPTH = ../..

ifeq ($(FINAL), YES)
OPTIMIZER =	-O2
LCDEFS = -DNDEBUG -D_FINALROM -DF3DEX_GBI_2
N64LIB	= -lleo -lgultra_rom
else
OPTIMIZER =	-g
LCDEFS = -DDEBUG -DF3DEX_GBI_2
N64LIB	= -lleo_d -lgultra_d
endif

APP =		main.out
TARGETS =	main.n64

TEXDIR  =	Texture

TEXTURES = 	${TEXDIR}/L2_TV.rgb  \
		${TEXDIR}/ball.rgba

TEXHFILES = 	./${TEXDIR}/lit_imgs.h          ./${TEXDIR}/lit_bms.h \
		./${TEXDIR}/ball_imgs.h         ./${TEXDIR}/ball_bms.h

SPRITES =       ./sp_lit.h ./sp_ball.h

HFILES =	cont.h font.h hdd.h herr.h hscr.h htry.h \
		main.h sysassigned.h hdd_internal.h

CODEFILES   =	main.c spgame.c font.c pics.c \
		cont.c dd.c messages.c static.c \
		heap.c try.c test_64dd.c err.c dram_stack.c

CODEOBJECTS =	$(CODEFILES:.c=.o)

DATAFILES   =	cfb.c rsp_cfb.c kbuffer.c

DATAOBJECTS =	$(DATAFILES:.c=.o)

DOCFILES = 	README.jp command.jp funcs.jp

CODESEGMENT =	codesegment.o

OBJECTS =	$(CODESEGMENT) $(DATAOBJECTS)

ifeq ($(START_FROM_CASSETTE), YES)
LCDEFS += -DSTART_FROM_CASSETTE
endif

LCINCS =	-I. -I$(ROOT)/usr/include/PR \
		-I$(ROOT)/PR/libultra/io -I$(ROOT)/PR/libultra/os
LCOPTS =	-G 0

LDFLAGS =	$(MKDEPOPT)  -L$(ROOT)/usr/lib $(N64LIB) -L$(GCCDIR)/mipse/lib -lkmc

LDIRT  =	$(APP) gwrite  disk

default:	$(TARGETS)

include $(COMMONRULES)

$(CODESEGMENT):	$(CODEOBJECTS)
		$(LD) -o $(CODESEGMENT) -r $(CODEOBJECTS) $(LDFLAGS)

ifeq ($(START_FROM_CASSETTE), YES)
ifeq ($(FINAL), YES)
$(TARGETS) $(APP):	spec $(OBJECTS) 
		$(MAKEROM) -s 9 -r $(TARGETS) -e $(APP) spec
		makemask $(TARGETS)
else
$(TARGETS) $(APP):	spec $(OBJECTS) 
		$(MAKEROM) -r $(TARGETS) -e $(APP) spec
endif
else
ifeq ($(FINAL), YES)
$(TARGETS) $(APP):	spec $(OBJECTS) 
		$(MAKEROM) -DD64 -t 0 -r $(TARGETS) spec
		$(ROOT)/usr/sbin/n64mdisk n64mdisk.cfg
else
$(TARGETS) $(APP):	spec $(OBJECTS) 
		$(MAKEROM) -DD64 -t 0 -r $(TARGETS) spec
endif
endif


# for exeGCC CELF
ifeq ($(GCC_CELF), ON)
ifneq ($(FINAL), YES)
CELFDIR = .
include $(CELFRULES)
$(CODEOBJECTS) $(DATAOBJECTS) :	$(CELFINFOFILES)
endif
endif
