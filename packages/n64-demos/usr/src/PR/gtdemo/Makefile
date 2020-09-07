#!smake -k
# --------------------------------------------------------------------
#        Copyright (C) 1997,1998 Nintendo. (Originated by SGI)
#
#        $RCSfile: Makefile.src,v $
#        $Revision: 1.1 $
#        $Date: 1999/01/12 13:28:00 $
# --------------------------------------------------------------------
#
# Makefile for turbo demo application
#

include $(ROOT)/usr/include/make/PRdefs

#FINAL = YES
ifeq ($(FINAL), YES)
OPTIMIZER       = -O2 -std=gnu90
LCDEFS          =
N64LIB          = -lultra_rom
else
OPTIMIZER       = -g -std=gnu90
LCDEFS          = -DDEBUG
N64LIB          = -lultra_d
endif

APP =		gtdemo.out

TARGETS =	gtdemo.n64

TEXHFILES =	../RGBA16checkerMM.h

HFILES =	gtdemo.h controller.h turbo.h character.h char_data.h 	\
		timer.h walk.h punch.h

CODEFILES   =	gtdemo.c floor.c turbo.c character.c controller.c 	\
		timer.c walk.c punch.c rdp_output.c rdp_output_len.c

CODEOBJECTS =	$(CODEFILES:.c=.o) 

DATAFILES   =	static.c zbuffer.c cfb.c

DATAOBJECTS =	$(DATAFILES:.c=.o)

CODESEGMENT =	codesegment.o

OBJECTS =	$(CODESEGMENT) $(DATAOBJECTS)

LCINCS =	-I. -I$(ROOT)/usr/include/PR -I $(ROOT)/usr/include -I../Texture
LCOPTS =	-G 0
LDIRT  =	$(APP) *.so walktable walktable.h punchtable punchtable.h

#
# Use non debug library just so we have some demos using it to test it
#
LDFLAGS =	$(MKDEPOPT)  -L$(ROOT)/usr/lib -L$(ROOT)/usr/lib/PR $(N64LIB) -L$(N64_LIBGCCDIR) -lgcc -L$(N64_NEWLIBDIR) -lc

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


#
# Header file build rules
#
walk.o:	walktable.h

punch.o:	punchtable.h


walktable:	walktable.c
	$(HOST_CC) -o $@ walktable.c

walktable.h:	walktable
	./walktable > walktable.h	

punchtable:	punchtable.c
	$(HOST_CC) -o $@ punchtable.c

punchtable.h:	punchtable
	./punchtable > punchtable.h	






