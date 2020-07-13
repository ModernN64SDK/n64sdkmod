# 	NuSYSTEM samples nu3 Makefile
#       Copyright (C) 1997-1999, NINTENDO Co,Ltd.

include $(ROOT)/usr/include/make/PRdefs

# The directory which has the include file and library of NuSYSTEM
#
NUSYSINCDIR  = /usr/include/n64/nusys
NUSYSLIBDIR  = /usr/lib/n64/nusys

LIB = $(ROOT)/usr/lib
LPR = $(LIB)/PR
INC = $(ROOT)/usr/include

# When you use the former audio micro codes,
# please comment this out.
N_AUDIO = yes

ifdef N_AUDIO
NUAUDIOLIB = -lnualsgi_n_d -ln_audio_d
else
NUAUDIOLIB = -lnualsgi_d
endif

LCDEFS =	-DNU_DEBUG -DF3DEX_GBI_2
LDIRT  =	$(APP) $(TARGETS)
LCINCS =	-I. -I$(NUSYSINCDIR) -I$(ROOT)/usr/include/PR
LCOPTS =	-G 0
LDFLAGS =	$(MKDEPOPT) -L$(LIB) -L$(NUSYSLIBDIR) $(NUAUDIOLIB) -lnusys_d -lultra_d -L$(N64_LIBGCCDIR) -lgcc -L$(N64_NEWLIBDIR) -lc

OPTIMIZER =	-g

APP =		nu3.out

TARGETS =	nu3.n64

HFILES =	main.h graphic.h segment.h

CODEFILES   = 	main.c stage00.c graphic.c gfxinit.c

CODEOBJECTS =	$(CODEFILES:.c=.o)  $(NUSYSLIBDIR)/nusys.o

DATAFILES   =	

DATAOBJECTS =	$(DATAFILES:.c=.o)

CODESEGMENT =	codesegment.o

OBJECTS =	$(CODESEGMENT) $(DATAOBJECTS)


default:        $(TARGETS)

include $(COMMONRULES)

$(CODESEGMENT):	$(CODEOBJECTS) Makefile
		$(LD) -o $(CODESEGMENT) -r $(CODEOBJECTS) $(LDFLAGS)

$(TARGETS):	$(OBJECTS)
		$(MAKEROM) spec -I$(NUSYSINCDIR) -r $(TARGETS) -s 10 -e $(APP)
		makemask $(TARGETS)
