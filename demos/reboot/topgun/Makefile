#!smake -k
#
# Makefile for topgun demo application 
#
#
#

GCC_CELF = OFF
DDPROCDIR = ../ddproc
include $(ROOT)/usr/include/make/PRdefs
 
ifeq ($(FINAL),YES)
	ARGS= FINAL
        OPTIMIZER = -O2					 	# (c) Optimizes in the maximum level.
#       LCDEFS    =						  
        N64LIB    = -lleo -lgultra_rom	 	# (l) Includes the library.
else
        OPTIMIZER = -g					    # (c) Creates the debug information.	
        LCDEFS   += -D_RDB_	-D_DEBUG		# (c) #define DEBUG 
        N64LIB    = -lleo_d -lgultra_d      # (l) Includes the library for debugging.
	LCOPTS+=	-Wswitch -Wreturn-type -Werror -W -Wstrict-prototypes
	LCOPTS+=	-Wimplicit -Wmissing-prototypes
    LCOPTS+=	-Wunused
#	LCOPTS+=    -Wredundant-decls	-Wcast-qual  					# (c) Warning types.
endif

APP =		topgun.out
TARGETS =	topgun.n64

TEXHFILES =	ground.h sky.h tree.h
TEXTURES =	Textures/clouds.rgb Textures/harrier_num.rgba \
		Textures/stree1.rgba  Textures/terrain.rgb \
		Textures/camo2.int Textures/decal.inta

MODELS =	Models/av8b.flt

HFILES =	topgun.h

CODEFILES   =	topgun.c dram_stack.c rdp_output.c rdp_output_len.c \
				dd_proc.c dd_rtc.c dd_summon.c err.c menu.c

CODEOBJECTS =	$(CODEFILES:.c=.o)

DATAFILES   =	av8b.c		\
		cfb.c		\
		ground.c	\
		static.c	\
		zbuffer.c   \

DATAOBJECTS =	$(DATAFILES:.c=.o)

DDPROCOBJECT = ddproc.o

CODESEGMENT =	codesegment.o

OBJECTS =	$(CODESEGMENT) $(DATAOBJECTS)

LCINCS =	-I. -I$(ROOT)/usr/include/PR -I $(ROOT)/usr/include -I$(DDPROCDIR)
LCOPTS+=	-G 0

LDIRT  =	$(APP) topgun

LDFLAGS =	$(MKDEPOPT) -L$(ROOT)/usr/lib $(N64LIB) -L$(GCCDIR)/mipse/lib -lkmc 

default:	$(TARGETS)

include $(COMMONRULES)

$(CODESEGMENT):	$(CODEOBJECTS) $(DDPROCOBJECT)
		$(LD) -o $(CODESEGMENT) -r $(CODEOBJECTS) $(DDPROCOBJECT) $(LDFLAGS)

ifeq ($(FINAL),YES)
$(TARGETS) $(APP):	spec $(OBJECTS)
		$(MAKEROM) spec -s 9 -r $(TARGETS) -e $(APP) -B 0
		makemask $(TARGETS)
else
$(TARGETS) $(APP):	spec $(OBJECTS)
		$(MAKEROM) spec -r $(TARGETS) -e $(APP) -B 0
endif

# for exeGCC CELF
ifeq ($(GCC_CELF), ON)
CELFDIR = .
include $(CELFRULES)
$(CODEOBJECTS) $(DATAOBJECTS) :	$(CELFINFOFILES)
endif


ddproc.o: $(DDPROCDIR)\ddproc.o
        mkddproc $(ARGS)

$(DDPROCDIR)\ddproc.o : 
        mkddproc $(ARGS)

