.SUFFIXES:
.SUFFIXES:	.o .c

include $(ROOT)/usr/include/make/PRdefs

########## Please customize the following macros as you see fit. ##########

# Go ahead and "make" GFXDL with one of the gfx*.c files.
# Each gfx*.c source file contains macros defined in the header that are
# customizable.
GFXDL		= gfx1.c

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

TARGET		= gfx

CODEFILES	= main.c $(GFXDL)
NUOBJ		= $(NUSYSLIB)/nusys.o
CODEOBJECTS	= $(CODEFILES:.c=.o) $(NUOBJ)

CUSTFLAGS	= -DSGI
LCINCS		= -I$(NUSYSINC) -I./Textures -I./Models
LCOPTS		= -G 0 -woff 709 -non_shared $(DEBUGSYM) $(CUSTFLAGS)
LDFLAGS		= -nostdlib -L$(ROOT)/usr/lib -L$(ROOT)/usr/lib/PR -L$(NUSYSLIB) \
		  -lnusys_d -lultra_d

CODESEGMENT	= codesegment.o

OBJECTS		= $(CODESEGMENT)

SYMBOL		= $(TARGET)
ROM		= rom

default:	$(ROM)

$(CODESEGMENT):	$(CODEOBJECTS)
		$(LD) -o $(CODESEGMENT) -r $(CODEOBJECTS) $(LDFLAGS)

$(ROM) :	$(OBJECTS)
		$(MAKEROM) -I$(NUSYSINC) -r $(ROM) spec

main.o:		main.c define.h $(DEPEND_ON_MAKE)
$(GFXDL:.c=.o):	$(GFXDL) define.h $(DEPEND_ON_MAKE)

include $(ROOT)/usr/include/make/commonrules
