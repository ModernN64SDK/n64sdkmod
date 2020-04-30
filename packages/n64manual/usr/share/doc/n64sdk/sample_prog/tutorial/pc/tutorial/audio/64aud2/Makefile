.SUFFIXES:
.SUFFIXES:	.o .c

include $(ROOT)/usr/include/make/PRdefs

########## Please customize the following macros as you see fit. ##########

# If N_AUDIO is set to 0, N_AUDIO will be disabled.
N_AUDIO		= 0

# If DEBUGSYM is set to an null text string, you will be unable to generate debug symbols.
DEBUGSYM	= -g

# If the following compiler flag is changed, the optimization level will also change.
OPTIMIZER	= -O0

# NuSystem related directories
N64KITDIR	= c:/nintendo/n64kit

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

ifeq	($(N_AUDIO), 1)
CUSTFLAGS	= -DN_AUDIO
LCINCS		= -I. -I$(NUSYSINC) -I$(ROOT)/usr/include -I$(ROOT)/usr/include/PR
LCOPTS		= -G 0 $(DEBUGSYM) $(CUSTFLAGS)
LDFLAGS		= -L$(ROOT)/usr/lib -L$(ROOT)/usr/lib/PR -L$(NUSYSLIB) \
		  -ln_gmus_d -ln_gaudio_sc -lnualstl_d -lnusys_d -lgultra_d \
		  -L$(GCCDIR)/mipse/lib -lkmc
else
CUSTFLAGS	= 
LCINCS		= -I. -I$(NUSYSINC) -I$(ROOT)/usr/include -I$(ROOT)/usr/include/PR
LCOPTS		= -G 0 $(DEBUGSYM) $(CUSTFLAGS)
LDFLAGS		= -L$(ROOT)/usr/lib -L$(ROOT)/usr/lib/PR -L$(NUSYSLIB) \
		  -lgmus_d -lnualstl_d -lnusys_d -lgultra_d \
		  -L$(GCCDIR)/mipse/lib -lkmc
endif

CODESEGMENT	= codesegment.o

OBJECTS		= $(CODESEGMENT)

DATADIR		= ../data
DATAFILES	= $(DATADIR)/demo2_1.ptr $(DATADIR)/demo2_1.wbk \
		  $(DATADIR)/demo2_1.bin $(DATADIR)/demo2_2.bin \
		  $(DATADIR)/demo2_1.bfx

SYMBOL		= $(TARGET).out
ROM		= $(TARGET).n64

default:	$(ROM)

$(CODESEGMENT):	$(CODEOBJECTS)
		$(LD) -o $(CODESEGMENT) -r $(CODEOBJECTS) $(LDFLAGS)

$(ROM) :	$(OBJECTS) spec $(DATAFILES)
		$(MAKEROM) spec -I$(NUSYSINC) -r $(ROM) -e $(SYMBOL) $(CUSTFLAGS)

main.o:			main.c $(DEPEND_ON_MAKE)
menu.o:			menu.c menu.h sound.h define.h $(DEPEND_ON_MAKE)
sound.o:		sound.c sound.h menu.h rom.h segment.h $(DEPEND_ON_MAKE)
rom.o:			rom.c $(DEPEND_ON_MAKE)

include $(ROOT)/usr/include/make/commonrules
