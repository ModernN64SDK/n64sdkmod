#=======================================================================#
#		NIFFprev						#
#		Makefile.win						#
#									#
#		Copyright (C) 1997, NINTENDO Co,Ltd.			#
#									#
#-----------------------------------------------------------------------# 
#

# Directories in which NuSYSTEM include files and library are located.
#
include /usr/include/n64/make/PRdefs

NUSYSINCDIR  = /usr/include/n64/nusys
NUSYSLIBDIR  = /usr/lib/n64/nusys

N64GCC_INC =	/usr/include/n64
USR_INC    =	./Include

GBI =		-DF3DEX_GBI_2

.c.o:
		$(CC) -g -G 0 -c $(CC_INC) $(CC_DEF) $<

CC_INC =	-I. -I$(N64GCC_INC)/PR -I$(N64GCC_INC) -I$(NUSYSINCDIR) -I$(USR_INC)
CC_DEF =	$(GBI) -D_WIN32

OPTIMIZER =	-g

TARGETS =	libreschar.a

CODEFILES   =	_frake_walk.c \
		_corngroup.c \
		_goal.c \
		_frake_shadow.c \
		_frake_down.c \
		_frake_kime.c \
		_frake_select.c \
		_pump.c
#		_frake_eat.c \
#		premodel.c \
#		_pika2.c _pika_dead.c \
#		_window.c _kabe.c \
#		_a_wall.c
#		_yoshi_w.c _yoshi_dead.c \
#		_yoshi_a1.c \

DATAOBJECTS =	$(CODEFILES:.c=.o)

OBJECTS =	$(DATAOBJECTS)

DEPENDFILE =    dependfile

$(TARGETS) :	$(DATAOBJECTS)
	mips-n64-ar -cr $(TARGETS) $(OBJECTS)
	mips-n64-ranlib $(TARGETS)

_frake_walk.o : frake_walk.c
_frake_down.o : frake_down.c
_corngroup.c  : corngroup.c
_goal.c       : goal.c

premodel.o : model.c model.h

default:	$(TARGETS)

depend:
	makedepnd -f$(DEPENDFILE) *.c

clean:
	rm -rf *.o
	rm -rf *.a

install:
	cp $(TARGETS) ../Lib

# DO NOT DELETE
