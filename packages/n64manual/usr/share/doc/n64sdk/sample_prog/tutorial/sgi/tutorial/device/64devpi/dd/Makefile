.SUFFIXES:
.SUFFIXES:	.o .c

include $(ROOT)/usr/include/make/PRdefs

########## Please customize the following macros as you see fit. ##########

# If DEBUGSYM is set to an null text string, you will be unable to generate debug symbols.
DEBUGSYM	= -g

# If the following compiler flag is changed, the optimization level will also change.
OPTIMIZER	= -O0

# NuSystem related directories
N64KITDIR	= /usr/local/n64kit

NUSYSDIR	= $(N64KITDIR)/nusys
NUSYSINC	= $(NUSYSDIR)/include
NUSYSLIB	= $(NUSYSDIR)/lib

MFSDIR		= $(N64KITDIR)/mfs
MFSINC		= $(MFSDIR)
MFSLIB		= $(MFSDIR)

# When an object is set up to be dependent on Makefile, 
# set DEPEND_ON_MAKE = Makefile.
# Comment this line out if you do not want it to be dependent.
DEPEND_ON_MAKE  = Makefile

########## You may customize the following. ##########

TARGET		= 64devdd

CODEFILES	= main.c ddmenu.c
NUOBJ		= $(NUSYSLIB)/nusys.o
CODEOBJECTS	= $(CODEFILES:.c=.o) $(NUOBJ)

CUSTFLAGS	=
LCINCS		= -I$(NUSYSINC) -I$(MFSINC)
LCOPTS		= -G 0 -woff 709 -non_shared $(DEBUGSYM) $(CUSTFLAGS)
LDFLAGS		= -L$(ROOT)/usr/lib -L$(ROOT)/usr/lib/PR -L$(NUSYSLIB) -L$(MFSLIB) \
		  -lnusys_d -lmfs_d -lleo_d -lultra_d

CODESEGMENT	= codesegment.o

OBJECTS		= $(CODESEGMENT)

SYMBOL		= $(TARGET)
ROM		= rom

default:	$(ROM)

$(CODESEGMENT):	$(CODEOBJECTS)
		$(LD) -o $(CODESEGMENT) -r $(CODEOBJECTS) $(LDFLAGS)

$(ROM) :	$(OBJECTS)
		$(ROOT)/usr/sbin/makedisk -t 0 -I$(NUSYSINC) -R $(ROM) spec

main.o:		main.c ddmenu.h define.h $(DEPEND_ON_MAKE)
ddmenu.o:	ddmenu.c ddmenu.h define.h $(DEPEND_ON_MAKE)

include $(ROOT)/usr/include/make/commonrules
