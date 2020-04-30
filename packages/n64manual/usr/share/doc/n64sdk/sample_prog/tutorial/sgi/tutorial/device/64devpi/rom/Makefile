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

# If you are using Developers' Kit 4.0, please set to 1,
# and if you are using Developers' Kit before 4.0, please set to 0.
DEVKIT4		= 1

# When an object is set up to be dependent on Makefile, 
# set DEPEND_ON_MAKE = Makefile.
# Comment this line out if you do not want it to be dependent.
DEPEND_ON_MAKE  = Makefile

########## You may customize the following. ##########

TARGET		= 64devrom

CODEFILES	= main.c common.c mainmenu.c rommenu.c srammenu.c
NUOBJ		= $(NUSYSLIB)/nusys.o
CODEOBJECTS	= $(CODEFILES:.c=.o) $(NUOBJ)

CUSTFLAGS	= -DSGI -DDEVKIT4=$(DEVKIT4)
LCINCS		= -I$(NUSYSINC)
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

main.o:		main.c mainmenu.h $(DEPEND_ON_MAKE)
common.o:	common.c common.h $(DEPEND_ON_MAKE)
mainmenu.o:	mainmenu.c mainmenu.h common.h rommenu.h srammenu.h $(DEPEND_ON_MAKE)
rommenu.o:	rommenu.c rommenu.h common.h $(DEPEND_ON_MAKE)
srammenu.o:	srammenu.c srammenu.h common.h $(DEPEND_ON_MAKE)

include $(ROOT)/usr/include/make/commonrules
