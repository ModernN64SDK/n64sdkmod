#=======================================================================#
#		NIFFprev						#
#		Makefile.win						#
#									#
#		Copyright (C) 1997, NINTENDO Co,Ltd.			#
#									#
#-----------------------------------------------------------------------# 
#

# The directory which has include files and library of NuSYSTEM
#
include /usr/include/n64/make/PRdefs
	
NUSYSINCDIR  = /usr/include/n64/nusys
NUSYSLIBDIR  = /usr/lib/n64/nusys

N64GCC_INC =	/usr/include/n64
USR_INC    =	./Include
COMMON_INC =	../Include

GBI =		-DF3DEX_GBI_2
.c.o:
	$(CC) -g -G 0 -c $(CC_INC) $(CC_DEF) $<

CC_INC =	-I. -I$(N64GCC_INC)/PR -I$(N64GCC_INC) -I$(N64GCC_INC)/nustd -I$(NUSYSINCDIR) -I$(USR_INC) -I$(COMMON_INC)
CC_DEF =	$(GBI) -D_WIN32

OPTIMIZER =	-g

TARGETS =	libnd.a

CODEFILES   =	ndres.c ndman.c ndeval.c nddisp.c \
		ndstuff.c

DATAOBJECTS =	$(CODEFILES:.c=.o)

OBJECTS =	$(DATAOBJECTS)

DEPENDFILE =    dependfile

default:	$(TARGETS)

$(TARGETS) :	$(DATAOBJECTS)
	mips-n64-ar -cr $(TARGETS) $(OBJECTS)
	mips-n64-ranlib $(TARGETS)

main.o : model.h

premodel.o : model.c model.h

depend:
	makedepnd -f$(DEPENDFILE) *.c

clean:
	rm -rf *.o
	rm -rf *.a

install:
	cp $(TARGETS) ../Lib

# DO NOT DELETE
