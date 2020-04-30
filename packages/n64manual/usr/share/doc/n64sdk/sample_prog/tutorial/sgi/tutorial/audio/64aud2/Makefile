.SUFFIXES:
.SUFFIXES:	.o .c

include $(ROOT)/usr/include/make/PRdefs

########## Please customize the following macros as you see fit. ##########

# If N_AUDIO is set to 0, N_AUDIO will be disabled.
N_AUDIO		= 

# If DEBUGSYM is set to an null text string, you will be unable to generate debug symbols.
DEBUGSYM	= -g

# If the following compiler flag is changed, the optimization level will also change.
OPTIMIZER	= -O0

# NuSystem related directories
N64KITDIR	= /usr/local/n64kit

NUSYSDIR	= $(N64KITDIR)/nusys
NUSYSINC	= $(NUSYSDIR)/include
NUSYSLIB	= $(NUSYSDIR)/lib

# When an object is set up to be dependent on Makefile, 
# set DEPEND_ON_MAKE = Makefile.
# Comment this line out if you do not want it to be dependent.
DEPEND_ON_MAKE  = Makefile

########## You may customize the following. ##########

TARGET		= 64aud2

CODEFILES	= main.c menu.c sound.c rom.c
NUOBJ		= $(NUSYSLIB)/nusys.o
CODEOBJECTS	= $(CODEFILES:.c=.o) $(NUOBJ)

#ifdef	N_AUDIO
CUSTFLAGS	= -DSGI -DN_AUDIO
LCINCS		= -I. -I$(NUSYSINC) -I$(ROOT)/usr/include -I$(ROOT)/usr/include/PR
LCOPTS		= -G 0 -woff 709 -non_shared $(DEBUGSYM) $(CUSTFLAGS)
LDFLAGS		= -nostdlib -L$(ROOT)/usr/lib -L$(ROOT)/usr/lib/PR -L$(NUSYSLIB) \
		  -ln_mus_d -ln_audio_sc -lnualstl_d -lnusys_d -lultra_d
#else
CUSTFLAGS	= -DSGI
LCINCS		= -I. -I$(NUSYSINC) -I$(ROOT)/usr/include -I$(ROOT)/usr/include/PR
LCOPTS		= -G 0 -woff 709 -non_shared $(DEBUGSYM) $(CUSTFLAGS)
LDFLAGS		= -nostdlib -L$(ROOT)/usr/lib -L$(ROOT)/usr/lib/PR -L$(NUSYSLIB) \
		  -lmus_d -lnualstl_d -lnusys_d -lultra_d
#endif

CODESEGMENT	= codesegment.o

OBJECTS		= $(CODESEGMENT)

DATADIR		= ../data
DATAFILES	= $(DATADIR)/demo2_1.ptr $(DATADIR)/demo2_1.wbk \
		  $(DATADIR)/demo2_1.bin $(DATADIR)/demo2_2.bin \
		  $(DATADIR)/demo2_1.bfx

SYMBOL		= $(TARGET)
ROM		= rom

default:	$(ROM)

$(CODESEGMENT):	$(CODEOBJECTS)
		$(LD) -o $(CODESEGMENT) -r $(CODEOBJECTS) $(LDFLAGS)

$(ROM) :	$(OBJECTS) spec $(DATAFILES)
		$(MAKEROM) -I$(NUSYSINC) -r $(ROM) $(CUSTFLAGS) spec

main.o:			main.c $(DEPEND_ON_MAKE)
menu.o:			menu.c menu.h sound.h define.h $(DEPEND_ON_MAKE)
sound.o:		sound.c sound.h menu.h rom.h segment.h $(DEPEND_ON_MAKE)
rom.o:			rom.c $(DEPEND_ON_MAKE)

include $(ROOT)/usr/include/make/commonrules
