# 
#  N64-HVQM2 library  Sample program
#
#  FILE : Makefile
# 
#  Copyright (C) 1998,1999 NINTENDO Co.,Ltd.
# 

# 1999-04-21

include /usr/include/n64/make/PRdefs

######################################################################
#  Kind of HVQM2 decoder being used
#    0 : CPU version
#    1 : CPU+RSP version
USE_RSP = 1

#  Whether to forcedly synchronize video to audio when video playback is late
#    0 : Decode every frame (do not synchronize)
#    1 : Skip frames to force video into sync
SYNC_VIDEO = 1

#  Format of display-use frame buffer (selected HVQM2 decoder also reflects on this)
#    1 : 16-bit
#    2 : 32-bit
CFB_FORMAT = 1

#  Maximum size of image [pixel]  (reflects on memory reserved for hvqmwork.c)
MAXWIDTH  = 320
MAXHEIGHT = 240

######################################################################
#  The directory holding the HVQM2 library (libhvqm2.a)
HVQMLIBDIR = /usr/lib/n64/hvqm/

#  The directory holding the HVQM2 include file
HVQMINCDIR = /usr/include/n64/hvqm/

#  The directory holding the HVQM2 microcode
HVQMUCODEDIR = /usr/lib/n64/hvqm/
######################################################################

LCINCS = -I. -I$(HVQMINCDIR) -I/usr/include/n64/PR -I/usr/include/n64
LCOPTS = -O2 -G 0 -g -mno-shared -std=gnu90 
LCDEFS = -D_LANGUAGE_C -DMAXWIDTH=$(MAXWIDTH) -DMAXHEIGHT=$(MAXHEIGHT) -DCFB_FORMAT=$(CFB_FORMAT) -DUSE_RSP=$(USE_RSP) -DSYNC_VIDEO=$(SYNC_VIDEO)
LDIRT  = $(APP) $(TARGETS)

LIBS = -L$(HVQMLIBDIR) -lhvqm2 -L/usr/lib/n64 -lultra_rom -L$(N64_LIBGCCDIR) -lgcc

#UCODE = $(HVQMUCODEDIR)/hvqm2sp$(CFB_FORMAT).o

NAME = sample

TARGETDIR = .
ROM = $(TARGETDIR)/$(NAME).n64
SYM = $(TARGETDIR)/$(NAME).out

HVQMFILE  = sample.hvqm

SPECFILE = spec

CFILES      = system.c main.c cfbkeep.c getrecord.c timekeeper.c
CODEOBJECTS = $(CFILES:.c=.o)
CODESEGMENT = codesegment.o

DATAFILES   = hvqmwork.c adpcmbuf.c hvqbuf.c pcmbuf.c cfb.c
DATAOBJECTS = $(DATAFILES:.c=.o)

default: $(ROM)

include $(COMMONRULES)

$(ROM): $(CODESEGMENT) $(DATAOBJECTS) $(HVQMFILE) $(SPECFILE) Makefile
	$(MAKEROM) -r $(ROM) -e $(SYM) $(SPECFILE)
	makemask $(ROM)

$(CODESEGMENT): $(CODEOBJECTS)
	$(LD) -o $(CODESEGMENT) -r $(CODEOBJECTS) $(LIBS)

system.o main.o cfbkeep.o getrecord.o timekeeper.o \
hvqmwork.o adpcmbuf.o hvqbuf.o pcmbuf.o cfb.o :  system.h

main.o timekeeper.o :  timekeeper.h

system.o main.o hvqmwork.o cfb.o :  Makefile

######################################################################
