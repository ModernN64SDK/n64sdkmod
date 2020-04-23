#!smake -k
#
# Makefile for onetri demo application 
#
#

GCC_CELF=OFF
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

APP     =	hello.out
TARGETS =	hello.n64

HFILES =	$(TEXHFILES) onetri.h

CODEFILES   =	hello_world.c dram_stack.c rdp_output.c rdp_output_len.c \
				dd_proc.c static.c 

CODEOBJECTS =	$(CODEFILES:.c=.o)

DATAFILES   =	cfb.c rsp_cfb.c

DATAOBJECTS =	$(DATAFILES:.c=.o)

DDPROCOBJECT =	ddproc.o
CODESEGMENT =	codesegment.o

OBJECTS =	$(CODESEGMENT) $(DATAOBJECTS)

LCINCS =	-I. -I$(DDPROCDIR)
LCOPTS+=	-G 0
DDDEFS =	-DD64 -t 0

LDFLAGS =	$(MKDEPOPT) -L$(ROOT)/usr/lib $(N64LIB) -L$(GCCDIR)/mipse/lib -lkmc 

LDIRT  =	$(APP) gwrite

default:	$(TARGETS)

include $(COMMONRULES)

$(CODESEGMENT):	$(CODEOBJECTS) $(DDPROCOBJECT)
		$(LD) -o $(CODESEGMENT) -r $(CODEOBJECTS)  $(DDPROCOBJECT) $(LDFLAGS)

$(TARGETS) $(APP):	spec $(OBJECTS)
		$(MAKEROM) $(DDDEFS) -r $(TARGETS) -e $(APP) spec

# for exeGCC CELF
ifeq ($(GCC_CELF), ON)
CELFDIR = .
include $(CELFRULES)
$(CODEOBJECTS) $(DATAOBJECTS) :	$(CELFINFOFILES)
endif

$(DDPROCOBJECT): $(DDPROCDIR)\ddproc2.o
        mkddproc $(ARGS)

$(DDPROCDIR)\ddproc2.o:
	mkddproc $(ARGS)
