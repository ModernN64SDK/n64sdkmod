#!smake
#
# Makefile for the sp subdirectory
#

include /usr/include/n64/make/PRdefs


# LCOPTS = -mips2 -non_shared -fullwarn -G 0
LCINCS = -I. -I$(ROOT)/usr/include/PR -I$(ROOT)/usr/include

CFILES  = 			\
	clearattribute.c	\
	color.c			\
	hide.c			\
	setattribute.c		\
	show.c			\
	sprite.c		\
	spriteEX.c		\
	spriteEX2.c		\
	spscale.c


ASFILES =

OBJECTS =	$(CFILES:.c=.o)

default: $(OBJECTS)

include $(COMMONRULES)
