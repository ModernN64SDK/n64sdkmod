#!smake
#
# Makefile for the gt subdirectory
#

include /usr/include/n64/make/PRdefs

# LCOPTS = -mips2 -non_shared -fullwarn -G 0

LCINCS = -I. -I/usr/include/n64/PR -I/usr/include/n64

CFILES  = dumpturbo.c gt.c

OBJECTS = $(CFILES:.c=.o)

default: $(OBJECTS)

include $(COMMONRULES)
