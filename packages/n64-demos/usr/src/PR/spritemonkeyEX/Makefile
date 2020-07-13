#!smake -k -J 4
# --------------------------------------------------------------------
#        Copyright (C) 1997,1998 Nintendo. (Originated by SGI)
#
#        $RCSfile: Makefile,v $
#        $Revision: 1.2 $
#        $Date: 1999/07/13 08:34:46 $
# --------------------------------------------------------------------
#
# Makefile for sprite demo application
#
#

include $(ROOT)/usr/include/make/PRdefs

#FINAL = YES
ifeq ($(FINAL), YES)
OPTIMIZER       = -O2 -std=gnu90
LCDEFS          = -DNDEBUG -D_FINALROM
N64LIB          = -lultra_rom
else
OPTIMIZER       = -g -std=gnu90
LCDEFS          = -DDEBUG
N64LIB          = -lultra_d
endif

APP =		sprite.out

TARGETS =	sprite.n64

HFILES =        Tile_Rect.h Cont.h Task.h Dma.h FrameBuffer.h RcpRdp.h

CODEFILES   =   main.c dram_stack.c \
                tile_rect.c task.c rcprdp.c framebuffer.c dma.c \
                cont.c game.c

CODEOBJECTS =   $(CODEFILES:.c=.o)

DATAFILES   =   static.c cfb.c rsp_cfb.c dk.c dk7.c road.c l2_tv.c

DATAOBJECTS =   $(DATAFILES:.c=.o)

CODESEGMENT =	codesegment.o

OBJECTS =	$(CODESEGMENT) $(DATAOBJECTS)

LCINCS =	-I. -I$(ROOT)/usr/include/PR
LCDEFS +=       $(HW_FLAGS) -DF3DEX_GBI_2 -DUSE_CFB32
LCOPTS =	-G 0
LDFLAGS =	$(MKDEPOPT)  -L$(ROOT)/usr/lib $(N64LIB) -L$(N64_LIBGCCDIR) -lgcc -L$(N64_NEWLIBDIR) -lc

LDIRT  =	$(APP)

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
