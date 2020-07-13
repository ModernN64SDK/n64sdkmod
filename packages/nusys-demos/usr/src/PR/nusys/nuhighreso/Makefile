#
# 	NuSystem highreso Program 
#
#	Copyright (C) 1997, NINTENDO Co,Ltd.
#

include $(ROOT)/usr/include/make/PRdefs

#
# NUSYS LIBRARY
#
#
N64KITDIR    = c:\nintendo\n64kit
NUSYSINCDIR  = /usr/include/n64/nusys
NUSYSLIBDIR  = /usr/lib/n64/nusys

LIB = $(ROOT)/usr/lib
LPR = $(LIB)/PR
INC = $(ROOT)/usr/include

APP 		=	highreso.out

OPTIMIZER 	=	-g

TARGETS 	=	highreso.n64

TEXHFILES 	=

HFILES 		= 	localdef.h

CODEFILES   	= 	main.c graphics.c

CODEOBJECTS	=	$(CODEFILES:.c=.o) $(NUSYSLIBDIR)/nusys.o

DATAFILES   	=

DATAOBJECTS	=	$(DATAFILES:.c=.o)

CODESEGMENT	=	codesegment.o

OBJECTS		=	$(CODESEGMENT) $(DATAOBJECTS)


LCDEFS = 
LCINCS =	-I. -I$(NUSYSINCDIR)
LCOPTS =	-G 0
LDFLAGS =	$(MKDEPOPT) -L$(ROOT)/usr/lib  -lnusys_d -L$(NUSYSLIBDIR) -lultra_d  -L$(N64_LIBGCCDIR) -L$(N64_NEWLIBDIR) -lgcc -lc

LDIRT  =	$(APP)

default:	$(TARGETS)

include $(COMMONRULES)

$(CODESEGMENT):	$(CODEOBJECTS)
		$(LD) -o $(CODESEGMENT) -r $(CODEOBJECTS) $(LDFLAGS)

$(TARGETS) $(APP):	spec $(OBJECTS)
		$(MAKEROM)  spec -I$(NUSYSINCDIR) -r $(TARGETS) -s 10 -e $(APP)
		makemask $(TARGETS)
