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
COM_INC    =	../Include

GBI =		-DF3DEX_GBI_2

.c.o:
		$(CC) -g -G 0 -c $(CC_INC) $(CC_DEF) $<

CC_INC =	-I. -I$(N64GCC_INC)/PR -I$(N64GCC_INC) -I$(NUSYSINCDIR) -I$(USR_INC) -I$(COM_INC)
CC_DEF =	$(GBI) -D_WIN32

OPTIMIZER =	-g

TARGETS =	libresbg.a

CODEFILES   =	_aa_group.c \
		_bb_group.c \
		_cc_group.c \
		_dd_group.c \
		_ee_group.c

DATAOBJECTS =	$(CODEFILES:.c=.o)

OBJECTS =	$(DATAOBJECTS)

DEPENDFILE =    dependfile

default:	$(TARGETS)

$(TARGETS) :	$(DATAOBJECTS)
	mips-n64-ar -cr $(TARGETS) $(OBJECTS)
	mips-n64-ranlib $(TARGETS)

map_group.o : map_group.h _map_group.h _map_group.c
_aa_group.o : _aa_group.h aa_group.c
_bb_group.o : _bb_group.h bb_group.c
_cc_group.o : _cc_group.h cc_group.c
_dd_group.o : _dd_group.h dd_group.c
_ee_group.o : _ee_group.h ee_group.c

depend:
	makedepnd -f$(DEPENDFILE) *.c

clean:
	rm -rf *.o
	rm -rf *.a

install:
	cp $(TARGETS) ../Lib

# DO NOT DELETE
