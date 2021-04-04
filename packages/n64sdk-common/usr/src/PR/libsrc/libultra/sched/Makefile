#!smake
#
# Makefile for the scheduler subdirectory
#

include /usr/include/n64/make/PRdefs

# LCOPTS = -mips2 -non_shared -fullwarn -G 0
LCINCS = -I. -I/usr/include/n64/PR -I/usr/include/n64

CFILES  = \
	sched.c

OBJECTS =	$(CFILES:.c=.o)

default: $(OBJECTS)

include $(COMMONRULES)
