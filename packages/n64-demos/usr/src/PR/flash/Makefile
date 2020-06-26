#!smake -k
# --------------------------------------------------------------------
#        Copyright (C) 1997,1998 Nintendo. (Originated by SGI)
#        
# --------------------------------------------------------------------
#
# Makefile for 1M flash test application 
#
#

include $(ROOT)/usr/include/make/PRdefs

#FINAL =

#ifdef FINAL
OPTIMIZER       = -O2
LCDEFS          = -DNDEBUG -D_FINALROM -DF3DEX_GBI_2
N64LIB          = -lultra_rom
#else
OPTIMIZER       = -g
LCDEFS          = -DDEBUG -DF3DEX_GBI_2
N64LIB          = -lultra_d
#endif

APP =		flash

TARGETS =	rom

HFILES =	buffer.h display.h graph.h nu64sys.h thread.h

CODEFILES   =	nu64sys.c main.c graph.c asci.c

CODEOBJECTS =	$(CODEFILES:.c=.o)

DATAFILES   =	cfb.c buffer.c

DATAOBJECTS =	$(DATAFILES:.c=.o)

CODESEGMENT =	codesegment.o

OBJECTS =	$(CODESEGMENT) $(DATAOBJECTS)

LCINCS =	-I. -I$(ROOT)/usr/include/PR -I $(ROOT)/usr/include
LCOPTS =	-fullwarn -wlint,-f -woff 813,819,826,827,852 -non_shared -G 0
LDFLAGS =	$(MKDEPOPT) -nostdlib -L$(ROOT)/usr/lib $(N64LIB)

LDIRT  =	$(APP)

default:	$(TARGETS)

include $(COMMONRULES)

install:	default
		$(INSTALL) -m 444 -F /usr/src/PR/demos/flash \
			$(HFILES) $(CODEFILES) $(DATAFILES) Makefile README.jp spec

$(CODESEGMENT):	$(CODEOBJECTS)
		$(LD) -o $(CODESEGMENT) -r $(CODEOBJECTS) $(LDFLAGS)

#ifdef FINAL
$(TARGETS) $(APP):      spec $(OBJECTS)
	$(MAKEROM) -s 9 -r $(TARGETS) spec
#ifndef NOMAKEMASK
	makemask $(TARGETS)
#endif
#else
$(TARGETS) $(APP):      spec $(OBJECTS)
	$(MAKEROM) -r $(TARGETS) spec
#endif
