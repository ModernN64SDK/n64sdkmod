.SUFFIXES:
.SUFFIXES:	.o .c

include $(ROOT)/usr/include/make/PRdefs

########## Please customize the following macros as you see fit. ##########

# If N_AUDIO is set to 0, N_AUDIO will be disabled.
N_AUDIO		= 

# If DEBUGSYM is set to an null text string, you will be unable to generate debug symbols.
DEBUGSYM	= -g

# If the following compiler flag is changed, the optimization level will also change. 
OPTIMIZER	= -O

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

TARGET		= 64aud3

CODEFILES	= main.c menu.c sound.c rom.c \
		  menu0.c menu1.c
NUOBJ		= $(NUSYSLIB)/nusys.o
CODEOBJECTS	= $(CODEFILES:.c=.o) $(NUOBJ)

#ifdef	N_AUDIO)
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
DATAFILES	= $(DATADIR)/groove.ptr $(DATADIR)/groove.wbk $(DATADIR)/groove.bin \
		  $(DATADIR)/orch.ptr $(DATADIR)/orch.wbk $(DATADIR)/orch.bin \
		  $(DATADIR)/rock.ptr $(DATADIR)/rock.wbk $(DATADIR)/rock.bin \
		  $(DATADIR)/demo3_4.ptr $(DATADIR)/demo3_4.wbk $(DATADIR)/demo3_4.bfx \
		  $(DATADIR)/demo3_5.ptr $(DATADIR)/demo3_5.wbk $(DATADIR)/demo3_5.bfx

SYMBOL		= $(TARGET)
ROM		= rom

default:	$(ROM)

$(CODESEGMENT):	$(CODEOBJECTS)
		$(LD) -o $(CODESEGMENT) -r $(CODEOBJECTS) $(LDFLAGS)

$(ROM) :	$(OBJECTS) spec $(DATAFILES)
		$(MAKEROM) -I$(NUSYSINC) -r $(ROM) $(CUSTFLAGS) spec

main.o:			main.c $(DEPEND_ON_MAKE)
menu.o:			menu.c menu.h define.h $(DEPEND_ON_MAKE)
menu0.o:		menu0.c menu.h sound.h define.h $(DEPEND_ON_MAKE)
menu1.o:		menu1.c menu.h sound.h define.h $(DEPEND_ON_MAKE)
sound.o:		sound.c sound.h menu.h define.h rom.h segment.h $(DEPEND_ON_MAKE)
rom.o:			rom.c $(DEPEND_ON_MAKE)

include $(ROOT)/usr/include/make/commonrules
